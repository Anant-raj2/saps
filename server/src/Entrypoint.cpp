#include "PollHandler.h"
#include <iostream>
#include <sys/poll.h>
#include <vector>

std::vector<struct pollfd> *PollHandler::pollList =
    PollHandler::CreatePollVector();

int main() {
  // std::unique_ptr<ServerLayer::Server> server =
  //     std::make_unique<ServerLayer::Server>();
  // server->Start();
  //
  PollHandler::AddPoll(2);
  PollHandler::AddPoll(3);
  PollHandler::AddPoll(4);
  PollHandler::AddPoll(5);
  PollHandler::AddPoll(6);
  for (struct pollfd element : *PollHandler::pollList) {
    std::cout << element.fd << ' ';
  }
  std::cout << '\n';
  PollHandler::DeletePoll(3);
  for (struct pollfd element : *PollHandler::pollList) {
    std::cout << element.fd << ' ';
  }
  std::cout << '\n';
  PollHandler::CleanUp();
  for (struct pollfd element : *PollHandler::pollList) {
    std::cout << element.fd << ' ';
  }
}
