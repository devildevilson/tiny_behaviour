#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <vector>
#include "Node.h"

namespace tb {
  // simple composite, see Composite pattern for more information
  class Compositor : public Node {
    friend class BehaviorTreeBuilder;
  public:
    Compositor() : Node(TINY_COMPOSITOR_NODE_TYPE) {}
    virtual ~Compositor() {}        

    void addChild(Node* child) { children.push_back(child); }
    bool hasChildren() const { return !children.empty(); }
//     int getIndex() const { return index; }
    
    std::string toString() const override { return "Compositor"; }
    void print(const std::string &indent) const override {
        std::cout << indent + toString() << "\n";
        for (const auto& child : children) child->print(indent + indent);
    }
    
    constexpr static uint32_t getType() { return TINY_COMPOSITOR_NODE_TYPE; }
  protected:
    std::vector<Node*> children;
//     uint32_t index = 0;
  };
}

#endif
