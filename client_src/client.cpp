#include "client.h"

#include <sys/syslog.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <utility>
#include <string>
#include <vector>
#include "../common_src/Protocol.h"
#include "../common_src/Socket.h"

std::string read_line_() {
    std::string c;
    std::getline(std::cin, c);
    return c;
}

std::vector<std::string> parse_command_(const std::string& command) {
    std::vector<std::string> result;
    std::stringstream s(command);
    std::string segment;

    while (getline(s, segment, ' ')) {
        result.push_back(segment);
    }

    return result;
}

char get_command_char_(std::basic_string<char> const &command) {
    if (command == COMMAND_DEFINE) {
        return 'd';
    }
    if (command == COMMAND_PUSH) {
        return 'u';
    }
    if (command == COMMAND_POP) {
        return 'o';
    }
    throw std::invalid_argument("Invalid command.");
}

void execute_command_(Protocol const &protocol,
                      std::vector<std::string> const &command) {
    protocol.sendCommand(get_command_char_(command.front()));
    protocol.send_word(command.at(1));

    if (command.front() == COMMAND_PUSH) {
        protocol.send_word(command.at(2));
    }
    if (command.front() == COMMAND_POP) {
        std::cout << protocol.get_word() << "\n";
    }
}

int run(const char *service, const char *port) {
    Socket socket;

    try {
        socket.connect(port, service);
    } catch (SocketException &e) {
        syslog(LOG_ERR, "[CLIENT] %s", e.what());
        closelog();
        return 2;
    }

    Protocol protocol(std::move(socket));

    while (true) {
        std::vector<std::string> command = parse_command_(read_line_());
        if (command.front() == COMMAND_EXIT) {
            break;
        }

        try {
            execute_command_(protocol, command);
        } catch (std::invalid_argument &e) {
            syslog(LOG_INFO, "[CLIENT] %s", e.what());
        } catch (SocketException &e) {
            syslog(LOG_INFO, "[CLIENT] %s", e.what());
            break;
        }
    }

    protocol.shutdown();
    closelog();
    return EXIT_SUCCESS;
}
