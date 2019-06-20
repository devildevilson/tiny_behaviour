#ifndef CONDITION_H
#define CONDITION_H

#include <functional>
#include "../Decorator.h"

namespace tb {
  class Condition : public Decorator {
  public:
    Condition() : Decorator() {}
    ~Condition() {}

    void setPredicate(const std::function<PREDICATE_SIGNATURE> &p) { predicate = p; }

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      status s;
      if (predicate && predicate(this, data)) s = child->update(data, runningPtr);
      else s = status::failure;

      return s;
    }
  private:
    std::function<PREDICATE_SIGNATURE> predicate;
  };
}

#endif
