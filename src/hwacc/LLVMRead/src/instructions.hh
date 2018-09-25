#ifndef __INSTRUCTIONS_HH__
#define __INSTRUCTIONS_HH__

#include "hwacc/comm_interface.hh"
#include "mem_request.hh"
#include "llvm_types.hh"
#include "debugFlags.hh"
#include "registers.hh"
#include "base_instruction.hh"
#include "power.hh"
#include <string>
#include <vector>

// counter type in parser 0 cycle


//--------- Begin Terminator Instruction Base -------------------------------//
//---------------------------------------------------------------------------//
class Terminator : public InstructionBase {
    public:
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
        Terminator (        const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            const std::string& Destination) // Unconditional Branch
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm)            
        , _Destination(     Destination) { 
                            _Terminator = true; 
                            Details("Terminator"); }
        Terminator (        const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            Register* Condition,
                            std::vector<std::string> Branches,
                            int8_t FunctionalUnit )  // Conditional Branches
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            FunctionalUnit)                    
        , _Condition(       Condition)
        , _Branches(        Branches) { 
                            _Terminator = true;
                            Details("Terminator"); }
        Terminator (        const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm) // Return Instruction
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) { 
                            _Terminator = true;
                            Details("Terminator"); }
        virtual ~Terminator() { Destruct("Terminator"); }
        virtual Terminator* clone() const { return new Terminator(*this); }
        virtual void compute() { }

};

class BadInstruction : public InstructionBase {
    // Used to draw hard dependencies, ie: ret
    public:
        BadInstruction(     const std::string& Line,
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
                            Comm) { 
                            Details("Bad Instruction"); }
        ~BadInstruction() { Destruct("Bad Instruction"); }
        void compute()      override { }
        virtual BadInstruction* clone() const { return new BadInstruction(*this); }
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
        : Terminator (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm, 
                            Destination)
        , _Unconditional(   Unconditional) {
                            Details("Br"); }
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
                            bool Unconditional,
                            int8_t FunctionalUnit)
        : Terminator (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm, 
                            Condition,
                            Branches,
                            FunctionalUnit)
        , _Unconditional(   Unconditional) {
                            Details("Br"); }
        ~Br()             { Destruct("Br"); }
        void compute()      override;
        virtual Br* clone() const { return new Br(*this); }
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
        : Terminator (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) { 
                            _Parents.push_back(new BadInstruction(Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm));
                            _ActiveParents++;
                            Details("Ret"); }
        ~Ret()            { Destruct("Ret"); }
        void compute()      override;
        virtual Ret* clone() const { return new Ret(*this); }
};

class LLVMSwitch : public Terminator {
    protected:
    // returnType is switch statement Int Type
    std::vector<int> _CaseValues;
    public:
        LLVMSwitch (            const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            Register* Condition,
                            std::vector<std::string> Branches,
                            std::vector<int> CaseValues,
                            int8_t FunctionalUnit)
        : Terminator (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            Condition,
                            Branches,
                            FunctionalUnit)
        , _CaseValues(      CaseValues) { 
                            Details("Switch"); 
                            for(int i = 0; i < _CaseValues.size(); i++) std::cout << _CaseValues.at(i) << "\n" ;
                            
                            
                            
                            
                            }
        ~LLVMSwitch()         { Destruct("Switch"); }
        void compute()      override;
        virtual LLVMSwitch* clone() const {  std::cout << "cloned\n"; return new LLVMSwitch(*this); }
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
        int64_t _Result;
        uint64_t _Flags;
        bool _ImmFirst;
    public:
        // ---- Constructor
        Binary(             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,     
                            std::vector<Register*> Operands,
                            uint64_t Flags,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            FunctionalUnit)
        , _Operands(        Operands)
        , _Flags(           Flags) 
        , _ImmFirst(        ImmFirst) { 
                            Details("Binary"); }
        virtual ~Binary() { Destruct("Binary"); }
        virtual void compute() { }
        virtual Binary* clone() const { return new Binary(*this); }
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
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Binary (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) { 
                            Details("Add"); }
        ~Add()            { Destruct("Add"); }
        void compute()      override;
        virtual Add* clone() const { return new Add(*this); }
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
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Binary (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) { 
                            Details("Sub"); }
        ~Sub()            { Destruct("Sub"); }
        void compute()      override;
        virtual Sub* clone() const { return new Sub(*this); }
};

class Mul : public Binary, public Integer {
    protected:
        
    public:
        Mul  (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Binary (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) { 
                            Details("Mul"); }
        ~Mul()            { Destruct("Mul"); }
        void compute()      override;
        virtual Mul* clone() const { return new Mul(*this); }
};

class UDiv : public Binary, public Integer, public Unsigned {
    protected:

    public:
        UDiv  (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Binary (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) 
        , Unsigned (        ImmOp) { 
                            Details("UDiv"); }
        ~UDiv()           { Destruct("UDiv"); }
        void compute()      override;
        virtual UDiv* clone() const { return new UDiv(*this); }
};

class SDiv : public Binary, public Integer, public Signed {
    protected:

    public:
    SDiv  (                 const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Binary (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) 
        , Signed (          ImmOp) { 
                            Details("SDiv"); }
        ~SDiv()           { Destruct("SDiv"); }
        void compute()      override;
        virtual SDiv* clone() const { return new SDiv(*this); }
};

class URem : public Binary, public Integer, public Unsigned {
    protected:

    public:
        URem  (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Binary (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) 
        , Unsigned (        ImmOp) { 
                            Details("URem"); }
        ~URem()           { Destruct("URem"); }
        void compute()      override;
        virtual URem* clone() const { return new URem(*this); }
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
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Binary (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) 
        , Signed (          ImmOp) { 
                            Details("SRem"); }
        ~SRem()           { Destruct("SRem"); }
        void compute()      override;
        virtual SRem* clone() const { return new SRem(*this); }
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
                            double ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Binary (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , FloatingPointSP ( ImmOp) 
        , FloatingPointDP ( ImmOp) {
                            Details("FAdd"); }
        ~FAdd()           { Destruct("FAdd"); }
        void compute()      override;
        virtual FAdd* clone() const { return new FAdd(*this); }
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
                            double ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Binary (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , FloatingPointSP ( ImmOp) 
        , FloatingPointDP ( ImmOp) { 
                            Details("FSub"); }
        ~FSub()           { Destruct("FSub"); }
        void compute()      override;
        virtual FSub* clone() const { return new FSub(*this); }
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
                            double ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Binary (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , FloatingPointSP ( ImmOp) 
        , FloatingPointDP ( ImmOp) { 
                            Details("FMul"); }
        ~FMul()           { Destruct("FMul"); }
        void compute()      override;
        virtual FMul* clone() const { return new FMul(*this); }
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
                            double ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Binary (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , FloatingPointSP ( ImmOp) 
        , FloatingPointDP ( ImmOp) { 
                            Details("FDiv"); }
        ~FDiv()           { Destruct("FDiv"); }
        void compute()      override;
        virtual FDiv* clone() const { return new FDiv(*this); }
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
                            double ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Binary (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , FloatingPointSP ( ImmOp) 
        , FloatingPointDP ( ImmOp) { 
                            Details("FRem"); }
        ~FRem()           { Destruct("FRem"); }
        void compute()      override;
        virtual FRem* clone() const { return new FRem(*this); }
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
        uint64_t _Flags;
        bool _ImmFirst;
   public:
        // ---- Constructor
        Bitwise(            const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,     
                            std::vector<Register*> Operands,
                            uint64_t Flags,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            FunctionalUnit)
        , _Operands(        Operands)
        , _Flags(           Flags)
        , _ImmFirst(        ImmFirst) { 
                            Details("Bitwise"); }
        virtual ~Bitwise(){ Destruct("Bitwise"); }
        virtual void compute() { }
        virtual Bitwise* clone() const { return new Bitwise(*this); }
};
// ---- Bitwise Instructions

class Shl : public Bitwise, public Integer {
    protected:

    public:
        Shl (               const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Bitwise (         Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) { 
                            Details("Shl"); }
        ~Shl()            { Destruct("Shl"); }
        void compute()      override;
        virtual Shl* clone() const { return new Shl(*this); }
};

class LShr : public Bitwise, public Integer {
    protected:

    public:
        LShr (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Bitwise (         Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) { 
                            Details("LShr"); }
        ~LShr()           { Destruct("LShr"); }
        void compute()      override;
        virtual LShr* clone() const { return new LShr(*this); }
};

class AShr : public Bitwise, public Integer {
    protected:

    public:
        AShr (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Bitwise (         Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) { 
                            Details("AShr"); }
        ~AShr()           { Destruct("AShr"); }
        void compute()      override;
        virtual AShr* clone() const { return new AShr(*this); }
};

class And : public Bitwise, public Integer {
    protected:

    public:
        And  (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Bitwise (         Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) { 
                            Details("And"); }
        ~And()            { Destruct("And"); }
        void compute()      override;
        virtual And* clone() const { return new And(*this); }
};

class Or : public Bitwise, public Integer {
    protected:

    public:
        Or (                const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Bitwise (         Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) { 
                            Details("Or"); }
        ~Or()             { Destruct("Or"); }
        void compute()      override;
        virtual Or* clone() const { return new Or(*this); }
};

class Xor : public Bitwise, public Integer {
    protected:

    public:
        Xor (               const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<Register*> RegOps,
                            uint64_t Flags,
                            int64_t ImmOp,
                            bool ImmFirst,
                            int8_t FunctionalUnit)
        : Bitwise (         Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            ImmFirst,
                            FunctionalUnit)
        , Integer (         ImmOp) { 
                            Details("Xor"); }
        ~Xor()            { Destruct("Xor"); }
        void compute()      override;
        virtual Xor* clone() const { return new Xor(*this); }
};

//---------------------------------------------------------------------------//
//--------- End Bitwise Instruction Base ------------------------------------//
//---------------------------------------------------------------------------//
//--------- Begin Conversion Instruction Base -------------------------------//
//---------------------------------------------------------------------------//
class Conversion : public InstructionBase {
    protected:
        std::string _OriginalType;
        Register* _COperand;
        uint64_t _Result;

    public:
        // ---- Constructor
        Conversion(         const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,         
                            const std::string& OriginalType,
                            Register* Operand,
                            int8_t FunctionalUnit)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            FunctionalUnit) 
        , _OriginalType(    OriginalType)
        , _COperand(        Operand) { 
                            Details("Conversion"); }
        virtual ~Conversion() { Destruct("Conversion"); }
        virtual void compute() { }
        virtual Conversion* clone() const { return new Conversion(*this); }
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
                            const std::string& OriginalType,
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) { 
                            Details("Trunc"); }
        ~Trunc()          { Destruct("Trunc"); }
        void compute()      override;  
        virtual Trunc* clone() const { return new Trunc(*this); } 
};

class ZExt : public Conversion {
    public:
        ZExt (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) { 
                            Details("ZExt"); }
        ~ZExt()           { Destruct("ZExt"); }
        void compute()      override;
        virtual ZExt* clone() const { return new ZExt(*this); }
};

class SExt : public Conversion {
    public:
        SExt (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) { 
                            Details("SExt"); }
        ~SExt()           { Destruct("SExt"); }
        void compute()      override;
        virtual SExt* clone() const { return new SExt(*this); }
};

class FPToUI : public Conversion {
    public:
        FPToUI (            const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) {
                            Details("FPToUI"); }
        ~FPToUI()         { Destruct("FPToUI"); }
        void compute()      override;
        virtual FPToUI* clone() const { return new FPToUI(*this); }
};

class FPToSI : public Conversion {
    public:
        FPToSI (            const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) { 
                            Details("FPToSI"); }
        ~FPToSI()         { Destruct("FPToSI"); }
        void compute()      override;
        virtual FPToSI* clone() const { return new FPToSI(*this); }
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
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) { 
                            Details("UIToFP"); }
        ~UIToFP()         { Destruct("UIToFP"); }
        void compute()      override;
        virtual UIToFP* clone() const { return new UIToFP(*this); }
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
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) { 
                            Details("SIToFP"); }
        ~SIToFP()         { Destruct("SIToFP"); }
        void compute()      override;
        virtual SIToFP* clone() const { return new SIToFP(*this); }
};

class FPTrunc : public Conversion {
    public:
        FPTrunc (           const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string& OriginalType,
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) { 
                            Details("FPTrunc"); }
        ~FPTrunc()        { Destruct("FPTrunc"); }
        void compute()      override;
        virtual FPTrunc* clone() const { return new FPTrunc(*this); }
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
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) { 
                            Details("FPExt"); }
        ~FPExt()          { Destruct("FPExt"); }
        void compute()      override;
        virtual FPExt* clone() const { return new FPExt(*this); }
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
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) { 
                            Details("PtrToInt"); }
        ~PtrToInt()       { Destruct("PtrToInt"); }
        void compute()      override;
        virtual PtrToInt* clone() const { return new PtrToInt(*this); }
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
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) { 
                            Details("IntToPtr"); }
        ~IntToPtr()       { Destruct("IntToPtr"); }
        void compute()      override;
        virtual IntToPtr* clone() const { return new IntToPtr(*this); }
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
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) { 
                            Details("BitCast"); }
        ~BitCast()        { Destruct("BitCast"); }
        void compute()      override;
        virtual BitCast* clone() const { return new BitCast(*this); }
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
                            Register* Operand,
                            int8_t FunctionalUnit)
        : Conversion (      Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            OriginalType,
                            Operand,
                            FunctionalUnit) {
                            Details("AddrSpaceCast"); }
        ~AddrSpaceCast()  { Destruct("AddrSpaceCast"); }
        void compute()      override;
        virtual AddrSpaceCast* clone() const { return new AddrSpaceCast(*this); }
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
        Memory (            const std::string& Line,
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
                            Comm) {
                            Details("Memory"); }
        Memory (            const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            uint8_t FunctionalUnit)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            FunctionalUnit) {
                            Details("Memory"); }
        virtual ~Memory()   { Destruct("Memory"); }
        virtual Memory* clone() const { return new Memory(*this); }
        virtual void compute () { }
};

class Load : public Memory {
    protected:
        uint64_t _Align;
        Register* _Pointer;
    public:
        Load (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            uint64_t Align,
                            Register* Pointer)
        : Memory (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm) 
        , _Align(           Align)
        , _Pointer(         Pointer) { 
                            Details("Load"); }
        ~Load()           { Destruct("Load"); }
        void compute()      override;
        virtual Load* clone() const { return new Load(*this); }
};

class Store : public Memory {
    protected:
        uint64_t _Align;
        uint64_t _Imm;
        Register* _Pointer;
        Register* _Value;
        bool _ImmVal = false;
    public:
        Store (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            uint64_t Align,
                            uint64_t Imm,
                            Register* Pointer,
                            Register* Value,
                            bool ImmVal)
        : Memory (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm)
        , _Align(           Align)
        , _Imm(             Imm)
        , _Pointer(         Pointer)
        , _Value(           Value) 
        , _ImmVal(          ImmVal) { 
                            Details("Store"); } 
        ~Store()          { Destruct("Store"); }
        void compute()      override;
        virtual Store* clone() const { return new Store(*this); }
};

class GetElementPtr : public Memory {
    public:
        std::string _Pty;
        LLVMType* _LLVMType;
        std::vector<Register*> _Idx;
        std::vector<std::string> _Type;
        std::vector<int64_t> _ImmIdx;
        Register* _PtrVal;
        uint64_t _ActivePtr;
        uint64_t _Index;

    public:
        GetElementPtr (     const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::string Pty,
                            LLVMType* LLVMType,
                            std::vector<Register*> Idx,
                            std::vector<std::string> Type,
                            std::vector<int64_t> ImmIdx,
                            Register* PtrVal,
                            uint64_t Index,
                            uint8_t FunctionalUnit)
        : Memory (          Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            FunctionalUnit)
        , _Pty(             Pty)
        , _LLVMType(        LLVMType)
        , _Idx(             Idx)
        , _Type(            Type)
        , _ImmIdx(          ImmIdx)
        , _PtrVal(          PtrVal)
        , _Index(           Index) { 
                            Details("GetElementPtr"); }
        ~GetElementPtr()  { Destruct("GetElementPtr"); }
        void compute()      override; 
        virtual GetElementPtr* clone() const { return new GetElementPtr(*this); }
};
//---------------------------------------------------------------------------//
//--------- End Memory Instruction Base -------------------------------------//
//---------------------------------------------------------------------------//
//--------- Begin Other Instruction Base ------------------------------------//
//---------------------------------------------------------------------------//
class Other : public InstructionBase {
    protected:


    public:
        Other (             const std::string& Line,
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
                            Comm) { 
                            Details("Other"); }
        Other (             const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            uint8_t FunctionalUnit)
        : InstructionBase ( Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            FunctionalUnit) { 
                            Details("Other"); }
        virtual ~Other()  { Destruct("Other"); }
        virtual Other* clone() const { return new Other(*this); }
        virtual void compute () { }
};

class Phi : public Other {
    protected:
        std::vector<std::string> _PhiVal; // Value to be loaded
        std::vector<Register*> _PhiReg;
		std::vector<std::string> _PhiLabel; // If from this BB
        int64_t _Result;
    public:
         Phi (              const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,
                            std::vector<std::string> PhiVal,
                            std::vector<Register*> PhiReg,
                            std::vector<std::string> PhiLabel,
                            int8_t FunctionalUnit)
        : Other (           Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            FunctionalUnit) 
        , _PhiVal(          PhiVal)
        , _PhiReg(          PhiReg)
        , _PhiLabel(        PhiLabel) { 
                            Details("Phi"); }
        ~Phi()            { Destruct("Phi"); }
        void compute()      override;
        virtual Phi* clone() const { return new Phi(*this); }
        std::vector<Register*> runtimeDependencies(std::string PrevBB) override;
};

class Select : public Other {
    protected:
        Register* _Condition;
        std::vector<Register*> _RegValues;
		std::vector<int64_t> _ImmValues;
        std::vector<bool> _Imm;
        int64_t _Result;
    public:
        // ---- Constructor
        Select (            const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,            
                            Register* Condition,
                            std::vector<Register*> RegValues,
		                    std::vector<int64_t> ImmValues,
                            std::vector<bool> Imm ,
                            int8_t FunctionalUnit)
        : Other (           Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            FunctionalUnit)
        , _Condition (      Condition)
        , _RegValues (      RegValues) 
        , _ImmValues (      ImmValues)
        , _Imm (            Imm) { 
                            Details("Select"); }
        ~Select()         { Destruct("Select"); }
        void compute()      override;
        virtual Select* clone() const { return new Select(*this); }
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
        Compare (           const std::string& Line,
                            const std::string& OpCode,
                            const std::string& ReturnType,
                            const std::string& InstructionType,
                            Register* ReturnRegister,
                            uint64_t MaxCycle,
                            std::vector<Register*> Dependencies,
                            CommInterface* Comm,            
                            std::vector<Register*> Operands,
                            uint64_t Flags ,
                            int8_t FunctionalUnit)
        : Other (           Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            FunctionalUnit)
        , _Operands (       Operands)
        , _Flags (          Flags) { 
                            Details("Compare"); }
        virtual ~Compare(){ Destruct("Compare"); }
        virtual Compare* clone() const { return new Compare(*this); }
        virtual void compute() { }
};

class ICmp : public Compare, public Integer, public Unsigned, public Signed {
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
                            int64_t ImmOp,
                            int8_t FunctionalUnit)
        : Compare (         Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            FunctionalUnit)
        , Integer (         ImmOp) 
        , Unsigned(         ImmOp)
        , Signed(           ImmOp) { 
                            Details("ICmp"); }
        ~ICmp()           { Destruct("ICmp"); }
        void compute()      override;
        virtual ICmp* clone() const { return new ICmp(*this); }
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
                            double ImmOp,
                            int8_t FunctionalUnit)
        : Compare (         Line, 
                            OpCode, 
                            ReturnType, 
                            InstructionType, 
                            ReturnRegister, 
                            MaxCycle, 
                            Dependencies, 
                            Comm,
                            RegOps,
                            Flags,
                            FunctionalUnit)
        , FloatingPointSP ( ImmOp) 
        , FloatingPointDP ( ImmOp) { 
                            Details("FCmp"); }
        ~FCmp()           { Destruct("FCmp"); }
        void compute()      override;
        virtual FCmp* clone() const { return new FCmp(*this); }
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
