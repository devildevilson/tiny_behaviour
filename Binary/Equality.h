#ifndef EQUALITY_H
#define EQUALITY_H

#include "../Binary.h"

namespace tb {
  class Equality : public Binary {
  public:
    Equality() : Binary() {}
    ~Equality() {}

    status update(void* const& data = nullptr) override {
      auto firstStatus = hasFirstChild() ? nodes.first->update(data) : status::failure;
      auto secondStatus = hasSecondChild() ? nodes.second->update(data) : status::failure;

      s = firstStatus == secondStatus ? status::success : status::failure;
      
      return s;
    }
  };
}

#endif
