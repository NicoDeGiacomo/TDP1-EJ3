#include "client.h"

#include <cstdlib>
#include <iostream>
#include "QueueManager.h"

int run(const char *service, const char *port) {
    std::string command;
    std::cin >> command;

    if (command == COMMAND_EXIT) {
        return EXIT_SUCCESS;
    }

    QueueManager<std::string> manager;
    
    return EXIT_SUCCESS;
}
