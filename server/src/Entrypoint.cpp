#include "Server.h"
#include <memory>

int main(){
  std::unique_ptr<ServerLayer::Server> server = std::make_unique<ServerLayer::Server>();
}
