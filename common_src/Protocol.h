#ifndef COMMON_SRC_PROTOCOL_H_
#define COMMON_SRC_PROTOCOL_H_

#include <string>
#include "Socket.h"

class Protocol {
  Socket socket_;
  void sendChar(char c);

 public:
  explicit Protocol(Socket socket);
  void sendCommand(char c);
  char getCommand();
  void send_word(const std::string& word);
  std::string get_word();
  void shutdown();
};

#endif  // COMMON_SRC_PROTOCOL_H_
