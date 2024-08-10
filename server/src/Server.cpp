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

#define IPADDR "127.0.0.1"
#define PORT "3000"
#define BUFSIZE 1024

namespace ServerLayer {
Server::Server() {
  int status;
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(IPADDR, PORT, &hints, &serverInfo)) != 0) {
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
  std::cout << "Started Write thread" << '\n';

  char writeBuffer[BUFSIZE];
  int len = strlen(writeBuffer);
  char prefix[] = "[Server]: ";

  while (true) {
    std::cout << "Started Write thread" << '\n';
    std::cin >> writeBuffer;

    int bytes_sent = send(clientFd, writeBuffer, len, 0);
    if (bytes_sent < len) {
      std::cout << "Write packets were lost " << errno;
      break;
    }
    memset(&writeBuffer, 0, sizeof(writeBuffer));
  }
}

void serve_connection(int sockfd) {
  // Clients attempting to connect and send data will succeed even before the
  // connection is accept()-ed by the server. Therefore, to better simulate
  // blocking of other clients while one is being served, do this "ack" from the
  // server which the client expects to see before proceeding.
  if (send(sockfd, "*", 1, 0) < 1) {
    perror_die("send");
  }

  ProcessingState state = WAIT_FOR_MSG;

  while (1) {
    uint8_t buf[1024];
    int len = recv(sockfd, buf, sizeof buf, 0);
    if (len < 0) {
      perror_die("recv");
    } else if (len == 0) {
      break;
    }

    for (int i = 0; i < len; ++i) {
      switch (state) {
      case WAIT_FOR_MSG:
        if (buf[i] == '^') {
          state = IN_MSG;
        }
        break;
      case IN_MSG:
        if (buf[i] == '$') {
          state = WAIT_FOR_MSG;
        } else {
          buf[i] += 1;
          if (send(sockfd, &buf[i], 1, 0) < 1) {
            perror("send error");
            close(sockfd);
            return;
          }
        }
        break;
      }
    }
  }

  close(sockfd);
}

void Server::Recieve() {

  char readBuffer[BUFSIZE];

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
  // std::thread readThread(&Server::Recieve, this);
  //
  writeThread.join();
  // readThread.join();
}

void Server::cleanup() {
  close(serverFd);
  freeaddrinfo(serverInfo);
}
} // namespace ServerLayer
