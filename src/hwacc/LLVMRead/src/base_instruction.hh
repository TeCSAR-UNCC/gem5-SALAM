#ifndef __BASE_INSTRUCTION_HH__
#define __BASE_INSTRUCTION_HH__

#include "hwacc/comm_interface.hh"
#include "mem_request.hh"
#include "llvm_types.hh"
#include "debugFlags.hh"
#include "registers.hh"
#include "base_instruction.hh"
#include "power.hh"
#include <string>
#include <vector>

#define Details(name) DPRINTF(ClassDetail, "Creating Instance of %s!\n", name)
#define Destruct(name) DPRINTF(ClassDetail, "Deleting Instance of %s!\n", name)
//---------------------------------------------------------------------------//
//--------- Begin Immediate Value Sub Classes -------------------------------//
//---------------------------------------------------------------------------//
class Signed {
    protected: 
        int64_t _SOperand;
    public:
        // ---- Constructor
        Signed(                     int64_t Operand)       
        : _SOperand(                (int64_t)Operand) 
                                    { Details("Signed"); }
        virtual ~Signed()           { Destruct("Signed"); }
        // ---- Get Functions
        int64_t getSigned()         { return _SOperand; }
};

class Unsigned {
    protected:
        uint64_t _UOperand;
    public:

        Unsigned(                   uint64_t Operand)         
        : _UOperand(                (uint64_t)Operand) 
                                    { Details("Unsigned"); }
        virtual ~Unsigned()         { Destruct("Unsigned"); }
        // ---- Get Functions
        int64_t getUnsigned()       { return _UOperand; }
};

class Integer {
    protected:
        int64_t _Operand;
    public:
        // ---- Constructor
        Integer(                    int64_t Operand)         
        : _Operand(                 Operand) 
                                    { Details("Integer"); }
        virtual ~Integer()          { Destruct("Integer"); }
        // ---- Get Functions
        int64_t getOperand()        { return _Operand; }
};

class FloatingPointSP {
    protected:
        float _OperandSP;
    public:
        // ---- Constructor
        FloatingPointSP(            float Operand)
        : _OperandSP(               Operand) 
                                    { Details("FloatingPointSP"); }
        virtual ~FloatingPointSP()  { Destruct("FloatingPointSP"); }
        // ---- Get Functions
        float getOperandSP()        { return _OperandSP; }
};

class FloatingPointDP {
    protected:
        double _OperandDP;
    public:
        // ---- Constructor
        FloatingPointDP(            double Operand)
        : _OperandDP(               Operand) 
                                    { Details("FloatingPointDP"); }
        virtual ~FloatingPointDP()  { Destruct("FloatingPointDP"); }
        // ---- Get Functions
        double getOperandDP()           { return _OperandDP; }
};
//---------------------------------------------------------------------------//
//--------- End Immediate Value Sub Classes ---------------------------------//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//--------- Begin Shared Instruction Base -----------------------------------//
//---------------------------------------------------------------------------//
class InstructionBase {
    public:
       
        MemoryRequest* _Req; // Pointer for creating a memory access request
        std::string _LLVMLine;
        std::string _OpCode;
        std::string _ReturnType; // Return Type
        std::string _InstructionType; // Terminator, Binary, Etc...
        Register* _ReturnRegister;
        uint64_t _MaxCycle;
        std::vector<Register*> _Dependencies;
        CommInterface* _Comm; // Pointer to add basic block to queues 
        std::vector<InstructionBase*> _Parents; // Parent Nodes
        std::vector<InstructionBase*> _Children; // Child Nodes
        int _ActiveParents; //Number of active parents. Instruction can call compute() when _ActiveParents==0
        uint64_t _Usage; // Counter for times instruction used
        uint64_t _CurrCycle;
        std::string _PrevBB;
        bool _Terminator = false;
        std::string _Dest;
        uint64_t _FinalResult;
        std::vector<int64_t> _Ops; 
        int8_t _FunctionalUnit;
        // ---- Constructor
        InstructionBase( const std::string& LLVMLine,
                         const std::string& OpCode,
                         const std::string& ReturnType,
                         const std::string& InstructionType,
                         Register* ReturnRegister,
                         uint64_t MaxCycle,
                         std::vector<Register*> Dependencies,
                         CommInterface* Comm):
                         _LLVMLine(LLVMLine),
                         _OpCode(OpCode), 
                         _ReturnType(ReturnType),
                         _InstructionType(InstructionType),
                         _ReturnRegister(ReturnRegister),
                         _MaxCycle(MaxCycle),
                         _Dependencies(Dependencies),
                         _Comm(Comm) 
                        { _Req = NULL;
                          _CurrCycle = 0; 
                          _Usage = 0;
                          _ActiveParents = 0; 
                          Details("Instruction Base"); 
                          while(_Dependencies.size() != _Ops.size()) _Ops.push_back(0);
                          _FunctionalUnit = -1;
                          }
        InstructionBase( const std::string& LLVMLine,
                         const std::string& OpCode,
                         const std::string& ReturnType,
                         const std::string& InstructionType,
                         Register* ReturnRegister,
                         uint64_t MaxCycle,
                         std::vector<Register*> Dependencies,
                         CommInterface* Comm,
                         int8_t FunctionalUnit):
                         _LLVMLine(LLVMLine),
                         _OpCode(OpCode), 
                         _ReturnType(ReturnType),
                         _InstructionType(InstructionType),
                         _ReturnRegister(ReturnRegister),
                         _MaxCycle(MaxCycle),
                         _Dependencies(Dependencies),
                         _Comm(Comm),
                         _FunctionalUnit(FunctionalUnit) 
                        { _Req = NULL;
                          _CurrCycle = 0; 
                          _Usage = 0;
                          _ActiveParents = 0; 
                          Details("Instruction Base"); 
                          while(_Dependencies.size() != _Ops.size()) _Ops.push_back(0);
                          }                  
        // ---- Get Functions
        std::string getLLVMLine()      { return _LLVMLine; }
        std::string getOpCode()        { return _OpCode; }
        std::string getInstrType()     { return _InstructionType; }
        // ---- Virtual Functions
        virtual ~InstructionBase()     { Destruct("Instruction Base"); }
        void signalChildren();
        virtual bool commit();
        // Each commit increments cycle count once, broadcast once complete
        // If memory type, commit request and broadcast
        virtual void compute()           = 0;  
        //virtual void powerCycle()        = 0;
        virtual InstructionBase* clone() const = 0;
        
        // ---- Hardware Usage Functions
        void used() { _Usage++; }
        // ---- Dependency Graph Functions
            // Find Parents and Return Register for Previous Instance 
        virtual std::vector<Register*> runtimeDependencies(std::string PrevBB);
        void fetchDependency(Register*);
        void fetchDependency(int);
        void registerChild(InstructionBase*);
        void registerParent(InstructionBase*);
        // ---- General Functions
        void setCommInterface(CommInterface *newComm) { _Comm = newComm; }
        MemoryRequest * getReq() { return _Req; }
        void setResult(void *Data);
};
//---------------------------------------------------------------------------//
//--------- End Shared Instruction Base -------------------------------------//
//---------------------------------------------------------------------------//


#endif //__BASE_INSTRUCTION_HH__
