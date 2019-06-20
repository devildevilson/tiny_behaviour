#ifndef UNTIL_FAIL_H
#define UNTIL_FAIL_H

#include "../Decorator.h"

namespace tb {
  class UntilFail : public Decorator {
  public:
    UntilFail() : Decorator() {}
    ~UntilFail() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      status s = child->update(data, runningPtr);
      
      s = s != status::failure ? status::running : s;
      
      return s;
    }
  };
}

#endif
