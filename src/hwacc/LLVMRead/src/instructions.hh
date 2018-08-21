#ifndef __INSTRUCTIONS_HH__
#define __INSTRUCTIONS_HH__
#include "hwacc/comm_interface.hh"
#include "mem_request.hh"
#include "debugFlags.hh"
#include "registers.hh"
#include "power.hh"
#include <string>
#include <vector>

// predecessors (sizeof(dependencies + returnregprevist))
// sucessors ()
// status (sizeof(dependencies + returnregprevist))
// signalChildren
// registerChild


// counter type in parser 0 cycle


//---------------------------------------------------------------------------//
//--------- Begin Immediate Value Sub Classes -------------------------------//
//---------------------------------------------------------------------------//
class Signed {
    private: 
        int64_t _Val;
        
    public:
        void setSigned(uint64_t Val) {_Val = (int64_t) Val; }
        int64_t getSigned() { return _Val; }
};
class Unsigned {
    private:
        uint64_t _Val;
    public:
        void setUnsigned(int64_t Val) {_Val = (uint64_t) Val; }
        int64_t getUnsigned() { return _Val; }
};
class Integer {
    private:
        int64_t _Operand;
        uint64_t _Size;
    public:
        // ---- Constructor
        Integer(int64_t Operand, uint64_t newSize): 
                    _Operand(Operand), 
                    _Size(newSize) { }
        // ---- Get Functions
        int64_t getOperand()           { return _Operand; }
        uint64_t getSize()             { return _Size; }
};
class FloatingPointSP {
    private:
        float _Operand;
    public:
        // ---- Constructor
        FloatingPointSP(float Operand): 
                    _Operand(Operand) { }
        // ---- Get Functions
        float getOperand()           { return _Operand; }
};
class FloatingPointDP {
    private:
        double _Operand;
    public:
        // ---- Constructor
        FloatingPointDP(double Operand): 
                    _Operand(Operand) { }
        // ---- Get Functions
        double getOperand()           { return _Operand; }
};
//---------------------------------------------------------------------------//
//--------- End Immediate Value Sub Classes ---------------------------------//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//--------- Begin Shared Instruction Base -----------------------------------//
//---------------------------------------------------------------------------//
class InstructionBase {
    private:
        CommInterface* _Comm; // Pointer to add basic block to queues 
        MemoryRequest* _Req; // Pointer for creating a memory access request
        std::string _LLVMLine;
        std::string _OpCode;
        std::string _InstrType; // Return Type
        std::vector<InstructionBase*> _Parents; // Parent Nodes
        std::vector<InstructionBase*> _Children; // Child Nodes
        std::vector<bool> _Status; // Ready Indicator, Index Matched To Parent
        uint64_t _Usage; // Counter for times instruction used
        uint64_t _MaxCycle;
        uint64_t _CurrCycle;
    public:
        // ---- Constructor
        InstructionBase( const std::string& LLVMLine,
                         const std::string& OpCode,
                         const std::string& InstrType):
                         _LLVMLine(LLVMLine),
                         _OpCode(OpCode), 
                         _InstrType(InstrType) {}
        // ---- Get Functions
        std::string getLLVMLine()      { return _LLVMLine; }
        std::string getOpCode()        { return _OpCode; }
        std::string getInstrType()     { return _InstrType; }
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
    private:
    // ---- Constructor

    public:

};

class Ret : public Terminator {
    private:

    public:

};
class Br : public Terminator {
    private:

    public:
    
};
class Switch : public Terminator {
    private:

    public:
    
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
    private:
       Register2* _ReturnRegister;
       std::vector<Register2*> _Dependencies;
       std::vector<uint64_t> _Operands;
       std::vector<uint64_t> _Flags;
       Pwr_Parameters _PwrParams;  
       uint64_t _Result;
       uint64_t _Usage;
       uint64_t _HardwareUnit;
       uint64_t _ReturnType;    
    public:
        // ---- Constructor
        Binary(    uint64_t ReturnType,
                   Register2* Op1,
                   uint64_t HardwareUnit,
                   uint64_t Flags);
        // ---- Constructor
        Binary(    uint64_t ReturnType,
                   Register2* Op1,
                   Register2* Op2,
                   uint64_t HardwareUnit,
                   uint64_t Flags);
};

//---------------------------------------------------------------------------//
//--------- End Binary Instruction Base -------------------------------------//
//---------------------------------------------------------------------------//
//--------- Begin Bitwise Instruction Base ----------------------------------//
//---------------------------------------------------------------------------//
class Bitwise : public InstructionBase {
private:
        Register2* _ReturnRegister;
        std::vector<Register2*> _Dependencies;
        std::vector<uint64_t> _Flags;
        Pwr_Parameters _PwrParams; 
        uint64_t _Result;
        uint64_t _Usage;
        uint64_t _HardwareUnit;
        uint64_t ReturnType;
   public:
        // ---- Constructor
        Bitwise(    uint64_t ReturnType,
                    Register2* Op1,
                    uint64_t HardwareUnit,
                    uint64_t Flags);
        // ---- Constructor
        Bitwise(    uint64_t ReturnType,
                    Register2* Op1,
                    Register2* Op2,
                    uint64_t HardwareUnit,
                    uint64_t Flags);
};
//---------------------------------------------------------------------------//
//--------- End Bitwise Instruction Base ------------------------------------//
//---------------------------------------------------------------------------//
//--------- Begin Conversion Instruction Base -------------------------------//
//---------------------------------------------------------------------------//
class Conversion : public InstructionBase {
    private:
        Register2* _ReturnRegister;
        std::vector<Register2*> _Dependencies;
        Pwr_Parameters _PwrParams;
        uint64_t _OriginalType;
        uint64_t _NewType;
        uint64_t _Size;
        uint64_t _Result;
        uint64_t _Usage;
        uint64_t _HardwareUnit;

    public:
        // ---- Constructor
        Conversion( uint64_t OriginalType,
                    uint64_t NewType,
                    uint64_t NewSize);
};
//---------------------------------------------------------------------------//
//--------- End Conversion Instruction Base ---------------------------------//
//---------------------------------------------------------------------------//
//--------- Begin Memory Instruction Base -----------------------------------//
//---------------------------------------------------------------------------//
class Memory : public InstructionBase {
    private:
        // Address
        // alignment

    public:


};

class Load : public Memory {

};

class Store : public Memory {

};

class GetElementPtr : public Memory {

};
//---------------------------------------------------------------------------//
//--------- End Memory Instruction Base -------------------------------------//
//---------------------------------------------------------------------------//
//--------- Begin Other Instruction Base ------------------------------------//
//---------------------------------------------------------------------------//
class Other : public InstructionBase {
    private:


    public:

};
class Compare : public Other {
private:
        Register2* _ReturnRegister;
        std::vector<Register2*> _Dependencies;
        std::vector<uint64_t> _Flags;
        Pwr_Parameters _PwrParams; 
        uint64_t _Result;
        uint64_t _Usage;
        uint64_t _HardwareUnit;
        uint64_t ReturnType;
   public:
        // ---- Constructor
        Compare(    uint64_t ReturnType,
                    Register2* Op1,
                    uint64_t HardwareUnit,
                    uint64_t Flags);
        // ---- Constructor
        Compare(    uint64_t ReturnType,
                    Register2* Op1,
                    Register2* Op2,
                    uint64_t HardwareUnit,
                    uint64_t Flags);
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
};
//---------------------------------------------------------------------------//
//--------- End Other Instruction Base --------------------------------------//
//---------------------------------------------------------------------------//


// ---- Binary ---- Integer

class Add : public Binary, public Integer {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};
class Sub : public Binary, public Integer {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};
class Mul : public Binary, public Integer {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};
class UDiv : public Binary, public Integer, public Unsigned {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};
class SDiv : public Binary, public Integer, public Signed {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};
class URem : public Binary, public Integer, public Unsigned {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};
class SRem : public Binary, public Integer, public Signed {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};

// ---- Binary ---- Floating Point

class FAdd : public Binary, public FloatingPointSP, public FloatingPointDP {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};

class FSub : public Binary, public FloatingPointSP, public FloatingPointDP {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};

class FMul : public Binary, public FloatingPointSP, public FloatingPointDP {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};

class FDiv : public Binary, public FloatingPointSP, public FloatingPointDP {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};

class FRem : public Binary, public FloatingPointSP, public FloatingPointDP {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};

// ---- Bitwise

class Shl : public Bitwise, public Integer {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};

class LShr : public Bitwise, public Integer {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};

class AShr : public Bitwise, public Integer {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};

class And : public Bitwise, public Integer {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};

class Or : public Bitwise, public Integer {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};

class Xor : public Bitwise, public Integer {
    private:

    public:
        void compute() override;
        void powerCycle() override;
};

// ---- 


//-----------------------------------------------------//
//--------- Currently Unused --------------------------//
//-----------------------------------------------------//
//--------- Begin Vector Instruction Base -------------//
//-----------------------------------------------------//
class Vector : public InstructionBase {
    //-------------------------------------------------//
    //----- Begin Private -----------------------------//
    private:




    //----- End Private -------------------------------//
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
    //----- Begin Private -----------------------------//
    private:




    //----- End Private -------------------------------//
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