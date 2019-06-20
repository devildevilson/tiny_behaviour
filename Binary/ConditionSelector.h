#ifndef CONDITIONAL_SELECTOR_H
#define CONDITIONAL_SELECTOR_H

#include <functional>
#include "../Binary.h"

namespace tb {
  class ConditionSelector : public Binary {
  public:
    ConditionSelector() : Binary() {}
    ~ConditionSelector() {}

    void setPredicate(const std::function<PREDICATE_SIGNATURE> &p) { predicate = p;}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      if (!predicate) return status::failure;
      
      status s;
      if (predicate(this, data)) s = hasFirstChild() ? nodes.first->update(data, runningPtr) : status::failure;
      else s = hasSecondChild() ? nodes.second->update(data, runningPtr) : status::failure;

      return s;
    }
  private:
    std::function<PREDICATE_SIGNATURE> predicate;
  };
}

#endif
