#ifndef INSTRUCTIONS_HH
#define INSTRUCTIONS_HH
//------------------------------------------//
#include "hwacc/comm_interface.hh"
#include "mem_request.hh"
#include "llvm_types.hh"
#include "debug_flags.hh"
#include "registers.hh"
#include "base_instruction.hh"
//#include "utilization.hh"
//------------------------------------------//
#include <string>
#include <vector>
//------------------------------------------//

class InstructionBase;

//---------------------------------------------------------------------------//
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
        std::shared_ptr<Terminator> clone() const { return std::static_pointer_cast<Terminator>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Terminator>(new Terminator(*this)); }
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
        std::shared_ptr<BadInstruction> clone() const { return std::static_pointer_cast<BadInstruction>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<BadInstruction>(new BadInstruction(*this)); }
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
        std::shared_ptr<Br> clone() const { return std::static_pointer_cast<Br>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Br>(new Br(*this)); }
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
                            // _Parents.push_back(new BadInstruction(Line,
                            // OpCode,
                            // ReturnType,
                            // InstructionType,
                            // ReturnRegister,
                            // MaxCycle,
                            // Dependencies,
                            // Comm));
                            _ActiveParents++;
                            Details("Ret"); }
        ~Ret()            { Destruct("Ret"); }
        void compute()      override;
        std::shared_ptr<Ret> clone() const { return std::static_pointer_cast<Ret>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Ret>(new Ret(*this)); }
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
                            #ifdef ClassDetail
                                for(int i = 0; i < _CaseValues.size(); i++) std::cout << _CaseValues.at(i) << "\n";
                            #endif




                            }
        ~LLVMSwitch()         { Destruct("Switch"); }
        void compute()      override;
        std::shared_ptr<LLVMSwitch> clone() const { return std::static_pointer_cast<LLVMSwitch>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<LLVMSwitch>(new LLVMSwitch(*this)); }
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
        std::shared_ptr<Binary> clone() const { return std::static_pointer_cast<Binary>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Binary>(new Binary(*this)); }
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
        std::shared_ptr<Add> clone() const { return std::static_pointer_cast<Add>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Add>(new Add(*this)); }
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
        std::shared_ptr<Sub> clone() const { return std::static_pointer_cast<Sub>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Sub>(new Sub(*this)); }
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
        std::shared_ptr<Mul> clone() const { return std::static_pointer_cast<Mul>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Mul>(new Mul(*this)); }
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
        std::shared_ptr<UDiv> clone() const { return std::static_pointer_cast<UDiv>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<UDiv>(new UDiv(*this)); }
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
        std::shared_ptr<SDiv> clone() const { return std::static_pointer_cast<SDiv>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<SDiv>(new SDiv(*this)); }
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
        std::shared_ptr<URem> clone() const { return std::static_pointer_cast<URem>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<URem>(new URem(*this)); }
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
        std::shared_ptr<SRem> clone() const { return std::static_pointer_cast<SRem>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<SRem>(new SRem(*this)); }
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
        std::shared_ptr<FAdd> clone() const { return std::static_pointer_cast<FAdd>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<FAdd>(new FAdd(*this)); }
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
        std::shared_ptr<FSub> clone() const { return std::static_pointer_cast<FSub>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<FSub>(new FSub(*this)); }
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
        std::shared_ptr<FMul> clone() const { return std::static_pointer_cast<FMul>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<FMul>(new FMul(*this)); }
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
        std::shared_ptr<FDiv> clone() const { return std::static_pointer_cast<FDiv>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<FDiv>(new FDiv(*this)); }
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
        std::shared_ptr<FRem> clone() const { return std::static_pointer_cast<FRem>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<FRem>(new FRem(*this)); }
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
        std::shared_ptr<Bitwise> clone() const { return std::static_pointer_cast<Bitwise>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Bitwise>(new Bitwise(*this)); }
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
        std::shared_ptr<Shl> clone() const { return std::static_pointer_cast<Shl>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Shl>(new Shl(*this)); }
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
        std::shared_ptr<LShr> clone() const { return std::static_pointer_cast<LShr>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<LShr>(new LShr(*this)); }
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
        std::shared_ptr<AShr> clone() const { return std::static_pointer_cast<AShr>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<AShr>(new AShr(*this)); }
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
        std::shared_ptr<And> clone() const { return std::static_pointer_cast<And>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<And>(new And(*this)); }
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
        std::shared_ptr<Or> clone() const { return std::static_pointer_cast<Or>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Or>(new Or(*this)); }
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
        std::shared_ptr<Xor> clone() const { return std::static_pointer_cast<Xor>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Xor>(new Xor(*this)); }
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
        std::shared_ptr<Conversion> clone() const { return std::static_pointer_cast<Conversion>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Conversion>(new Conversion(*this)); }
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
        std::shared_ptr<Trunc> clone() const { return std::static_pointer_cast<Trunc>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Trunc>(new Trunc(*this)); }
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
        std::shared_ptr<ZExt> clone() const { return std::static_pointer_cast<ZExt>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<ZExt>(new ZExt(*this)); }
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
        std::shared_ptr<SExt> clone() const { return std::static_pointer_cast<SExt>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<SExt>(new SExt(*this)); }
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
        std::shared_ptr<FPToUI> clone() const { return std::static_pointer_cast<FPToUI>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<FPToUI>(new FPToUI(*this)); }
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
        std::shared_ptr<FPToSI> clone() const { return std::static_pointer_cast<FPToSI>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<FPToSI>(new FPToSI(*this)); }
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
        std::shared_ptr<UIToFP> clone() const { return std::static_pointer_cast<UIToFP>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<UIToFP>(new UIToFP(*this)); }
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
        std::shared_ptr<SIToFP> clone() const { return std::static_pointer_cast<SIToFP>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<SIToFP>(new SIToFP(*this)); }
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
        std::shared_ptr<FPTrunc> clone() const { return std::static_pointer_cast<FPTrunc>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<FPTrunc>(new FPTrunc(*this)); }
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
        std::shared_ptr<FPExt> clone() const { return std::static_pointer_cast<FPExt>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<FPExt>(new FPExt(*this)); }
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
        std::shared_ptr<PtrToInt> clone() const { return std::static_pointer_cast<PtrToInt>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<PtrToInt>(new PtrToInt(*this)); }
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
        std::shared_ptr<IntToPtr> clone() const { return std::static_pointer_cast<IntToPtr>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<IntToPtr>(new IntToPtr(*this)); }
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
        std::shared_ptr<BitCast> clone() const { return std::static_pointer_cast<BitCast>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<BitCast>(new BitCast(*this)); }
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
        std::shared_ptr<AddrSpaceCast> clone() const { return std::static_pointer_cast<AddrSpaceCast>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<AddrSpaceCast>(new AddrSpaceCast(*this)); }
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
        std::shared_ptr<Memory> clone() const { return std::static_pointer_cast<Memory>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Memory>(new Memory(*this)); }
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
                            Details("Load");
                            _RawCheck = _Pointer;}
        ~Load()           { Destruct("Load"); }
        void compute()      override;
        std::shared_ptr<Load> clone() const { return std::static_pointer_cast<Load>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Load>(new Load(*this)); }
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
                            Details("Store");
                            _RawCheck = _Pointer;}
        ~Store()          { Destruct("Store"); }
        void compute()      override;
        std::shared_ptr<Store> clone() const { return std::static_pointer_cast<Store>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Store>(new Store(*this)); }
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
        bool _Global;

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
                            uint8_t FunctionalUnit,
                            bool Global)
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
        , _Index(           Index)
        , _Global(          Global) {
                            Details("GetElementPtr"); }
        ~GetElementPtr()  { Destruct("GetElementPtr"); }
        void compute()      override;
        bool isGlobal()     override {return _Global; }
        std::shared_ptr<GetElementPtr> clone() const { return std::static_pointer_cast<GetElementPtr>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<GetElementPtr>(new GetElementPtr(*this)); }
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
        std::shared_ptr<Other> clone() const { return std::static_pointer_cast<Other>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Other>(new Other(*this)); }
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
        std::shared_ptr<Phi> clone() const { return std::static_pointer_cast<Phi>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Phi>(new Phi(*this)); }
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
        std::shared_ptr<Select> clone() const { return std::static_pointer_cast<Select>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Select>(new Select(*this)); }
};
// ---- Other Sub Type ---- Compare
class Compare : public Other {
    protected:
        std::string _Condition;
        std::vector<Register*> _Operands;
        uint64_t _Flags;
        int64_t _Result;
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
        std::shared_ptr<Compare> clone() const { return std::static_pointer_cast<Compare>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<Compare>(new Compare(*this)); }
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
        std::shared_ptr<ICmp> clone() const { return std::static_pointer_cast<ICmp>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<ICmp>(new ICmp(*this)); }
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
        std::shared_ptr<FCmp> clone() const { return std::static_pointer_cast<FCmp>(createClone()); }
        virtual std::shared_ptr<InstructionBase> createClone() const override { return std::shared_ptr<FCmp>(new FCmp(*this)); }
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
