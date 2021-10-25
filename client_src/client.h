#ifndef CLIENT_SRC_CLIENT_H_
#define CLIENT_SRC_CLIENT_H_

#define COMMAND_EXIT "exit"
#define COMMAND_PUSH "push"
#define COMMAND_POP "pop"
#define COMMAND_DEFINE "define"

/// Starts the client module for the middleware with the
/// desired configuration.
/// \param service Service name of the server.
/// \param port Port of the server
/// \return Returns 1 if the parameters where invalid,
/// 2 if there was an error, 0 otherwise.
int run(const char* service, const char* port);

#endif  // CLIENT_SRC_CLIENT_H_
