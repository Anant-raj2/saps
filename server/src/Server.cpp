#include "Server.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace ServerLayer {
Server::Server() {
  int status;
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(nullptr, "3490", &hints, &serverInfo)) != 0) {
    std::cout << "Could Not Recieve server information: "
              << gai_strerror(status) << '\n';
    exit(status);
  }

  serverFd = socket(serverInfo->ai_family, serverInfo->ai_socktype,
                    serverInfo->ai_protocol);

  if (serverFd == -1) {
    std::cout << "Could not initialize server file descriptor! " << errno
              << '\n';
    exit(serverFd);
  }

  status = bind(serverFd, serverInfo->ai_addr, serverInfo->ai_addrlen);
  if (status == -1) {
    std::cout << "Could not bind server file descriptor! " << errno << '\n';
    exit(status);
  }

  std::cout << "Server bound" << '\n';
}

Server::~Server() { cleanup(); }

void Server::Listen() {
  int status = listen(serverFd, 10);
  if (status != 0) {
    std::cout << "Server Couldn't Start listening " << errno << '\n';
    exit(status);
  }


}

void Server::cleanup() { freeaddrinfo(serverInfo); }
} // namespace ServerLayer
//
