#ifndef MEM_SEQUENCE_H
#define MEM_SEQUENCE_H

#include <unordered_map>

#include "../Compositor.h"

// TODO: is it possible to rewrite memory
namespace tb {
  class MemSequence : public Compositor {
  public:
    MemSequence() : Compositor() {}
    ~MemSequence() {}

    status update(void* const& data = nullptr) override {
      if (!hasChildren()) return status::success;

      auto itr = memory.find(data);
      if (itr == memory.end()) {
        itr = memory.insert(std::make_pair(data, 0)).first;
      }

      for (size_t i = itr->second; i < children.size(); i++) {
        index = i;

        s = children[i]->update(data);
        if (s != status::success) {
          itr->second = i;
          return s;
        }
      }

      index = 0;
      itr->second = 0;
      s = status::success;
      return s;
    }
  protected:
    static std::unordered_map<void*, size_t> memory;
  };
}

#endif

#ifdef TINY_BEHAVIOUR_STATIC_INIT
  std::unordered_map<void*, size_t> MemSequence::memory;
#endif
