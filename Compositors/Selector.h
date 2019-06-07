#ifndef SELECTOR_H
#define SELECTOR_H

#include "../Compositor.h"

namespace tb {
  class Selector : public Compositor {
  public:
    Selector() : Compositor() {}
    ~Selector() {}

    status update(void* const& data = nullptr) override {
      if (!hasChildren()) {
        index = 0;
        s = status::success;
        return s;
      }

      for (index = 0; index < children.size(); index++) {
        s = children[index]->update(data);

        if (s != status::failure) return s;
      }

      s = status::failure;
      return s;
    }
  };
}

#endif
