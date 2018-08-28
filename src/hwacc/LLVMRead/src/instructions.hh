#ifndef __INSTRUCTIONS_HH__
#define __INSTRUCTIONS_HH__

#include "hwacc/comm_interface.hh"
#include "mem_request.hh"
#include "llvm_types.hh"
#include "debugFlags.hh"
#include "registers.hh"
#include "power.hh"
#include <string>
#include <vector>

// counter type in parser 0 cycle


//---------------------------------------------------------------------------//
//--------- Begin Immediate Value Sub Classes -------------------------------//
//---------------------------------------------------------------------------//
class Signed {
    protected: 
        int64_t _SOperand;
    public:
        // ---- Constructor
        Signed(           int64_t Operand):           
                        _SOperand((int64_t)Operand) { }
        // ---- Get Functions
        int64_t getSigned() { return _SOperand; }
};

class Unsigned {
    protected:
        uint64_t _UOperand;
    public:
            // ---- Constructor
        Unsigned(        uint64_t Operand):           
                        _UOperand((uint64_t)Operand) { }
        // ---- Get Functions
        int64_t getUnsigned() { return _UOperand; }
};

class Integer {
    protected:
        int64_t _Operand;
    public:
        // ---- Constructor
        Integer(         int64_t Operand):           
                        _Operand(Operand) { }
        // ---- Get Functions
        int64_t getOperand()           { return _Operand; }
};

class FloatingPointSP {
    protected:
        float _OperandSP;
    public:
        // ---- Constructor
        FloatingPointSP(   float Operand): 
                        _OperandSP(Operand) { }
        // ---- Get Functions
        float getOperandSP()           { return _OperandSP; }
};

class FloatingPointDP {
    protected:
        double _OperandDP;
    public:
        // ---- Constructor
        FloatingPointDP(  double Operand): 
                        _OperandDP(Operand) { }
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
    protected:
       
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
        std::vector<bool> _Status; // Ready Indicator, Index Matched To Parent
        uint64_t _Usage; // Counter for times instruction used
        uint64_t _CurrCycle;
    public:
        // ---- Constructor
        /* Default Compute Node Construction Call
        (lineCpy, opCode, returnType, instructionType, ret_reg, maxCycles, dependencies, co)
        */
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
                          _Usage = 0; }
        // ---- Get Functions
        std::string getLLVMLine()      { return _LLVMLine; }
        std::string getOpCode()        { return _OpCode; }
        std::string getInstrType()     { return _InstructionType; }
        // ---- Virtual Functions
        virtual bool commit()            = 0;
        virtual void compute()           = 0;  
        virtual void powerCycle()        = 0;
        // ---- Hardware Usage Functions
        void used() { _Usage++; }
        // ---- Dependency Graph Functions
            // Find Parents and Return Register for Previous Instance 
        std::vector<InstructionBase*> checkDependencies();
        void signalChildren(InstructionBase*); 
        void registerChild(InstructionBase*);
        // ---- General Functions
        void setCommInterface(CommInterface *newComm) { _Comm = newComm; }
        MemoryRequest * getReq() { return _Req; }
};
//---------------------------------------------------------------------------//
//--------- End Shared Instruction Base -------------------------------------//
//---------------------------------------------------------------------------//
//--------- Begin Terminator Instruction Base -------------------------------//
//---------------------------------------------------------------------------//
class Terminator : public InstructionBase {
    protected:
        Register* _Condition;
        // _Condition Usage
        // Br: Boolean, iftrue or iffalse
        // Switch: Integer Value to Match Case
        std::string _Destination;
        std::vector<std::string> _Branches;
        // _Branches Usage
        // Br: [0] == iftrue, [1] == iffalse
        // Switch: [0] == default, [1] == case 1, [2] == case 2, etc...
    public:
        Terminator ( const std::string& Destination) : // Unconditional Branch
                    _Destination(Destination) { }
        Terminator ( std::vector<std::string> Branches,
                    Register* Condition) : // Conditional Branches
                    _Condition(Condition),
                    _Branches(Branches) { }   
};

class Ret : public Terminator {
    protected:

    public:
        Ret (               const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) { }
};

class Br : public Terminator {
    protected:
        bool _Unconditional;

    public:
        // Unconditional Branch Constructor
        Br (                const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string Destination,
                            bool Unconditional)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Terminator (      Destination)
        , _Unconditional(   Unconditional) { }
        // Conditional Branch Constructor
        Br (                const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            Register* Condition,
                            std::vector<std::string> Branches,
                            bool Unconditional)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Terminator (      Condition,
                            Branches)
        , _Unconditional(   Unconditional) { }
};

class Switch : public Terminator {
    protected:
    // returnType is switch statement Int Type
    std::vector<std::int> _CaseValues;
    public:
        Switch (            const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            Register* Condition,
                            std::vector<std::string> Branches,
                            std::vector<int> CaseValues)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Terminator (      Condition,
                            Branches)
        , _CaseValues(      CaseValues) { }
};

class IndirectBr : public Terminator {
    // Not Implemented
};

class Invoke : public Terminator {
    // Not Implemented
};

class Resume : public Terminator {
    // Not Implemented
};

class CatchSwitch : public Terminator {
    // Not Implemented
};

class CatchRet : public Terminator {
    // Not Implemented
};

class CleanupRet : public Terminator {
    // Not Implemented
};

class Unreachable : public Terminator {
    // Not Implemented
};
//---------------------------------------------------------------------------//
//--------- End Terminator Instruction Base ---------------------------------//
//---------------------------------------------------------------------------//
//--------- Begin Binary Instruction Base -----------------------------------//
//---------------------------------------------------------------------------//
class Binary : public InstructionBase {
    protected:
        std::vector<Register*> _Operands;
        uint64_t _Result;
        uint64_t _Flags;
    public:
        // ---- Constructor
        Binary(     std::vector<Register*> Operands,
                    uint64_t Flags)
        :           _Operands(Operands),
                    _Flags(Flags) { }
};
// ---- Binary ---- Integer Instructions

class Add : public Binary, public Integer {
    protected:

    public:
        Add (               const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Binary (          RegOps,
                            Flags)
        , Integer (         ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class Sub : public Binary, public Integer {
    protected:

    public:
        Sub (               const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Binary (          RegOps,
                            Flags)
        , Integer (         ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class Mul : public Binary, public Integer {
    protected:
        
    public:
        Mul  (               const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Binary (          RegOps,
                            Flags)
        , Integer (         ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class UDiv : public Binary, public Integer, public Unsigned {
    protected:

    public:
        UDiv  (               const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Binary (          RegOps,
                            Flags)
        , Integer (         ImmOp) 
        , Unsigned (        ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class SDiv : public Binary, public Integer, public Signed {
    protected:

    public:
    SDiv  (               const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Binary (          RegOps,
                            Flags)
        , Integer (         ImmOp) 
        , Signed (          ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class URem : public Binary, public Integer, public Unsigned {
    protected:
        URem  (               const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Binary (          RegOps,
                            Flags)
        , Integer (         ImmOp) 
        , Unsigned (        ImmOp) { }
    public:
        void compute() override;
        void powerCycle() override;
};

class SRem : public Binary, public Integer, public Signed {
    protected:

    public:
        SRem (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Binary (          RegOps,
                            Flags)
        , Integer (         ImmOp) 
        , Signed (          ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

// ---- Binary ---- Floating Point Instructions

class FAdd : public Binary, public FloatingPointSP, public FloatingPointDP {
    protected:

    public:
        FAdd (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Binary (          RegOps,
                            Flags)
        , FloatingPointSP ( ImmOp) 
        , FloatingPointDP ( ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class FSub : public Binary, public FloatingPointSP, public FloatingPointDP {
    protected:

    public:
        FSub (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Binary (          RegOps,
                            Flags)
        , FloatingPointSP ( ImmOp) 
        , FloatingPointDP ( ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class FMul : public Binary, public FloatingPointSP, public FloatingPointDP {
    protected:

    public:
    FMul (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Binary (          RegOps,
                            Flags)
        , FloatingPointSP ( ImmOp) 
        , FloatingPointDP ( ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class FDiv : public Binary, public FloatingPointSP, public FloatingPointDP {
    protected:

    public:
    FDiv (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Binary (          RegOps,
                            Flags)
        , FloatingPointSP ( ImmOp) 
        , FloatingPointDP ( ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class FRem : public Binary, public FloatingPointSP, public FloatingPointDP {
    protected:

    public:
    FRem (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Binary (          RegOps,
                            Flags)
        , FloatingPointSP ( ImmOp) 
        , FloatingPointDP ( ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

//---------------------------------------------------------------------------//
//--------- End Binary Instruction Base -------------------------------------//
//---------------------------------------------------------------------------//
//--------- Begin Bitwise Instruction Base ----------------------------------//
//---------------------------------------------------------------------------//
class Bitwise : public InstructionBase {
    protected:
        std::vector<Register*> _Operands;
        uint64_t _Result;
        uint64_t _Flag;
   public:
        // ---- Constructor
        Binary(     std::vector<Register*> Operands,
                    uint64_t Flags)
        :           _Operands(Operands),
                    _Flags(Flags) { }
};
// ---- Bitwise Instructions

class Shl : public Bitwise, public Integer {
    protected:

    public:
    Shl (                   const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Bitwise (         RegOps,
                            Flags)
        , Integer (         ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class LShr : public Bitwise, public Integer {
    protected:

    public:
    LShr (                   const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Bitwise (         RegOps,
                            Flags)
        , Integer (         ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class AShr : public Bitwise, public Integer {
    protected:

    public:
    AShr (                   const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Bitwise (         RegOps,
                            Flags)
        , Integer (         ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class And : public Bitwise, public Integer {
    protected:

    public:
        And  (               const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Bitwise (         RegOps,
                            Flags)
        , Integer (         ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class Or : public Bitwise, public Integer {
    protected:

    public:
        Or (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Bitwise (         RegOps,
                            Flags)
        , Integer (         ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class Xor : public Bitwise, public Integer {
    protected:

    public:
    Xor (                  const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Bitwise (         RegOps,
                            Flags)
        , Integer (         ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

//---------------------------------------------------------------------------//
//--------- End Bitwise Instruction Base ------------------------------------//
//---------------------------------------------------------------------------//
//--------- Begin Conversion Instruction Base -------------------------------//
//---------------------------------------------------------------------------//
class Conversion : public InstructionBase {
    protected:
        Register* _Operand;
        std::string _OriginalType;
        uint64_t _Result;

    public:
        // ---- Constructor
        Conversion(         const std::string& OriginalType,
                            Register* Operand) :
        _OriginalType(      OriginalType),
        _Operand(           Operand) { }
};
// ---- Conversion Instructions

class Trunc : public Conversion {
    public:
        Trunc (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

class ZExt : public Conversion {
    public:
        ZExt (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

class SExt : public Conversion {
    public:
        SExt (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

class FPToUI : public Conversion {
    public:
        FPToUI (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

class FPToSI : public Conversion {
    public:
        FPToSI (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

class UIToFP : public Conversion {
    public:
        UIToFP (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

class SIToFP : public Conversion {
    public:
        SIToFP (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

class FPTrunc : public Conversion {
    public:
        FPTrunc (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

class FPExt : public Conversion {
    public:
        FPExt (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

class PtrToInt : public Conversion {
    public:
        PtrToInt (          const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

class IntToPtr : public Conversion {
    public:
        IntToPtr (          const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

class BitCast : public Conversion {
    public:
        BitCast (           const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

class AddrSpaceCast : public Conversion {
    public:
        AddrSpaceCast (     const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Conversion (      OriginalType,
                            Operand)
};

//---------------------------------------------------------------------------//
//--------- End Conversion Instruction Base ---------------------------------//
//---------------------------------------------------------------------------//
//--------- Begin Memory Instruction Base -----------------------------------//
//---------------------------------------------------------------------------//
class Memory : public InstructionBase {
    protected:
        // Address
        // alignment

    public:
        Memory();

};

class Load : public Memory {
    protected:
        uint64_t _Align;
        Register* _Pointer;
    public:
        Load();
};

class Store : public Memory {
    protected:
        uint64_t _Align;
        uint64_t _Imm;
        Register* _Pointer;
        Register* _Value;
    public:
        Store();
};

class GetElementPtr : public Memory {
    protected:
        std::string _Pty;
        LLVMType* _LLVMType;
        std::vector<Register*> _Idx;
        std::vector<std::string> _Type;
        std::vector<int64_t> _ImmIdx;
        Register* _PtrVal;
        uint64_t _Index;

    public:
        GetElementPtr();
};
//---------------------------------------------------------------------------//
//--------- End Memory Instruction Base -------------------------------------//
//---------------------------------------------------------------------------//
//--------- Begin Other Instruction Base ------------------------------------//
//---------------------------------------------------------------------------//
class Other : public InstructionBase {
    protected:


    public:

};

class Phi : public Other, public Integer {
/* 
			std::string ty;
			std::string ival[MAXPHI];
			std::string ilabel[MAXPHI];
			bool immVal[MAXPHI];
			bool immLabel[MAXPHI];
			Register *val[MAXPHI];
			std::string label[MAXPHI];
			mutable Register *takenVal;
*/
    protected:
        std::vector<std::string> _PhiVal; // Value to be loaded
		std::vector<std::string> _PhiLabel; // If from this BB

    public:
        Phi();

};

class Select : public Other, public Integer, public FloatingPointSP, public FloatingPointDP {
/*
			Register *cond;
			Register *val1;
			Register *val2;
			bool icondFlag = false;
			bool icond = false;
			int immVal[2];
			std::string ty;
			bool intTy = false;
			bool floatTy = false;
			bool doubleTy = false;
			bool immediate[2];
*/
    protected:
        Register* _Condition;
        std::vector<Register*> _RegValues;
		std::vector<int64_t> _ImmValues;
        std::vector<bool> _Imm;
    public:
        Select();
};
// ---- Other Sub Type ---- Compare
class Compare : public Other {
    protected:
        std::string _Condition;
        std::vector<Register*> _Operands;
        uint64_t _Flags;
        uint64_t _Result;
    public:
        // ---- Constructor
        Compare(            std::vector<Register*> Operands,
                            uint64_t Flags ) :
        _Operands(          Operands),
        _Flags(             Flags) { }
};

class ICmp : public Compare, public Integer {
    public:
        ICmp (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Compare (         RegOps,
                            Flags)
        , Integer (         ImmOp) { }
        void compute() override;
        void powerCycle() override;
};

class FCmp : public Compare, public FloatingPointSP, public FloatingPointDP {
    public:
        FCmp (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            double ImmOp)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , Compare (         RegOps,
                            Flags)
        , FloatingPointSP ( ImmOp) 
        , FloatingPointDP ( ImmOP) { }
        void compute() override;
        void powerCycle() override;
};
//---------------------------------------------------------------------------//
//--------- End Other Instruction Base --------------------------------------//
//---------------------------------------------------------------------------//


//-----------------------------------------------------//
//--------- Currently Unused --------------------------//
//-----------------------------------------------------//
//--------- Begin Vector Instruction Base -------------//
//-----------------------------------------------------//
class Vector : public InstructionBase {
    //-------------------------------------------------//
    //----- Begin protected -----------------------------//
    protected:




    //----- End protected -------------------------------//
    //-------------------------------------------------//
    //----- Begin Public ------------------------------//
    public:




    //----- End Public --------------------------------//
    //-------------------------------------------------//
};
//-----------------------------------------------------//
//--------- End Vector Instruction Base ---------------//
//-----------------------------------------------------//
//--------- Begin Aggregate Instruction Base ----------//
//-----------------------------------------------------//
class Aggregate : public InstructionBase {
    //-------------------------------------------------//
    //----- Begin protected -----------------------------//
    protected:




    //----- End protected -------------------------------//
    //-------------------------------------------------//
    //----- Begin Public ------------------------------//
    public:




    //----- End Public --------------------------------//
    //-------------------------------------------------//
};
//-----------------------------------------------------//
//--------- End Aggregate Instruction Base ------------//
//-----------------------------------------------------//


#endif //__INSTRUCTIONS_HH__