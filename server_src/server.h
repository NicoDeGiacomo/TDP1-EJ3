#ifndef SERVER_SRC_SERVER_H_
#define SERVER_SRC_SERVER_H_

#define COMMAND_EXIT 'q'
#define COMMAND_PUSH 'u'
#define COMMAND_POP 'o'
#define COMMAND_DEFINE 'd'

int run(const char* port);

#endif  // SERVER_SRC_SERVER_H_
