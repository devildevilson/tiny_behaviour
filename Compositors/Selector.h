#ifndef SELECTOR_H
#define SELECTOR_H

#include "../Compositor.h"

namespace tb {
  class Selector : public Compositor {
  public:
    Selector() : Compositor() {}
    ~Selector() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      status s;
      if (!hasChildren()) {
        s = status::success;
        return s;
      }

      for (size_t index = 0; index < children.size(); index++) {
        s = children[index]->update(data, runningPtr);

        if (s != status::failure) return s;
      }

      s = status::failure;
      return s;
    }
  };
}

#endif
