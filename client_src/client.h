#ifndef CLIENT_SRC_CLIENT_H_
#define CLIENT_SRC_CLIENT_H_

#define COMMAND_EXIT "exit"
#define COMMAND_PUSH "push"
#define COMMAND_POP "pop"
#define COMMAND_DEFINE "define"

int run(const char* service, const char* port);

#endif  // CLIENT_SRC_CLIENT_H_
