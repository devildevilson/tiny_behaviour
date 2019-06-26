#ifndef DECORATOR_H
#define DECORATOR_H

#include "Node.h"

namespace tb {
  // simple decorator, see Decorator pattern for more information
  class Decorator : public Node {
    friend class BehaviorTreeBuilder;
  public:
    Decorator() : Node(TINY_DECORATOR_NODE_TYPE) {}
    virtual ~Decorator() {}

    void setChild(Node* node) { child = node; }
    bool hasChild() const { return child != nullptr; }

    std::string toString() const override { return "Decorator"; }
    void print(const std::string &indent) const override {
      std::cout << indent << toString() << "\n";
      if (child != nullptr) child->print(indent + indent);
    }

    constexpr static uint32_t getType() { return TINY_DECORATOR_NODE_TYPE; }
  protected:
    Node* child = nullptr;
  };
}

#endif // DECORATOR_H
