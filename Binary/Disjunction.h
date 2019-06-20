#ifndef DISJUNCTION_H
#define DISJUNCTION_H

#include "../Binary.h"

namespace tb {
  class Disjunction : public Binary {
  public:
    Disjunction() : Binary() {}
    virtual ~Disjunction() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      auto firstStatus = hasFirstChild() ? nodes.first->update(data, runningPtr) : status::failure;
      if (firstStatus == status::running) return status::running;
      
      auto secondStatus = hasSecondChild() ? nodes.second->update(data, runningPtr) : status::failure;
      if (secondStatus == status::running) return status::running;

      status s;
      if (firstStatus == status::failure || secondStatus == status::failure) s = status::failure;
      else s = status::success;
      
      return s;
    }
  };
}

#endif
