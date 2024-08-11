#include "PollHandler.h"
#include <sys/poll.h>
#include <vector>

std::vector<struct pollfd> *PollHandler::CreatePollVector() {
  pollList = new std::vector<struct pollfd>();
  return pollList;
}

void PollHandler::AddPoll(int fd) {
  pollList->push_back({.fd = fd, .events = POLLIN});
}

void PollHandler::DeletePoll(int index) {
  pollList->erase(pollList->begin() + index);
}

void PollHandler::CleanUp() { delete[] pollList; }

PollHandler::PollHandler() = default;
