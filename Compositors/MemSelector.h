#ifndef MEM_SELECTOR_H
#define MEM_SELECTOR_H

#include <unordered_map>

#include "../Compositor.h"

namespace tb {
  class MemSelector : public Compositor {
  public:
    MemSelector() : Compositor() {}
    ~MemSelector() {}

    status update(void* const& data = nullptr) override {
      if (!hasChildren()) return status::success;
      
      auto itr = memory.find(data);
      if (itr == memory.end()) {
        itr = memory.insert(std::make_pair(data, 0)).first;
      }

      for (size_t i = itr->second; i < children.size(); i++) {
        index = i;

        s = children[i]->update(data);
        if (s != status::failure) {
          itr->second = i;
          return s;
        }
      }

      index = 0;
      itr->second = 0;
      s = status::failure;
      return s;
    }
  protected:
    static std::unordered_map<void*, size_t> memory;
  };
}

#endif

#ifdef TINY_BEHAVIOUR_STATIC_INIT
  std::unordered_map<void*, size_t> MemSelector::memory;
#endif
