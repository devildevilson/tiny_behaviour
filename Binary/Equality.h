#ifndef EQUALITY_H
#define EQUALITY_H

#include "../Binary.h"

namespace tb {
  class Equality : public Binary {
  public:
    Equality() : Binary() {}
    ~Equality() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      auto firstStatus = hasFirstChild() ? nodes.first->update(data, runningPtr) : status::failure;
      if (firstStatus == status::running) return status::running;
      
      auto secondStatus = hasSecondChild() ? nodes.second->update(data, runningPtr) : status::failure;
      if (secondStatus == status::running) return status::running;

      status s = firstStatus == secondStatus ? status::success : status::failure;
      
      return s;
    }
  };
}

#endif
