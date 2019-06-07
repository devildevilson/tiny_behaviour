#ifndef UNTIL_SUCCESS_H
#define UNTIL_SUCCESS_H

#include "../Decorator.h"

namespace tb {
  class UntilSuccess : public Decorator {
  public:
    UntilSuccess() : Decorator() {}
    ~UntilSuccess() {}

    status update(void* const& data = nullptr) override {
      s = child->update(data);
      
      s = s != status::success ? status::running : s;
      
      return s;
    }
  };
}

#endif
