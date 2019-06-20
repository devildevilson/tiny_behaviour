#ifndef ACTION_H
#define ACTION_H

#include <functional>
#include "Node.h"

namespace tb {
  // userdefined action
  class Action : public Node {
  public:
    Action() : Node(TINY_ACTION_NODE_TYPE) {}
    ~Action() {}

    void setAction(const std::function<status(Node* const& currentNode, void* const& data)> &a) { action = std::move(a);}

    status update(void* const& data = nullptr, Node** runningPtr = nullptr) override {
      status s = status::failure;
      if (action) s = action(this, data);
      if (s == status::running && runningPtr != nullptr) *runningPtr = this;
      return s;
    }
    
    std::string toString() const override { return "Action"; }
    void print(const std::string &indent) const override {
      std::cout << indent + toString() << "\n";
    }
    
    constexpr static uint32_t getType() { return TINY_ACTION_NODE_TYPE; }
  private:
    std::function<status(Node* const& currentNode, void* const& data)> action;
  }; // Action
} // TinyBehavior

#endif // ACTION_H
