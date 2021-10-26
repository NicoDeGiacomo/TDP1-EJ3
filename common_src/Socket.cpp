#include "Socket.h"

#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

Socket::Socket() : fd(-1) {}

Socket::Socket(int fd) : fd(fd) {}

struct addrinfo *Socket::get_addresses(const char *port, const char *ip) {
    struct addrinfo* addresses;
    struct addrinfo hints{};
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = (ip == nullptr) ? AI_PASSIVE : 0;
    if (getaddrinfo(ip, port, &hints, &addresses) != 0) {
        throw SocketException("Failed getting address information.");
    }
    return addresses;
}

void Socket::bind(const char *port) {
    struct addrinfo* addresses = get_addresses(port, nullptr);
    int val = 1;
    for (struct addrinfo* i = addresses; i != nullptr; i = i->ai_next) {
        int skt = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
        ::setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
        if (skt != -1) {
            if (::bind(skt, i->ai_addr, i->ai_addrlen) != -1) {
                fd = skt;
                break;
            }
            close(skt);
        }
    }
    freeaddrinfo(addresses);

    if (fd == -1) {
        throw SocketException("Failed to bind Socket.");
    }
}

void Socket::listen(int size) const {
    if (::listen(fd, size) == -1) {
        throw SocketException("Failed to listen.");
    }
}

Socket Socket::accept() const {
    int skt = ::accept(fd, nullptr, nullptr);
    if (skt == -1) {
        throw ClosedSocketException();
    }
    return Socket(skt);
}

void Socket::connect(const char* port, const char* name) {
    struct addrinfo* addresses = get_addresses(port, name);
    struct addrinfo* a;
    for (a = addresses; a != nullptr; a = a->ai_next) {
        int skt = ::socket(a->ai_family, a->ai_socktype, a->ai_protocol);
        if (skt != -1) {
            if (::connect(skt, a->ai_addr, a->ai_addrlen) != -1) {
                fd = skt;
                break;
            }
            close(skt);
        }
    }
    freeaddrinfo(addresses);

    if (a == nullptr) {
        throw SocketException("Error connecting to server.");
    }
}

void Socket::send(const char *buffer, unsigned int size) const {
    size_t sent = 0;
    while (sent < size) {
        ssize_t s = ::send(fd, &buffer[sent], size - sent, MSG_NOSIGNAL);
        if (s == -1) {
            throw SocketException("Error sending bytes.");
        }
        if (s == 0) {
            throw ClosedSocketException();
        }
        sent += s;
    }
}

void Socket::receive(char* buffer, unsigned int size) const {
    size_t received = 0;
    while (received < size) {
        ssize_t s = ::recv(fd, &buffer[received], size - received, 0);
        if (s == -1) {
            throw SocketException("Error receiving bytes.");
        }
        if (s == 0) {
            throw ClosedSocketException();
        }
        received += s;
    }
}

Socket::Socket(Socket&& other) noexcept {
    fd = other.fd;
    other.fd = -1;
}

Socket &Socket::operator=(Socket &&other)  noexcept {
    if (this != &other) {
        fd = other.fd;
        other.fd = -1;
    }
    return *this;
}

void Socket::shutdown() {
    if (fd != -1) {
        ::shutdown(fd, SHUT_RDWR);
        close(fd);
        fd = -1;
    }
}

Socket::~Socket() {
    shutdown();
}
