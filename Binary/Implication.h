#ifndef IMPLICATION_H
#define IMPLICATION_H

#include "../Binary.h"

namespace tb {
  class Implication : public Binary {
  public:
    Implication() : Binary() {}
    ~Implication() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      auto firstStatus = hasFirstChild() ? nodes.first->update(data, runningPtr) : status::failure;
      if (firstStatus == status::running) return status::running;
      
      auto secondStatus = hasSecondChild() ? nodes.second->update(data, runningPtr) : status::failure;
      if (secondStatus == status::running) return status::running;
      
      status s;
      if (!(firstStatus == status::success) || secondStatus == status::success) s = status::success;
      else s = status::failure;
      
      return s;
    }
  };
}

#endif
