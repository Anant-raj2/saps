#include "Server.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace ServerLayer {
Server::Server(int &argc, char **argv[]) {
  int status;
  struct addrinfo hints;

  if (argc != 2) {
    std::cout << "Arguments not provided" << '\n';
    exit(1);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(*argv[0], "3490", &hints, &serverInfo)) != 0) {
    std::cout << "Could Not Recieve server information: "
              << gai_strerror(status) << '\n';
    exit(1);
  }

  std::cout << serverInfo->ai_addr;
}

Server::~Server() { cleanup(); }

void Server::cleanup() { freeaddrinfo(serverInfo); }
} // namespace ServerLayer
//
