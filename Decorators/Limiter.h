#ifndef LIMITER_H
#define LIMITER_H

#include "../Decorator.h"

namespace tb {
  class Limiter : public Decorator {
  public:
    Limiter(const uint32_t &limit = 0) : Decorator(), limit(limit), counter(0) {}
    ~Limiter() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      status s = child->update(data, runningPtr);
      ++counter;
      if (limit > 0 && counter < limit) return s;
      
      s = status::failure;
      return s;
    }
  protected:
    uint32_t limit;
    uint32_t counter;
  };
}

#endif
