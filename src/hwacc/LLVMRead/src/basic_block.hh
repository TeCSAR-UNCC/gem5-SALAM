#ifndef __BASIC_BLOCK_HH__

#define __BASIC_BLOCK_HH__

#include "compute_node.hh"
#include "debug.hh"

#include <list>

class BasicBlock {
  friend class LLVMInterface;
  private:
    std::list<ComputeNode *> *cnList;
    std::string name;
    unsigned bbID;
  public:
    BasicBlock(std::string id, unsigned bbnum);
    ~BasicBlock();
    void addNode(ComputeNode * cn);
    std::string getName() { return name; }
  protected:
  
};

#endif //__BASIC_BLOCK_HH__
