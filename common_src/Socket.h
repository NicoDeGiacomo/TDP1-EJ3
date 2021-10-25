#ifndef COMMON_SRC_SOCKET_H_
#define COMMON_SRC_SOCKET_H_

#include <exception>
#include <string>
#include <utility>

/// Implementation of a TCP Socket.
class Socket {
 private:
  int fd;
  explicit Socket(int fd);
  static struct addrinfo* get_addresses(const char* port, const char* ip);

 public:
  /// Creates a Socket.
  Socket();

  /// Destroys a Socket.
  ~Socket();

  /// Moves a Socket.
  /// \param other Socket to move.
  Socket(Socket &&other) noexcept;

  /// Assignation operator.
  /// \param other Socket to assign.
  /// \return The newly assigned Socket.
  Socket& operator=(Socket&& other) noexcept;

  /// Binds the Socket to a port.
  /// \param port Port to bind.
  /// \throw SocketException if there was an error.
  void bind(const char* port);

  /// Listen for connections.
  /// \param size Size of the connection queue.
  /// \throw SocketException if there was an error.
  void listen(int size) const;

  /// Accepts a client. Returns a new Socket with the connection to the client.
  /// \throw ClosedSocketException if the Socket was closed.
  Socket accept() const;

  /// Connects to a Socket with the desired service name and port.
  /// \param port Port to connect to.
  /// \param name Service name to connect to.
  /// \throw SocketException if there was an error.
  void connect(const char *port, const char *name);

  /// Sends and array of bytes.
  /// \param buffer Array of bytes to send.
  /// \param size Length of the array of bytes.
  /// \throw SocketException or ClosedSocketException accordingly.
  void send(const char *buffer, unsigned int size) const;

  /// Receives and array of bytes
  /// \param buffer Array of bytes to receive.
  /// \param size Length of the array of bytes.
  /// \throw SocketException or ClosedSocketException accordingly.
  void receive(char* buffer, unsigned int size) const;

  /// Shutdowns the Socket.
  void shutdown();
};

/// Exception thrown when an error was produced.
class SocketException;

/// Exception thrown when the Socket was closed.
struct ClosedSocketException;

/****************** IMPLEMENTATION ******************/
class SocketException: public std::exception {
 private:
  std::string msg_;
 public:
  explicit SocketException(std::string message) : msg_(std::move(message)) {}
  const char* what() const noexcept override {
      return msg_.c_str();
  }
};

struct ClosedSocketException : public SocketException {
  ClosedSocketException() : SocketException("The socket is closed.") {}
};

#endif  // COMMON_SRC_SOCKET_H_
