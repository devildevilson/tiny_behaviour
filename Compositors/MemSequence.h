#ifndef MEM_SEQUENCE_H
#define MEM_SEQUENCE_H

#include <unordered_map>
#ifdef TINY_BEHAVIOUR_MULTITHREADING
#include <mutex>
#endif

#include "../Compositor.h"

// TODO: is it possible to rewrite memory
namespace tb {
  class MemSequence : public Compositor {
  public:
    MemSequence() : Compositor() {}
    ~MemSequence() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      if (!hasChildren()) return status::success;
      status s;

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

      for (size_t i = firstIndex; i < children.size(); i++) {
        s = children[i]->update(data, runningPtr);
        
        if (s != status::success) {
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
      s = status::success;
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
//   std::unordered_map<void*, size_t> MemSequence::memory;
// #endif
