#ifndef COMMON_SRC_PROTOCOL_H_
#define COMMON_SRC_PROTOCOL_H_

#include <string>
#include "Socket.h"

class Protocol {
  Socket socket_;
  void sendChar(char c) const;

 public:
  explicit Protocol(Socket socket);
  void sendCommand(char c) const;
  char getCommand() const;
  void send_word(const std::string& word) const;
  std::string get_word() const;
  void shutdown();
};

#endif  // COMMON_SRC_PROTOCOL_H_
