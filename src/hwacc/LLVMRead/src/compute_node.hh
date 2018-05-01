#ifndef __COMPUTE_NODE_HH__

#define __COMPUTE_NODE_HH__

#include "register.hh"
#include "operations.hh"
#include "instruction.hh"
#include "hwacc/comm_interface.hh"
#include "base/types.hh"
#include <vector>

class ComputeNode {
  friend class BasicBlock;

private:
  enum opValue {
    evNotDefined,
    IR_Move,
    IR_Ret,
    IR_Br,
    IR_Switch,
    IR_IndirectBr,
    IR_Invoke,
    IR_Resume,
    IR_Unreachable,
    IR_Add,
    IR_FAdd,
    IR_Sub,
    IR_FSub,
    IR_Mul,
    IR_FMul,
    IR_UDiv,
    IR_SDiv,
    IR_FDiv,
    IR_URem,
    IR_SRem,
    IR_FRem,
    IR_Shl,
    IR_LShr,
    IR_AShr,
    IR_And,
    IR_Or,
    IR_Xor,
    IR_Alloca,
    IR_Load,
    IR_Store,
    IR_GetElementPtr,
    IR_Fence,
    IR_AtomicCmpXchg,
    IR_AtomicRMW,
    IR_Trunc,
    IR_ZExt,
    IR_SExt,
    IR_FPToUI,
    IR_FPToSI,
    IR_UIToFP,
    IR_SIToFP,
    IR_FPTrunc,
    IR_FPExt,
    IR_PtrToInt,
    IR_IntToPtr,
    IR_BitCast,
    IR_AddrSpaceCast,
    IR_ICmp,
    IR_FCmp,
    IR_PHI,
    IR_Call,
    IR_Select,
    IR_VAArg,
    IR_ExtractElement,
    IR_InsertElement,
    IR_ShuffleVector,
    IR_ExtractValue,
    IR_InsertValue,
    IR_LandingPad,
    IR_DMAFence,
    IR_DMAStore,
    IR_DMALoad,
    IR_IndexAdd,
    IR_SilentStore,
    IR_Sine,
    IR_Cosine
  };

  typedef std::map<std::string, int> OPMap;
  OPMap s_opMap = {
      {"move", IR_Move},
      {"ret", IR_Ret},
      {"br", IR_Br},
      {"switch", IR_Switch},
      {"indirectbr", IR_IndirectBr},
      {"invoke", IR_Invoke},
      {"resume", IR_Resume},
      {"unreachable", IR_Unreachable},
      {"add", IR_Add},
      {"fadd", IR_FAdd},
      {"sub", IR_Sub},
      {"fsub", IR_FSub},
      {"mul", IR_Mul},
      {"fmul", IR_FMul},
      {"udiv", IR_UDiv},
      {"sdiv", IR_SDiv},
      {"fdiv", IR_FDiv},
      {"urem", IR_URem},
      {"srem", IR_SRem},
      {"frem", IR_FRem},
      {"shl", IR_Shl},
      {"lshr", IR_LShr},
      {"ashr", IR_AShr},
      {"and", IR_And},
      {"or", IR_Or},
      {"xor", IR_Xor},
      {"alloca", IR_Alloca},
      {"load", IR_Load},
      {"store", IR_Store},
      {"getelementptr", IR_GetElementPtr},
      {"fence", IR_Fence},
      {"cmpxchg", IR_AtomicCmpXchg},
      {"atomicrmw", IR_AtomicRMW},
      {"trunc", IR_Trunc},
      {"zext", IR_ZExt},
      {"sext", IR_SExt},
      {"fptoui", IR_FPToUI},
      {"fptosi", IR_FPToSI},
      {"uitofp", IR_UIToFP},
      {"sitofp", IR_SIToFP},
      {"fptrunc", IR_FPTrunc},
      {"fpext", IR_FPExt},
      {"ptrtoint", IR_PtrToInt},
      {"inttoptr", IR_IntToPtr},
      {"bitcast", IR_BitCast},
      {"addrspacecast", IR_AddrSpaceCast},
      {"icmp", IR_ICmp},
      {"fcmp", IR_FCmp},
      {"phi", IR_PHI},
      {"call", IR_Call},
      {"select", IR_Select},
      {"va_arg", IR_VAArg},
      {"extractelement", IR_ExtractElement},
      {"insertelement", IR_InsertElement},
      {"shufflevector", IR_ShuffleVector},
      {"extractvalue", IR_ExtractValue},
      {"insertvalue", IR_InsertValue},
      {"landingpad", IR_LandingPad},
      {"dmafence", IR_DMAFence},
      {"dmastore", IR_DMAStore},
      {"dmaload", IR_DMALoad},
      {"indexadd", IR_IndexAdd},
      {"silentstore", IR_SilentStore},
      {"sine", IR_Sine},
      {"cosine", IR_Cosine}};

  Instruction instruction;
  int dependencies = 0;
  std::string prevBB;
  CommInterface *comm;

public:
  ComputeNode(std::string line, RegisterList *list, std::string prev, CommInterface *co);
  ~ComputeNode();
  Instruction getInstruction() { return instruction; }
  void setPrevBB(std::string prev) { prevBB = prev; }
  void reset();
  void compute();
  bool commit();
  bool checkDependency();
  void debug(std::vector<std::string> &parameters);
  void setFlags(std::vector<std::string> &parameters, Instruction &instruction);
  bool isRegister(std::string data);
  void setOperands(RegisterList *list, std::vector<std::string> &parameters, Instruction &instruction);
  void initializeReturnRegister(std::vector<std::string> &parameters, Instruction &instruction);
protected:
};

#endif //__COMPUTE_NODE_HH__
