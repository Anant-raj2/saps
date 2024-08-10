#pragma once
#include <netdb.h>
#include <sys/socket.h>

namespace ServerLayer {
class Server {
private:
  // The file descriptor that will be connected to the server socket
  int serverFd;
  int clientFd;
  struct addrinfo *serverInfo;
  struct sockaddr_storage clientInfo;

private:
  // Initialize the socket and to a port
  int init();
  void cleanup();
  // Start Listening for connections
  // Accept a connection
  // Send to a connection
  // Recieve from a connection

public:
  Server();
  ~Server();
  // Run the Server
  void Start();
  void Recieve();
  void Write();
};
} // namespace ServerLayer
