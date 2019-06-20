#ifndef UNTIL_SUCCESS_H
#define UNTIL_SUCCESS_H

#include "../Decorator.h"

namespace tb {
  class UntilSuccess : public Decorator {
  public:
    UntilSuccess() : Decorator() {}
    ~UntilSuccess() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      status s = child->update(data, runningPtr);
      
      s = s != status::success ? status::running : s;
      
      return s;
    }
  };
}

#endif
