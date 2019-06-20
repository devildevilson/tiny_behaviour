#ifndef MEM_SELECTOR_H
#define MEM_SELECTOR_H

#include <unordered_map>
#ifdef TINY_BEHAVIOUR_MULTITHREADING
#include <mutex>
#endif

#include "../Compositor.h"

namespace tb {
  class MemSelector : public Compositor {
  public:
    MemSelector() : Compositor() {}
    ~MemSelector() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      if (!hasChildren()) return status::success;
      
      size_t firstIndex = 0;
#ifdef TINY_BEHAVIOUR_MULTITHREADING
      {
        std::unique_lock<std::mutex> lock(mutex);
#endif
      
      auto itr = memory.find(data);
      if (itr == memory.end()) {
        itr = memory.insert(std::make_pair(data, 0)).first;
      }
      
      firstIndex = itr->second;
      
#ifdef TINY_BEHAVIOUR_MULTITHREADING
      }
#endif
      
      status s;
      for (size_t i = firstIndex; i < children.size(); i++) {
        s = children[i]->update(data, runningPtr);
        
        if (s != status::failure) {
#ifdef TINY_BEHAVIOUR_MULTITHREADING
          std::unique_lock<std::mutex> lock(mutex);
          memory[data] = i;
#else 
          itr->second = i;
#endif
          return s;
        }
      }

#ifdef TINY_BEHAVIOUR_MULTITHREADING
      std::unique_lock<std::mutex> lock(mutex);
      memory[data] = 0;
#else 
      itr->second = 0;
#endif
      s = status::failure;
      return s;
    }
  protected:
#ifdef TINY_BEHAVIOUR_MULTITHREADING
    std::mutex mutex;
#endif
    std::unordered_map<void*, size_t> memory;
  };
}

#endif

// #ifdef TINY_BEHAVIOUR_STATIC_INIT
//   std::unordered_map<void*, size_t> MemSelector::memory;
// #endif
