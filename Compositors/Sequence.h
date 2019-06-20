#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "../Compositor.h"

namespace tb {
  class Sequence : public Compositor {
  public:
    Sequence() : Compositor() {}
    ~Sequence() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      status s;
      if (!hasChildren()) {
        s = status::success;
        return s;
      }

      for (size_t index = 0; index < children.size(); index++) {
        s = children[index]->update(data, runningPtr);

        if (s != status::success) return s;
      }

      s = status::success;
      return s;
    }
  };
}

#endif
