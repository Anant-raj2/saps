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

void Server::Write() {
  char writeBuffer[1024];
  int len = strlen(writeBuffer);
  char prefix[] = "[Server]: ";

  while (true) {
    std::cin >> writeBuffer;

    int bytes_sent = send(clientFd, writeBuffer, len, 0);
    if (bytes_sent < len) {
      std::cout << "Write packets were lost " << errno;
      break;
    }
    memset(&writeBuffer, 0, sizeof(writeBuffer));
  }
}

void Server::Recieve() {

  char readBuffer[1024];

  char prefix[] = "[Client]: ";

  int len = strlen(readBuffer);

  while (true) {
    int bytes_read = recv(clientFd, &readBuffer, len, 0);
    if (bytes_read == -1) {
      std::cout << "Read packets were lost " << errno;
      break;
    }
    std::cout << prefix << readBuffer << '\n';

    memset(&readBuffer, 0, sizeof(readBuffer));
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

  std::thread writeThread(&Server::Write, this);
  std::thread readThread(&Server::Recieve, this);

  writeThread.join();
  readThread.join();
}

void Server::cleanup() {
  close(serverFd);
  freeaddrinfo(serverInfo);
}
} // namespace ServerLayer
