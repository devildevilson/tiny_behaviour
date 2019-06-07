#ifndef PARALLEL_SEQUENCE_H
#define PARALLEL_SEQUENCE_H

#include "../Compositor.h"

namespace tb {
  class ParallelSequence : public Compositor {
  public:
    ParallelSequence(const uint32_t &minSuccess = 0, const uint32_t &minFail = 0) : Compositor(), minSuccess(minSuccess), minFail(minFail) {}
    ~ParallelSequence() {}

    status update(void* const& data = nullptr) override {
      uint32_t minimumSuccess = minSuccess;
      uint32_t minimumFail = minFail;

      if (minSuccess == 0) minimumSuccess = children.size();
      if (minFail == 0) minimumFail = children.size();

      uint32_t totalSuccess = 0;
      uint32_t totalFail = 0;

      for (auto &child : children) {
        const auto &status = child->update(data);
        if (status == status::success) totalSuccess++;
        if (status == status::failure) totalFail++;
      }

      if (totalSuccess >= minimumSuccess) s = status::success;
      else if (totalFail >= minimumFail) s = status::failure;
      else s = status::running;

      return s;
    }
  private:
    uint32_t minSuccess;
    uint32_t minFail;
  };
}

#endif // PARALLEL_SEQUENCE_H
