# tiny_behaviour
Tiny Behavior is a small, easy to learn and use, header only Behavior Tree library writen in C++.  

## How To

Its easy, look, there is a simple example:  
```c++
#include <iostream>
#include "TinyBehavior/TinyBehavior.h"

int main(int argc, char* argv[]) {
    tb::BehaviorTreeBuilder builder;
    tb::BehaviorTree* tree;
    tree = builder.parallel()
                    .action([] (tb::Node* const& currentNode, void* const& data = nullptr) {
                        std::cout << "Im an action in parallel node!" << "\n";
                        return tb::Node::Status::Success;
                    })
                    .failer()
                        .action([] (tb::Node* const& currentNode, void* const& data = nullptr) {
                            std::cout << "Im another action in parallel node!" << "\n";
                            return tb::Node::Status::Success;
                        })
                    .condition([] (tb::Node* const& currentNode, void* const& data = nullptr) {
                        std::cout << "Some condition before action!" << "\n";
                        return true;
                    })
                        .action([] (tb::Node* const& currentNode, void* const& data = nullptr) {
                            std::cout << "Im an action in the condition!" << "\n";
                            return tb::Node::Status::Running;
                        })
                    .limiter(5)
                        .action([] (tb::Node* const& currentNode, void* const& data = nullptr) {
                            std::cout << "Im an action that succeds or run only 5 times" << "\n";
                            return tb::Node::Status::Success;
                        })
                    // ... and others type of nodes, you can read about them below
                   .end() // parallel needs to be end
                .build();
    tree.print(); // for debugging
    tree.update(); // use
    // ...
    delete tree; // dont forget to delete tree, it deletes all nodes in that tree too
}
```
You can even make your own node:  
```c++
class Idle : public tb::Leaf {
public:
    Idle(bool* enemyInSight) : Leaf(), enemyInSight(enemyInSight) {}
    
    // main function
    Status update(void* const& data = nullptr) override {
        if (*enemyInSight) return Node::Status::Success;
        std::cout << "Im waiting!" << "\n";
        return Node::Status::Failure;
    }
    
    // std::string toString() const {} - can be overriden
    // void print(const std::string &indent) const {} - can be overriden
private:
    bool* enemyInSight;
};
```
and use it in your code:  
```c++
// ...
Idle* idle = new Idle(someBoolPointer);
// ...
tree = builder
                // ...
                .add(idle)
                // ...
            .build();
            
    tree.update();
    // ...
    delete tree; // not needs to delete idle
```
All information about making your own nodes is in paragraph Inheritance.  <br/>

## Tree nodes description
BehaviorTreeBuilder class methods:  <br/>
### Compositor nodes:  <br/>
`sequence()` - updates all nodes until faces `Status::Failure` or `Status::Running`  <br/>
`selector()` - updates all nodes until faces `Status::Success` or `Status::Running`  <br/>
`parallel(minSuccess, minFail)` - updates all nodes, return `Status::Success` if `totalSuccess` >= `minSuccess` or `Status::Failure` if `totalFails` >= `minFail` else `Status::Running`  <br/>
`memSelector()` - updates all nodes until faces `Status::Success` or `Status::Running`, next updation will start from that node  <br/>
`memSequence()` - updates all nodes until faces `Status::Failure` or `Status::Running`, next updation will start from that node  <br/>
`random(seed)` - updates random (depends on seed) node  <br/>
`whiledo(predicate)` - updates all nodes while `condition == true`, returns `Status::Running` in this case, if `condition == false` does nothing and returns `Status::Failure`  <br/>
### Binary nodes:  <br/>
(`Status::Success` is `true`, `Status::Failure` is `false`)<br/>
`conjunction()` - Status first && Status second (`Status::Running` = `Status::Failure`)  <br/>
`disjunction()` - Status first || Status second (`Status::Running` = `Status::Success`)  <br/>
`equality()` - Status first == Status second  <br/>
`implication()` - Status first -> Status second (!(Status first) || Status second) (`Status::Running` = `Status::Failure`)  <br/>
`ifelse(predicate)` - if `contidion == true` updates first, else second  <br/>
### Decorator nodes:  <br/>
`inverter()` - inverts `Status::Success` and `Status::Failure`, `Status::Running` ignored  <br/>
`repeater(limit)` - returns `Status::Running` `limit` times, then return node status and resets  <br/>
`limiter(limit)` - returns decorator's child status `limit` times, then return `Status::Failure`  <br/>
`untilFail()` - returns only `Status::Running` or `Status::Failure`  <br/>
`untilSuccess()` - returns only `Status::Running` or `Status::Success`  <br/>
`failer()` - returns only `Status::Failure`  <br/>
`succeeder()` - returns only `Status::Success`  <br/>
`condition(predicate)` - updates child if `predicate == true` else returns `Status::Failure`  <br/>
### Special nodes:  <br/>
`action(action)` - userdefined action function, must return statuses  <br/>
### Other methods:  <br/>
`end()` - use it after every Compositor or Binary nodes  <br/>
`add(node)` - adds nodes or trees to the current tree, NOTE: old tree will be deleted  <br/>
`build()` - does some postworks and returns `BehaviorTree` pointer  <br/>
`debug(string)` - just prints the string  <br/>
`setDebugCallback(callback)` - set callback function for the error strings  <br/>
  <br/>
## Inheritance
Every nodes in TinyBehavior has the virtual destructor, that means technicaly you can inherit every class, but if you want make a new node type (besides Compositor, Decorator, Binary, Action and Tree) you must implement a static method `constexpr static uint32_t getType() {}` for the template method `is()` in Node class and override `add()` or add a new method to the BehaviorTreeBuilder (which has the virtual destructor too). <br/>
About the main types:  <br/>
### Compositor - methods:  <br/>
`addChild(node)` - adds the child to the node vector  <br/>
`hasChildren()` - is the node vector empty?  <br/>
`getIndex()` - get the current index of node  <br/>
#### Compositor - members:  <br/>
`children` - nodes vector  <br/>
`index` - current index  <br/>
### Decorator - methods:  <br/>
`setChild()` - sets child  <br/>
`hasChild()` - child != nullptr  <br/>
#### Decorator - members:  <br/>
`child` - decorator child  <br/>
### Binary - methods:  <br/>
`setFirstChild(node)` - sets first child  <br/>
`setSecondChild(node)` - sets second child  <br/>
`setChild(index, node)` - sets child using index (0 or 1)  <br/>
`hasFirstChild()` - is first child != nullptr  <br/>
`hasSecondChild()` - is second child != nullptr  <br/>
#### Binary - members:  <br/>
`nodes` - pair of childs  <br/>
  <br/>
`update(data)`, `toString()` and `print(indent)` can be overriden from any class. Check source code for more information  <br/>
  
## ToDo
1. Some other nodes  
2. Some other node types  
3. Delete repeater?  
4. Tests  
5. Benchmarks  
6. Some optimisation  
  
## Licence  
MIT  
