#include "Server.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

namespace ServerLayer {
Server::Server() {
  int status;
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo("127.0.0.1", "3000", &hints, &serverInfo)) != 0) {
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

void Server::Write(){
  char buffer[1024];
  int len = strlen(buffer);
  char prefix[] = "[Server]: ";

  while (true) {
    std::cin >> buffer;

    int bytes_sent = send(clientFd, strcat(prefix, buffer), len+sizeof(prefix), 0);
    if (bytes_sent < len) {
      std::cout << "packets were lost " << errno;
      break;
    }
    memset(&buffer, 0, sizeof(buffer));
  }
}

void Server::Recieve(){
  char buffer[1024];
  int len = strlen(buffer);
  char prefix[] = "[Client]: ";

  while (true) {
    std::cin >> buffer;

    int bytes_read = recv(clientFd, strcat(prefix, buffer), len+sizeof(prefix), 0);

    if (bytes_read == -1) {
      std::cout << "packets were lost " << errno;
      break;
    }

    memset(&buffer, 0, sizeof(buffer));
  }
}

void Server::Start() {
  socklen_t addrSize = sizeof(clientInfo);

  int status = listen(serverFd, 5);
  if (status != 0) {
    std::cout << "Server Couldn't listening " << errno << '\n';
    exit(status);
  }
  clientFd = accept(serverFd, (struct sockaddr *)&clientInfo, &addrSize);

  std::thread writeThread(&Server::Write);
  std::thread readThread(&Server::Recieve);

  writeThread.join();
  readThread.join();
}

void Server::cleanup() {
  close(serverFd);
  freeaddrinfo(serverInfo);
}
} // namespace ServerLayer
//
//
