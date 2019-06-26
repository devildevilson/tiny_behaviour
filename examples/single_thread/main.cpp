#include <iostream>
#include "TinyBehavior.h"
#include "Leaf.h"

class ActionNode1 : public tb::Leaf {
public:
  ActionNode1(bool* enemyInSight) : enemyInSight(enemyInSight) {}
  ~ActionNode1() {}

  // main function
  tb::Node::status update(void* const& data = nullptr, tb::Node** runningPtr = nullptr) override {
    (void)runningPtr;

    if (*enemyInSight) return tb::Node::status::success;
    std::cout << "Im waiting!" << "\n";
    return tb::Node::status::failure;
  }

  std::string toString() const { return "ActionNode1"; }
  void print(const std::string &indent) const { std::cout << indent << toString() << "\n"; }
private:
  bool* enemyInSight;
};

int main(int argc, char const *argv[]) {
  bool sight = false;

  // all data would be local if we provide appropriate size to the builder
  const size_t treeSize = sizeof(tb::ParallelSequence) + sizeof(tb::Action) * 4 + sizeof(tb::Failer) + sizeof(tb::Condition) + sizeof(tb::Limiter) + sizeof(ActionNode1);
  tb::BehaviorTreeBuilder builder(treeSize);
  tb::BehaviorTree* tree;
  tree = builder.parallel()
                  .action([] (tb::Node* const& currentNode, void* const& data = nullptr) {
                    std::cout << '\n';
                    std::cout << "Im an action in parallel node!" << "\n";
                    return tb::Node::status::success;
                  })
                  .failer()
                    .action([] (tb::Node* const& currentNode, void* const& data = nullptr) {
                      std::cout << "Im another action in parallel node!" << "\n";
                      return tb::Node::status::success;
                    })
                  .condition([] (tb::Node* const& currentNode, void* const& data = nullptr) {
                    std::cout << "Some condition before action!" << "\n";
                    return true;
                  })
                    .action([] (tb::Node* const& currentNode, void* const& data = nullptr) {
                      std::cout << "Im an action in the condition!" << "\n";
                      return tb::Node::status::running;
                    })
                  .limiter(5)
                    .action([] (tb::Node* const& currentNode, void* const& data = nullptr) {
                      std::cout << "Im an action that succeds or run only 5 times" << "\n";
                      return tb::Node::status::success;
                    })
                  .leaf<ActionNode1>(nullptr, &sight) // note that method 'leaf' has different arguments
                  // ... and others type of nodes, you can read about them below
                 .end() // parallel needs to be end
              .build();
  tree->print("  "); // for debugging

  std::cout << '\n';

  tb::Node* running = nullptr; // for optimisation purpose you can take currently running action node
  for (size_t i = 0; i < 6; ++i) {
    tree->update(nullptr, &running); // use
  }

  if (running != nullptr) running->update();

  // ...
  delete tree; // dont forget to delete tree, it deletes all nodes in that tree too

  return 0;
}
