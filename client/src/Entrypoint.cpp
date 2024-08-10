#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
int main(){
  struct addrinfo *res, hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
}
