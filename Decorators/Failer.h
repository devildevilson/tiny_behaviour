#ifndef FAILER_H
#define FAILER_H

#include "../Decorator.h"

namespace tb {
  class Failer : public Decorator {
  public:
    Failer() : Decorator() {}
    ~Failer() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      child->update(data, runningPtr);
      status s = status::failure;
      return s;
    }
  };
}

#endif
