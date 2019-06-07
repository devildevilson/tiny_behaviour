#ifndef IMPLICATION_H
#define IMPLICATION_H

#include "../Binary.h"

namespace tb {
  class Implication : public Binary {
  public:
    Implication() : Binary() {}
    ~Implication() {}

    status update(void* const& data = nullptr) override {
      auto firstStatus = hasFirstChild() ? nodes.first->update(data) : status::failure;
      auto secondStatus = hasSecondChild() ? nodes.second->update(data) : status::failure;

      if (!(firstStatus == status::success) || secondStatus == status::success) s = status::success;
      else s = status::failure;
      
      return s;
    }
  };
}

#endif
