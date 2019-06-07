#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "../Compositor.h"

namespace tb {
  class Sequence : public Compositor {
  public:
    Sequence() : Compositor() {}
    ~Sequence() {}

    status update(void* const& data = nullptr) override {
      if (!hasChildren()) {
        index = 0;
        s = status::success;
        return s;
      }

      for (index = 0; index < children.size(); index++) {
        s = children[index]->update(data);

        if (s != status::success) return s;
      }

      s = status::success;
      return s;
    }
  };
}

#endif
