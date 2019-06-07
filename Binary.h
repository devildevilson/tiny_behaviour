#ifndef BINARY_H
#define BINARY_H

#include <utility>
#include "Node.h"

namespace tb {
  // compositor that holds only two elements
  class Binary : public Node {
    friend class BehaviorTreeBuilder;
  public:
    Binary() : Node(TINY_BINARY_NODE_TYPE) { nodes = std::make_pair(nullptr, nullptr); }
    virtual ~Binary() {}

    void setFirstChild(Node* node) { nodes.first = node; }
    void setSecondChild(Node* node) { nodes.second = node; }
    void setChild(const uint8_t &index, Node* node) {
      if (index == 0) nodes.first = node;
      else nodes.second = node;
    }
    bool hasFirstChild() const { return nodes.first != nullptr; }
    bool hasSecondChild() const { return nodes.second != nullptr; }
    
    std::string toString() const override { return "Binary"; }
    void print(const std::string &indent) const override {
      std::cout << indent + toString() << "\n";
      if (nodes.first != nullptr) nodes.first->print(indent + indent);
      if (nodes.second != nullptr) nodes.second->print(indent + indent);
    }
    
    constexpr static uint32_t getType() { return TINY_BINARY_NODE_TYPE; }
  protected:
    std::pair<Node*, Node*> nodes;
  };
}

#endif
