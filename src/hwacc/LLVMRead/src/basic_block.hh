#ifndef __BASIC_BLOCK_HH__

#define __BASIC_BLOCK_HH__

#include "compute_node.hh"

class BasicBlock {
  friend class BasicBlockList;
  private:
    ComputeNode * start;
    ComputeNode * end;
    ComputeNode * currNode;
    unsigned numNodes;
    std::string name;
    BasicBlock * next; //The next basic block in the list
  public:
    BasicBlock(std::string id);
    ~BasicBlock();
    void addNode(ComputeNode * cn);
    ComputeNode * step();
    ComputeNode * getStart() { return start; }
    ComputeNode * getEnd() { return end; }
    std::string getName() { return name; }
  protected:
  
};

class BasicBlockList {
  private:
    BasicBlock *head, *tail;
    unsigned numBasicBlocks;
  public:
    BasicBlockList();
    ~BasicBlockList();
    void addBasicBlock(BasicBlock *bb);
    BasicBlock *findBasicBlock(std::string name);
  protected:
};

#endif //__BASIC_BLOCK_HH__
