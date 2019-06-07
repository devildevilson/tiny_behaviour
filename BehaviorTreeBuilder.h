#ifndef BEHAVIOR_TREE_BUILDER_H
#define BEHAVIOR_TREE_BUILDER_H

#include <cassert>

#include "Node.h"
#include "Leaf.h"
#include "Action.h"

#include "Decorators/Failer.h"
#include "Decorators/Condition.h"
#include "Decorators/Inverter.h"
#include "Decorators/Limiter.h"
#include "Decorators/Repeater.h"
#include "Decorators/Succeeder.h"
#include "Decorators/UntilFail.h"
#include "Decorators/UntilSuccess.h"

#include "Compositors/MemSelector.h"
#include "Compositors/MemSequence.h"
#include "Compositors/ParallelSequence.h"
#include "Compositors/RandomSelector.h"
#include "Compositors/Selector.h"
#include "Compositors/Sequence.h"
#include "Compositors/WhileDo.h"

#include "Binary/Disjunction.h"
#include "Binary/Conjunction.h"
#include "Binary/Equality.h"
#include "Binary/Implication.h"
#include "Binary/ConditionSelector.h"

#ifndef TINY_BEHAVIOR_DEFAULT_NODE_CONTAINER_SIZE
  #define TINY_BEHAVIOR_DEFAULT_NODE_CONTAINER_SIZE 50*sizeof(TinyBehavior::ParallelSequence)
#endif

namespace tb {
//     void printErrorAndExit(const std::string &err) {
//         std::cout << err << "\n";
//         exit(-1);
//     }
  
  class NodeContainer {
  public:
    NodeContainer(const size_t &containerBlockSize) : blockSize(containerBlockSize), currentSize(0), memory(nullptr) {
      allocateNewBlock();
    }
    
    NodeContainer(NodeContainer &&container) : blockSize(container.blockSize), currentSize(container.currentSize), memory(container.memory) {
      container.currentSize = 0;
      container.memory = nullptr;
    }
    
    ~NodeContainer() {
      char* curBlockPtr = memory;
      while (curBlockPtr != memory) {
        char* nextBlock = reinterpret_cast<char**>(curBlockPtr)[0];
        delete [] curBlockPtr;
        curBlockPtr = nextBlock;
      }
    }
    
    NodeContainer() = delete;
    NodeContainer(const NodeContainer &container) = delete;
    
    NodeContainer & operator=(const NodeContainer &container) = delete;
    NodeContainer & operator=(NodeContainer &&container) = delete;
//     void operator=(NodeContainer&& container) {
//       blockSize = std::move(container.blockSize);
//       currentSize = container.currentSize;
//       memory = container.memory;
//       container.currentSize = 0;
//       container.memory = nullptr;
//     }
    
    template<typename T, typename... Args>
    T* create(Args&&... args) {
      assert(sizeof(T) < blockSize && "Bad node container size");
      
      if (currentSize + sizeof(T) > blockSize) {
        allocateNewBlock();
      }
      
      T* memoryPtr = reinterpret_cast<T*>(memory + sizeof(char*) + currentSize);
      new (memoryPtr) T(std::forward<Args>(args)...);
      currentSize += sizeof(T); 
      
      return memoryPtr;
    }
    
    template<typename T>
    void destroy(T* ptr) {
      ptr->~T();
    }
    
    constexpr size_t size() const {
      return blockSize;
    }
  private:
    const size_t blockSize;
    size_t currentSize;
    char* memory;
    
    void allocateNewBlock() {
      const size_t newSize = blockSize + sizeof(char*);
      char* newBuffer = new char[newSize];
      
      currentSize = 0;
      
      char** tmp = reinterpret_cast<char**>(newBuffer);
      tmp[0] = memory;
      memory = newBuffer;
    }
  };

  class BehaviorTree : public Node {
    friend class BehaviorTreeBuilder;
  public:
    BehaviorTree(const size_t &containerSize) : Node(TINY_BEHAVIOR_TREE_NODE_TYPE), container(containerSize) {}
    BehaviorTree(NodeContainer &&container) : Node(TINY_BEHAVIOR_TREE_NODE_TYPE), container(std::move(container)) {}
    ~BehaviorTree() {
      for (auto &node : nodes) {
        //delete node;
        container.destroy(node);
      }
    }

    status update(void* const& data = nullptr) override {
      if (root == nullptr) {
        std::cout << "This tree is invalid and must be deleted!" << "\n";
        exit(-1);
      }

      return root->update(data);
    }

    std::string toString() const override { return "BehaviorTree"; }
    void print(const std::string &indent) const override {
      if (root != nullptr) root->print(indent);
    }
    
    constexpr static uint32_t getType() { return TINY_BEHAVIOR_TREE_NODE_TYPE; }
    
    Node* getRunning() { return currentAction; }
    void setRunning(Node* node) override { currentAction = node; }
  private:
    Node* root = nullptr;
    Node* currentAction = nullptr;
    std::vector<Node*> nodes;
    NodeContainer container;
  };

  class BehaviorTreeBuilder {
  public:
    BehaviorTreeBuilder() : currentContainer(TINY_BEHAVIOR_DEFAULT_NODE_CONTAINER_SIZE) { /*callback = std::bind(&printErrorAndExit, std::placeholders::_1);*/ }
    BehaviorTreeBuilder(const size_t &containerSize) : currentContainer(containerSize) {}
    virtual ~BehaviorTreeBuilder() {
      for (auto &node : nodes) {
        currentContainer.destroy(node);
      }
    }
    
    BehaviorTreeBuilder & sequence(Sequence** out = nullptr) {
      error();

      //Node* node = new Sequence();
      Sequence* node = currentContainer.create<Sequence>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      compositeOrBinaryNode.push_back(node);
      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & selector(Selector** out = nullptr) {
      error();

//         Node* node = new Selector();
      Selector* node = currentContainer.create<Selector>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      compositeOrBinaryNode.push_back(node);
      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & parallel(const uint32_t &minSuccess = 0, const uint32_t &minFail = 0, ParallelSequence** out = nullptr) {
      error();

//         ParallelSequence* node = new ParallelSequence(minSuccess, minFail);
      ParallelSequence* node = currentContainer.create<ParallelSequence>(minSuccess, minFail);

      checkCurrentNode(node);
      if (out != nullptr) *out = node;

      compositeOrBinaryNode.push_back(node);
      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & memSelector(MemSelector** out = nullptr) {
      error();

//         MemSelector* node = new MemSelector();
      MemSelector* node = currentContainer.create<MemSelector>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      compositeOrBinaryNode.push_back(node);
      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & memSequence(MemSequence** out = nullptr) {
      error();

//         MemSequence* node = new MemSequence();
      MemSequence* node = currentContainer.create<MemSequence>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      compositeOrBinaryNode.push_back(node);
      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & random(const uint32_t &seed = 0, RandomSelector** out = nullptr) {
      error();

//         RandomSelector* node = new RandomSelector(seed);
      RandomSelector* node = currentContainer.create<RandomSelector>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      compositeOrBinaryNode.push_back(node);
      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & whiledo(const std::function<PREDICATE_SIGNATURE> &f, WhileDo** out = nullptr) {
      error();

//         WhileDo* node = new WhileDo();
      WhileDo* node = currentContainer.create<WhileDo>();
      node->setPredicate(f);
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      compositeOrBinaryNode.push_back(node);
      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & end() {
      if (currentNode->is<Decorator>()) if (callback) callback("Empty decorator!");
      if (compositeOrBinaryNode.empty()) if (callback) callback("There are no composite nodes that needs to end!");

      compositeOrBinaryNode.pop_back();
      if (compositeOrBinaryNode.empty()) currentNode = nullptr;
      else currentNode = compositeOrBinaryNode.back();

      return *this;
    }
    
    BehaviorTreeBuilder & inverter(Inverter** out = nullptr) {
      error();

//         Inverter* node = new Inverter();
      Inverter* node = currentContainer.create<Inverter>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & repeater(const uint32_t &limit = 0, Repeater** out = nullptr) {
      error();

//         Repeater* node = new Repeater(limit);
      Repeater* node = currentContainer.create<Repeater>(limit);
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & limiter(const uint32_t &limit = 0, Limiter** out = nullptr) {
      error();

//         Limiter* node = new Limiter(limit);
      Limiter* node = currentContainer.create<Limiter>(limit);
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & untilFail(UntilFail** out = nullptr) {
      error();

//         UntilFail* node = new UntilFail();
      UntilFail* node = currentContainer.create<UntilFail>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & untilSuccess(UntilSuccess** out = nullptr) {
      error();

//         UntilSuccess* node = new UntilSuccess();
      UntilSuccess* node = currentContainer.create<UntilSuccess>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & failer(Failer** out = nullptr) {
      error();

//         Failer* node = new Failer();
      Failer* node = currentContainer.create<Failer>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & succeeder(Succeeder** out = nullptr) {
      error();

//         Succeeder* node = new Succeeder();
      Succeeder* node = currentContainer.create<Succeeder>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & condition(const std::function<PREDICATE_SIGNATURE> &f, Condition** out = nullptr) {
      error();

//         Condition* node = new Condition();
      Condition* node = currentContainer.create<Condition>();
      node->setPredicate(f);
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & conjunction(Conjunction** out = nullptr) {
      error();

//         Conjunction* node = new Conjunction();
      Conjunction* node = currentContainer.create<Conjunction>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      compositeOrBinaryNode.push_back(node);
      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & disjunction(Disjunction** out = nullptr) {
      error();

//         Disjunction* node = new Disjunction();
      Disjunction* node = currentContainer.create<Disjunction>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      compositeOrBinaryNode.push_back(node);
      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & equality(Equality** out = nullptr) {
      error();

//         Equality* node = new Equality();
      Equality* node = currentContainer.create<Equality>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      compositeOrBinaryNode.push_back(node);
      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & implication(Implication** out = nullptr) {
      error();

//         Implication* node = new Implication();
      Implication* node = currentContainer.create<Implication>();
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      compositeOrBinaryNode.push_back(node);
      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & ifelse(const std::function<PREDICATE_SIGNATURE> &f, ConditionSelector** out = nullptr) {
      error();

//         ConditionSelector* node = new ConditionSelector();
      ConditionSelector* node = currentContainer.create<ConditionSelector>();
      node->setPredicate(f);
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      compositeOrBinaryNode.push_back(node);
      nodes.push_back(node);
      currentNode = node;

      return *this;
    }
    
    BehaviorTreeBuilder & action(const std::function<Node::status(Node* const&, void* const&)> &f, Action** out = nullptr) {
      error();

//         Action* node = new Action();
      Action* node = currentContainer.create<Action>();
      node->setAction(f);
      if (out != nullptr) *out = node;

      checkCurrentNode(node);

      nodes.push_back(node);
      if (!compositeOrBinaryNode.empty()) currentNode = compositeOrBinaryNode.back();
      else currentNode = nullptr;

      return *this;
    }
    
    template<typename T, typename ...Args>
    BehaviorTreeBuilder & leaf(Args&&... args, T** out = nullptr) {
      T* node = currentContainer.create<T>(std::forward<Args>(args)...);
      
      if (out != nullptr) *out = node;
      
      checkCurrentNode(node);

      nodes.push_back(node);
      if (!compositeOrBinaryNode.empty()) currentNode = compositeOrBinaryNode.back();
      else currentNode = nullptr;

      return *this;
    }
    
    virtual BehaviorTreeBuilder & add(Node* node) {
      error();

      bool emptyDecorator = false;
      checkCurrentNode(node);
      addToNodesRecursive(node, emptyDecorator);

      if (node->is<BehaviorTree>()) {
        throw std::runtime_error("BehaviorTree movement is not implemented yet");
        // тут по идее мы не должны удалять, да и копировать не надо
        // просто сделать так чтобы эти вещи не удалялись при удалении текущего дерева
        // как это сделать?
        
//         auto* tree = reinterpret_cast<BehaviorTree*>(node);
//         tree->nodes.clear();
//         tree->root = nullptr;
//         delete tree;
      }

      if (!emptyDecorator && !compositeOrBinaryNode.empty()) {
        currentNode = compositeOrBinaryNode.back();
      }

      return *this;
    }
    
    BehaviorTree* build() {
      if (!compositeOrBinaryNode.empty()) if (callback) callback("There are still not ended composites or binary!");
      if (!nodes.back()->is<Action>()) if (callback) callback("Last element is not an action!");
      if (currentNode != nullptr) if (callback) callback("currentNode is not null");

      BehaviorTree* tree = new BehaviorTree(std::move(currentContainer));

      for (size_t i = 0; i < nodes.size(); i++) {
        nodes[i]->tree = tree;
        nodes[i]->cIndex = i;
      }

      tree->nodes = nodes;
      tree->root = nodes.front();

      this->currentNode = nullptr;
      compositeOrBinaryNode.clear();
      nodes.clear();

      return tree;
    }

    BehaviorTreeBuilder & debug(const std::string &s) { std::cout << s << "\n"; return *this; }
    void setDebugCallback(const std::function<void(const std::string &)> &c) { callback = c; }
  protected:
    Node* currentNode = nullptr;
    std::vector<Node*> compositeOrBinaryNode;
    std::vector<Node*> nodes;
    NodeContainer currentContainer;
    std::function<void(const std::string &)> callback;

    void error() {
      if (compositeOrBinaryNode.empty() && !nodes.empty() && nodes.back()->is<Action>()) if (callback) callback("Tree already builded!");
      if (currentNode != nullptr && currentNode->is<Action>()) if (callback) callback("Trying to set child to an action!");
    }

    virtual void checkCurrentNode(Node* node) {
      if (node->is<BehaviorTree>()) node = (reinterpret_cast<BehaviorTree*>(node))->nodes.front();

      if (currentNode != nullptr && currentNode->is<Decorator>()) {
        auto* parent = reinterpret_cast<Decorator*>(currentNode);
        parent->setChild(node);
        node->parent = parent;
      }
      
      if (currentNode != nullptr && currentNode->is<Compositor>()) {
        auto* parent = reinterpret_cast<Compositor*>(currentNode);
        parent->addChild(node);
        node->parent = parent;
      }
      
      if (currentNode != nullptr && currentNode->is<Binary>()) {
        auto* parent = reinterpret_cast<Binary*>(currentNode);
        uint8_t index = 0;
        if (parent->hasFirstChild()) index = 1;
        else if (parent->hasSecondChild()) if (callback) callback("Trying to set third child to binary!");
        parent->setChild(index, node);
        node->parent = parent;
      }
    }

    virtual void addToNodesRecursive(Node* node, bool &emptyDecorator) {
      if (node->is<BehaviorTree>()) node = (reinterpret_cast<BehaviorTree*>(node))->nodes.front();

      nodes.push_back(node);

      if (node->is<Compositor>()) {
        if (!(reinterpret_cast<Compositor*>(node))->hasChildren()) compositeOrBinaryNode.push_back(node);
        else for (const auto &child : (reinterpret_cast<Compositor*>(node))->children) addToNodesRecursive(child, emptyDecorator);
        
      } else if (node->is<Decorator>()) {
        auto* child = (reinterpret_cast<Decorator*>(node))->child;
        if (child == nullptr) {
            currentNode = node;
            emptyDecorator = true;
        }
        else addToNodesRecursive((reinterpret_cast<Decorator*>(node))->child, emptyDecorator);
        
      } else if (node->is<Binary>()) {
        auto* binary = (reinterpret_cast<Binary*>(node));
        if (!(binary->hasFirstChild() && binary->hasSecondChild())) compositeOrBinaryNode.push_back(node);
        if (binary->hasFirstChild()) addToNodesRecursive((reinterpret_cast<Binary*>(node))->nodes.first, emptyDecorator);
        if (binary->hasSecondChild()) addToNodesRecursive((reinterpret_cast<Binary*>(node))->nodes.second, emptyDecorator);
      }
    }
  };
}

#endif
