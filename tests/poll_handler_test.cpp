#include "PollHandler.h"
#include "gtest/gtest.h"
#include <sys/poll.h>
#include <vector>

TEST(PollHandler, CreatePollList) {
  PollHandler::CreatePollVector(2);

  std::vector<struct pollfd> *poller = new std::vector<struct pollfd>();
  poller->push_back({.fd = 2, .events = POLLIN});

  EXPECT_EQ(PollHandler::pollList, poller);
}

TEST(PollHandler, AddPoll) {
  struct pollfd poll;
  poll.fd = 1;
  poll.events = POLLIN;
  PollHandler::AddPoll(poll);

  std::vector<struct pollfd> *poller = new std::vector<struct pollfd>();
  poller->push_back({.fd = 1, .events = POLLIN});

  EXPECT_EQ(PollHandler::pollList, poller);
}

TEST(PollHandler, DeletPoll) {
  PollHandler::DeletePoll(1);

  std::vector<struct pollfd> *poller = new std::vector<struct pollfd>();
  poller->push_back({.fd = 1, .events = POLLIN});

  EXPECT_EQ(PollHandler::pollList, poller);
}

TEST(PollHandler, CleanUp) {
  PollHandler::CleanUp();
  EXPECT_EQ(PollHandler::pollList, poller);
}
