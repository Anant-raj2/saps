#include "Server.h"
#include "PollHandler.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <poll.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

#define IPADDR "127.0.0.1"
#define PORT "3000"
#define BUFSIZE 1024

namespace ServerLayer {

Server::Server() { create_listener_fd(); }

// Sets the serverFd to the accquired fd
int Server::create_listener_fd() {
  struct addrinfo *serverInfo, hints, *index;

  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_INET;
  hints.ai_socktype = AF_INET;

  int yes{};

  if ((yes = getaddrinfo(IPADDR, PORT, &hints, &serverInfo)) != 0) {
    std::cout << "Could not initialize Server Info: " << gai_strerror(yes)
              << '\n';
    return -1;
  }

  for (index = serverInfo; index != nullptr; index = serverInfo->ai_next) {
    if ((serverFd = socket(serverInfo->ai_family, serverInfo->ai_socktype,
                           serverInfo->ai_protocol)) < 0) {
      std::cout << "Could not initialize the file descriptor: " << errno
                << '\n';
      return -1;
    }
    if ((bind(serverFd, serverInfo->ai_addr, serverInfo->ai_addrlen)) < 0) {
      std::cout << "Could not bind to " << IPADDR << "::" << PORT << " "
                << errno << '\n';
      return -1;
    }
  }

  freeaddrinfo(serverInfo);

  if (index != nullptr) {
    std::cout << "Could not bind to " << IPADDR << "::" << PORT << " " << errno
              << '\n';
  }

  if (listen(serverFd, 10)) {
    return -1;
  }
  return 0;
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
