#include "Server.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <poll.h>
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
  struct addrinfo hints, *serverInfo, *index;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(IPADDR, PORT, &hints, &serverInfo)) != 0) {
    std::cout << "Could Not Recieve server information: "
              << gai_strerror(status) << '\n';
    exit(status);
  }

  for (index = serverInfo; index != nullptr; index = index->ai_next) {
    if ((serverFd = socket(index->ai_family, index->ai_socktype,
                           serverInfo->ai_protocol)) != 0) {
      std::cout << "Could not initialize server file descriptor! " << errno
                << '\n';
      exit(1);
    }

    if ((bind(serverFd, index->ai_addr, index->ai_addrlen)) != 0) {
      std::cout << "Could not bind server file descriptor! " << errno << '\n';
      exit(1);
    }
  }
  freeaddrinfo(serverInfo);
}

Server::~Server() { cleanup(); }

void Server::Write() {
  std::cout << "Started Write thread" << '\n';

  char writeBuffer[BUFSIZE];
  int len = strlen(writeBuffer);
  char prefix[] = "[Server]: ";

  while (true) {
    std::cout << "Writing..." << '\n';
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
  std::cout << "Started Read thread" << '\n';

  char readBuffer[BUFSIZE];

  char prefix[] = "[Client]: ";

  int len = strlen(readBuffer);

  while (true) {
    std::cout << "Reading..." << '\n';
    int bytes_read = recv(clientFd, &readBuffer, len, 0);
    if (bytes_read == -1) {
      std::cout << "Read packets were lost " << errno;
      break;
    }
    std::cout << prefix << readBuffer << '\n';

    memset(&readBuffer, 0, sizeof(readBuffer));
  }
}

// Create the serverFd
// Add the serverFd to the pdfsList
// Realloc the pdfsList if neccassary
// create the add (Realloc), delete(Realloc) pdfsList utility functions
// Start the polling
// Check to see if anyone is able to work
// If it is the listener that is ready to read then
// If it is the listener, then go ahead and add a new clientFd and add it to the
// poller
// If it isn't the listener then send the data that is ready to send to the
// listener When if send the data we have to check for errors to see if the data
// was sent properly if the data is bad and couldn't send then delete the
// clientFd and delete it from the poll list
// if the data is good than broadcast it to all of the connections that are in
// the poll list excpet the listener and ourselves

void Server::Start() {
  socklen_t addrSize = sizeof(clientInfo);

  if (listen(serverFd, 5) == 0) {
    std::cout << "Listening on ip: " << IPADDR << '\n';
  } else {
    std::cout << "Failed to listen...";
    exit(1);
  }

  clientFd = accept(serverFd, (struct sockaddr *)&clientInfo, &addrSize);

  std::thread writeThread(&Server::Write, this);
  std::thread readThread(&Server::Recieve, this);

  writeThread.join();
  readThread.join();
}

void Server::cleanup() { close(serverFd); }
} // namespace ServerLayer
