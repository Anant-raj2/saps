#include "PollHandler.h"
#include <sys/poll.h>

struct pollfd *PollHandler::CreatePollVector(int size) {
  struct pollfd *pollList = new struct pollfd[size];
  return pollList;
}

void PollHandler::AddFD(int fd) {
  if (size == capacity) {
    capacity *= 2;
    struct pollfd *temp = new struct pollfd[capacity];
    for (int i = 0; i < size; i++) {
      pollList[i] = temp[i];
    }
    delete[] pollList;

    pollList = temp;

    pollList[size] = {.fd = fd, .events = POLLIN};

    fd_count++;
    size++;
    return;
  }
  pollList[size] = {.fd = fd, .events = POLLIN};
  fd_count++;
  size++;
  return;
}

PollHandler::PollHandler() = default;
