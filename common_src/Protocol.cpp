#include "Protocol.h"

#include <netinet/in.h>
#include <utility>
#include <string>

#define MAX_WORD_SIZE 65535  // word size is two bytes max

Protocol::Protocol(Socket socket) : socket_(std::move(socket)) {}

void Protocol::sendChar(char c) {
    socket_.send(&c, 1);
}

void Protocol::sendCommand(char c) {
    sendChar(c);
}

char Protocol::getCommand() {
    char c;
    socket_.receive(&c, 1);
    return c;
}

void Protocol::send_word(const std::string& word) {
    sendChar((char) htons(word.size()));
    sendChar((char) (htons(word.size()) >> 8));
    socket_.send(word.c_str(), word.size());
}

std::string Protocol::get_word() {
    char buffer[2];
    socket_.receive(&buffer[0], 2);

    const size_t kSize = ntohs(buffer[0] + (buffer[1] << 8));
    char word[MAX_WORD_SIZE];

    socket_.receive(word, kSize);
    word[kSize] = '\0';

    return std::move(std::string(word));
}

void Protocol::shutdown() {
    socket_.shutdown();
}


