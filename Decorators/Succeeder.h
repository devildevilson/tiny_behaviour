#ifndef SUCCEEDER_H
#define SUCCEEDER_H

#include "../Decorator.h"

namespace tb {
  class Succeeder : public Decorator {
  public:
    Succeeder() : Decorator() {}
    ~Succeeder() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      child->update(data, runningPtr);
      status s = status::success;
      return s;
    }
  };
}

#endif
