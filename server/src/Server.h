#pragma once
#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>

namespace ServerLayer {
class Server {
private:
  // The file descriptor that will be connected to the server socket
  int serverFd;
  int clientFd;
  bool isRunning;
  struct sockaddr_storage clientInfo;
  struct pollfd pfds[3];

private:
  // Initialize the socket and to a port
  int init();
  void cleanup();
  int create_listener_fd();
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
