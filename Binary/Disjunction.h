#ifndef DISJUNCTION_H
#define DISJUNCTION_H

#include "../Binary.h"

namespace tb {
  class Disjunction : public Binary {
  public:
    Disjunction() : Binary() {}
    virtual ~Disjunction() {}

    status update(void* const& data = nullptr) override {
      auto firstStatus = hasFirstChild() ? nodes.first->update(data) : status::failure;
      auto secondStatus = hasSecondChild() ? nodes.second->update(data) : status::failure;

      if (firstStatus == status::failure || secondStatus == status::failure) s = status::failure;
      else s = status::success;
      
      return s;
    }
  };
}

#endif
