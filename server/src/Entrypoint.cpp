#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

int main(int argc, char *argv[]) {
  int status;
  struct addrinfo hints;
  struct addrinfo *serverInfo;

  if (argc != 2) {
    std::cout << "Arguments not provided" << '\n';
    exit(1);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(argv[1], nullptr, &hints, &serverInfo)) != 0) {
    std::cout << "Could Not Recieve server information: "
              << gai_strerror(status) << '\n';
    exit(1);
  }

  std::cout << "IP addresses for: " << argv[1] << '\n';

  struct sockaddr_in *ipv4 = (struct sockaddr_in *)serverInfo->ai_addr;
  void* addr = &(ipv4->sin_addr);
  char ipstr[INET6_ADDRSTRLEN];
  inet_ntop(serverInfo->ai_family, addr, ipstr, sizeof(ipstr));
  std::cout << ipstr;
}
#include <iostream>
