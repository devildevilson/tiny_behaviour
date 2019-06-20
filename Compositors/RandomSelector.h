#ifndef RANDOM_SELECTOR_H
#define RANDOM_SELECTOR_H

#include <unordered_map>
#include <random>
#include "../Compositor.h"

// TODO: rewrite
namespace tb {
  class RandomSelector : public Compositor {
  public:
    RandomSelector() : Compositor(), seed(1) {
      engine.seed(seed);
    }

    RandomSelector(const uint32_t &seed) : Compositor(), seed(seed) {
      engine.seed(seed);
    }

    ~RandomSelector() {}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      if (!hasChildren()) return status::success;
      
      uint32_t maxIndex = children.size()-1;
      d.param(std::uniform_int_distribution<unsigned int>::param_type(0, maxIndex));
      
      auto itr = memory.find(data);
      if (itr == memory.end()) {
        auto out = memory.insert(std::make_pair(data, 0));
        itr = out.first;
      }
      
      if (itr->second == UINT64_MAX) itr->second = d(engine);
      status s = children[itr->second]->update(data, runningPtr);
      
      if (s != status::running) itr->second = UINT64_MAX;

      return s;
    }
  protected:
    uint32_t seed;
    std::mt19937 engine;
    static std::uniform_int_distribution<uint32_t> d;
    static std::unordered_map<void*, size_t> memory;
  };
}

#endif

#ifdef TINY_BEHAVIOUR_STATIC_INIT
  std::uniform_int_distribution<uint32_t> RandomSelector::d;
  std::unordered_map<void*, size_t> RandomSelector::memory;
#endif
