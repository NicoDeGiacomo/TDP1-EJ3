#ifndef COMMON_SRC_PROTOCOL_H_
#define COMMON_SRC_PROTOCOL_H_

#include <string>
#include "Socket.h"

/// Implementation of the communication protocol between server and clients.
class Protocol {
  Socket socket_;
  void sendChar(char c) const;

 public:
  /// Creates the Protocol.
  /// \param socket Socket to use to send an receive data.
  /// \throw Exceptions thrown by the Socket.
  explicit Protocol(Socket socket);

  /// Sends a command in the form of a char.
  /// \param c Command to send.
  /// \throw Exceptions thrown by the Socket.
  void sendCommand(char c) const;

  /// Reads a command in the form of a char.
  /// \return Command received.
  /// \throw Exceptions thrown by the Socket.
  char getCommand() const;

  /// Sends two bytes representing the length of the word,
  /// and sends the word afterwards.
  /// \param word Word to send.
  /// \throw Exceptions thrown by the Socket.
  void send_word(const std::string& word) const;

  /// Reads a word.
  /// \return Word read as a string.
  /// \throw Exceptions thrown by the Socket.
  std::string get_word() const;

  /// Shutdowns the socket, and the communication ceases.
  void shutdown();
};

#endif  // COMMON_SRC_PROTOCOL_H_
