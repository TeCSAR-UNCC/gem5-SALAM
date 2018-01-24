#ifndef __COMPUTE_NODE_HH__

#define __COMPUTE_NODE_HH__

#include "register.hh"

class ComputeNode {
  friend class BasicBlock;
  private:
    ComputeNode * next;
    std::string llvm_inst;
    Register * returnRegister;
    std::string op;
    std::string params;
    std::string prevBB;
    char finished;
  public:
    ComputeNode(std::string line, RegisterList *list, std::string prev);
    ~ComputeNode();
    void compute();
    std::string computeBranch();
    bool isBranch() { return (op.compare("br") == 0); }
    void checkState();
  protected:
  
};

#endif //__COMPUTE_NODE_HH__
