#ifndef REPEATER_H
#define REPEATER_H

#include "../Decorator.h"

namespace tb {
  class Repeater : public Decorator {
  public:
    Repeater(const uint32_t &limit = 0) : Decorator(), limit(limit), counter(0) {}
    ~Repeater() {}

    status update(void* const& data = nullptr) override {
      s = child->update(data);
      counter++;
      
      if (limit > 0 && counter < limit) {
        s = status::running;
        return s;
      }
      
      counter = 0;
      return s;
    }
  protected:
    uint32_t limit;
    uint32_t counter;
  };
}

#endif
