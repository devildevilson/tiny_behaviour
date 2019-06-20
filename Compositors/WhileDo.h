#ifndef WHILE_DO_H
#define WHILE_DO_H

#include <functional>

#include "../Compositor.h"

namespace tb {
  class WhileDo : public Compositor {
  public:
    WhileDo() : Compositor() {}
    ~WhileDo() {}

    void setPredicate(const std::function<PREDICATE_SIGNATURE> &f) { predicate = f; }

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      status s;
      if (predicate && predicate(this, data)) {
          for (auto &child : children) child->update(data, runningPtr);
          s = status::running;
      } else s = status::failure;

      return s;
    }
  private:
    std::function<PREDICATE_SIGNATURE> predicate;
  };
}

#endif
