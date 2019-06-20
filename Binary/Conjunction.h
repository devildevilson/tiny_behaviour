#ifndef CONJUNCTION_H
#define CONJUNCTION_H

#include "../Binary.h"

namespace tb {
  class Conjunction : public Binary {
  public:
    Conjunction() : Binary() {}
    ~Conjunction() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      auto firstStatus = hasFirstChild() ? nodes.first->update(data, runningPtr) : status::failure;
      if (firstStatus == status::running) return status::running;
      
      auto secondStatus = hasSecondChild() ? nodes.second->update(data, runningPtr) : status::failure;
      if (secondStatus == status::running) return status::running;
      
      status s;
      if (firstStatus == status::success && secondStatus == status::success) s = status::success;
      else s = status::failure;
      
      return s;
    }
  };
}

#endif
