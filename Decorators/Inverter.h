#ifndef INVERTER_H
#define INVERTER_H

#include "../Decorator.h"

namespace tb {
  class Inverter : public Decorator {
  public:
    Inverter() : Decorator() {}
    ~Inverter() {}

    status update(void* const& data = nullptr) override {
      s = child->update(data);

      s = s == status::success ? status::failure : (s == status::failure ? status::success : s);

      return s;
    }
  };
}

#endif
