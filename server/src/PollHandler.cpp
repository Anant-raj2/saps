#include "PollHandler.h"
#include <cstdlib>
#include <sys/poll.h>

struct pollfd *PollHandler::CreatePollVector(int size, int newCapacity) {
  struct pollfd *pollList = malloc(sizeof *pfds * fd_size);
  fd_count++;
  capacity = newCapacity;
  return pollList;
}

void PollHandler::AddFD(int fd) {
  if (fd_count == capacity) {
    capacity *= 2;
    pollList = (struct pollfd *)realloc(pollList, sizeof(*pollList) * 2);
  }
  pollList[fd_count].fd = fd;
  pollList[fd_count].events = POLLIN;

  fd_count++;
}

void PollHandler::CleanUp(){
  free(pollList);
}

PollHandler::PollHandler() = default;
