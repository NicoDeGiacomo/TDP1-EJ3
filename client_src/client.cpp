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

std::vector<std::string> parse_command_(const std::string& command) {
    std::vector<std::string> result;
    std::stringstream s(command);
    std::string segment;

    while (getline(s, segment, ' ')) {
        result.push_back(segment);
    }

    return result;
}

int run(const char *service, const char *port) {
    Socket socket;
    socket.connect(port, service);

    Protocol protocol(std::move(socket));  // todo option para no pasar socket

    while (true) {
        std::string c;
        std::getline(std::cin, c);

        // todo validaciones
        std::vector<std::string> command = parse_command_(c);

        try {
            if (command.front() == COMMAND_EXIT) {
                return EXIT_SUCCESS;
            }

            if (command.front() == COMMAND_DEFINE) {
                protocol.sendCommand('d');
                protocol.send_word(command.at(1));
                continue;
            }

            if (command.front() == COMMAND_PUSH) {
                protocol.sendCommand('u');
                protocol.send_word(command.at(1));
                protocol.send_word(command.at(2));
                continue;
            }

            if (command.front() == COMMAND_POP) {
                protocol.sendCommand('o');
                protocol.send_word(command.at(1));
                std::cout << protocol.get_word() << "\n";
                continue;
            }
        } catch (SocketException &e) {
            protocol.shutdown();
            syslog(LOG_INFO, "[CLIENT] %s", e.what());
            return EXIT_SUCCESS;
        }
    }
}
