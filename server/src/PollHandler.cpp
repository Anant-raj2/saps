#include "PollHandler.h"
#include <sys/poll.h>
#include <vector>

std::vector<struct pollfd> *PollHandler::CreatePollVector(int listenerFD) {
  pollList = new std::vector<struct pollfd>();
  pollList->push_back({.fd = listenerFD, .events = POLLIN});
  return pollList;
}

void PollHandler::AddPoll(struct pollfd &client) {
  pollList->push_back(client);
}

void PollHandler::DeletePoll(int index) {
  pollList->erase(pollList->begin() + index);
}

void PollHandler::CleanUp() { delete[] pollList; }

PollHandler::PollHandler() = default;
