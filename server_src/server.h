#ifndef SERVER_SRC_SERVER_H_
#define SERVER_SRC_SERVER_H_

#define COMMAND_EXIT 'q'
#define COMMAND_PUSH 'u'
#define COMMAND_POP 'o'
#define COMMAND_DEFINE 'd'

/// Starts the server module for the middleware with the
/// desired configuration.
/// \param port Port where to run the server.
/// \return Returns 1 if the parameters where invalid,
/// 2 if there was an error, 0 otherwise.
int run(const char* port);

#endif  // SERVER_SRC_SERVER_H_
