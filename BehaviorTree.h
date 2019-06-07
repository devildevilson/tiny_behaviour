#ifndef BEHAVIOR_TREE_H
#define BEHAVIOR_TREE_H

#include <iostream>
#include <vector>

#include "Node.h"

namespace tb {
  //void printRecursive(const std::string &indent, Node* const& node);

  // deprecated
  class BehaviorTree : public Node {
      friend class BehaviorTreeBuilder;
  public:
    BehaviorTree() : Node(TINY_BEHAVIOR_TREE_NODE_TYPE) {}
    ~BehaviorTree() {
      for (auto &node : nodes) {
        delete node;
      }
    }

    status update(void* const& data = nullptr) override {
      if (root == nullptr) {
        std::cout << "This tree is invalid and must be deleted!" << "\n";
        exit(-1);
      }

      return root->tick();
    }
    
    std::string toString() const override { return "BehaviorTree"; }
    void print(const std::string &indent) const override {
      if (root != nullptr) root->print("  ");
    }
    
    constexpr static uint32_t getType() { return TINY_BEHAVIOR_TREE_NODE_TYPE; }
  private:
    Node* root = nullptr;
    std::vector<Node*> nodes;
  };
}

#endif
