#pragma once
#include <netdb.h>

namespace ServerLayer {
class Server {
private:
  // The file descriptor that will be connected to the server socket
  int serverFd;
  struct addrinfo *serverInfo;

private:
  // Initialize the socket and to a port
  int init();
  void cleanup();
  // Start Listening for connections
  // Accept a connection
  // Send to a connection
  // Recieve from a connection

public:
  Server(int& argc, char **argv[]);
  ~Server();
  //Run the Server
};
} // namespace ServerLayer
