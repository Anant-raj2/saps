#include "Server.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace ServerLayer {
int Server::init() {
  int status;
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(NULL, "3490", &hints, &serverInfo)) != 0) {
    std::cout << "Could Not Recieve server information: "
              << gai_strerror(status) << '\n';
    exit(1);
  }
  return 0;
}

Server::Server() { init(); }

Server::~Server() { cleanup(); }

void Server::cleanup() { freeaddrinfo(serverInfo); }
} // namespace ServerLayer
//
