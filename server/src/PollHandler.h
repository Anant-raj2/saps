#pragma once
#include <sys/poll.h>
#include <vector>

class PollHandler {
public:
  static std::vector<struct pollfd> *pollList;
  static std::vector<struct pollfd> *CreatePollVector(int listenerFD);
  static void AddPoll(struct pollfd &client);
  static void DeletePoll(int index);
  static void CleanUp();
private:
  PollHandler();
};
