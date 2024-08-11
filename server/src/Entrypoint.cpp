#include "PollHandler.h"
#include <iostream>
#include <sys/poll.h>

struct pollfd *PollHandler::pollList = PollHandler::CreatePollVector(1, 3);
int PollHandler::capacity{};
int PollHandler::fd_count{};

int main() {
  // std::unique_ptr<ServerLayer::Server> server =
  //     std::make_unique<ServerLayer::Server>();
  // server->Start();
  //
  for (int i; i < PollHandler::fd_count; i++) {
    std::cout << PollHandler::pollList[i].fd << ' ';
  }
  PollHandler::AddFD(2);
  // std::cout << PollHandler::pollList[0].fd;
  for (int i; i < PollHandler::fd_count; i++) {
    std::cout << PollHandler::pollList[i].fd << ' ';
  }
}
