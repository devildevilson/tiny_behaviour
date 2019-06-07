#ifndef CONJUNCTION_H
#define CONJUNCTION_H

#include "../Binary.h"

namespace tb {
  class Conjunction : public Binary {
  public:
    Conjunction() : Binary() {}
    ~Conjunction() {}

    status update(void* const& data = nullptr) override {
      auto firstStatus = hasFirstChild() ? nodes.first->update(data) : status::failure;
      auto secondStatus = hasSecondChild() ? nodes.second->update(data) : status::failure;

      if (firstStatus == status::success && secondStatus == status::success) s = status::success;
      else s = status::failure;
      
      return s;
    }
  };
}

#endif
