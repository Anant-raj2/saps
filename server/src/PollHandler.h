#pragma once
#include <sys/poll.h>
#include <vector>

class PollHandler {
public:
  static std::vector<struct pollfd> *pollList;
  static std::vector<struct pollfd> *CreatePollVector();
  static void AddPoll(int fd);
  static void DeletePoll(int index);
  static void CleanUp();
private:
  PollHandler();
};
