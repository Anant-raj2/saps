#include "PollHandler.h"
#include <iostream>
#include <sys/poll.h>

struct pollfd *PollHandler::pollList = PollHandler::CreatePollVector(5);
int PollHandler::capacity{};
int PollHandler::size{};
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
  for (int i; i < PollHandler::fd_count; i++) {
    std::cout << PollHandler::pollList[i].fd << ' ';
  }
}
