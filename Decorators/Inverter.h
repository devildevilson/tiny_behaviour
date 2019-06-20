#ifndef INVERTER_H
#define INVERTER_H

#include "../Decorator.h"

namespace tb {
  class Inverter : public Decorator {
  public:
    Inverter() : Decorator() {}
    ~Inverter() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      status s = child->update(data, runningPtr);
      if (s == status::running) return s;

      s = s == status::success ? status::failure : (s == status::failure ? status::success : s);

      return s;
    }
  };
}

#endif
