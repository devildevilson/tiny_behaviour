#ifndef UNTIL_FAIL_H
#define UNTIL_FAIL_H

#include "../Decorator.h"

namespace tb {
  class UntilFail : public Decorator {
  public:
    UntilFail() : Decorator() {}
    ~UntilFail() {}

    status update(void* const& data = nullptr) override {
      s = child->update(data);
      
      s = s != status::failure ? status::running : s;
      
      return s;
    }
  };
}

#endif
