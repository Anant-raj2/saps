#pragma once
#include <sys/poll.h>

class PollHandler {
public:
  static struct pollfd *pollList;
  static int fd_count;
  static int capacity;

  static struct pollfd *CreatePollVector(int size, int newCapacity);

  static void AddFD(int fd);
  static void DeletePoll(int index);
  static void CleanUp();

private:
  PollHandler();
};
