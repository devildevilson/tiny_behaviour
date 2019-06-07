#ifndef NODE_H
#define NODE_H

#include <cstdint>
#include <iostream>
#include <string>
#ifndef DEBUG_OUT
#define DEBUG_OUT(child) if (child->is<Compositor>()) std::cout << "Composite" << "\n"; \
                         if (child->is<Decorator>()) std::cout << "Decorator" << "\n"; \
                         if (child->is<Binary>()) std::cout << "Binary" << "\n"; \
                         if (child->is<Action>()) std::cout << "Action" << "\n";
#endif

#define PREDICATE_SIGNATURE bool(Node* const& currentNode, void* const& data)

#define TINY_DECORATOR_NODE_TYPE 0
#define TINY_COMPOSITOR_NODE_TYPE 1
#define TINY_BINARY_NODE_TYPE 2
#define TINY_ACTION_NODE_TYPE 3
#define TINY_BEHAVIOR_TREE_NODE_TYPE 4

namespace tb {
  class Node {
    friend class BehaviorTreeBuilder;
  public:
    enum class status {
      init,
      success,
      failure,
      running,
    };

    Node(const uint32_t &type) : type(type) {}
    virtual ~Node() {}

    virtual std::string toString() const = 0;
    virtual void print(const std::string &indent) const = 0;
    
    virtual status update(void* const& data = nullptr) = 0;
    
    // overrided only in BehaviorTree
    virtual void setRunning(Node* node) {
      tree->setRunning(node);
    }

    bool isSuccess() const { return s == status::success; }
    bool isFailure() const { return s == status::failure; }
    bool isRunning() const { return s == status::running; }
    void reset() { s = status::init; }
    
    Node* getParent() const { return parent; }
    Node* getRoot() const { return tree; }
    uint32_t getContainerIndex() const { return cIndex; }
    
    template<typename NodeClass> bool is() const { return getNodeType() == NodeClass::getType(); }
    uint32_t getNodeType() const { return type; }
  protected:
    uint32_t cIndex = 0;
    Node* tree = nullptr;
    Node* parent = nullptr;
    status s = status::init;
    uint32_t type = UINT32_MAX;
  };
}

#endif // NODE_H
