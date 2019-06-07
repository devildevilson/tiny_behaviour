#ifndef LEAF_H
#define LEAF_H

#include "Node.h"

namespace tb {
  // use this to create an action node
  class Leaf : public Node {
  public:
    Leaf() : Node(TINY_ACTION_NODE_TYPE) {}
    virtual ~Leaf() {}

    std::string toString() const override { return "Action"; }
    void print(const std::string &indent) const override { std::cout << indent + toString() << "\n"; }
    
    constexpr static uint32_t getType() { return TINY_ACTION_NODE_TYPE; }
  }; // Leaf
} // TinyBehavior

#endif // LEAF_H
