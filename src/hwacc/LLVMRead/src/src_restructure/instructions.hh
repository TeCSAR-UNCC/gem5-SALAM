#ifndef __INSTRUCTIONS_HH__

#define __INSTRUCTIONS_HH__

#include "power.hh"
#include "registers.hh"
#include <string>
#include <vector>



//---------------------------------------------------------------------------//
//--------- Begin Shared Instruction Base -----------------------------------//
//---------------------------------------------------------------------------//
class InstructionBase {
    private:
        std::string _LLVMLine;
        std::string _OpCode;
        std::string _InstrType;
        uint64_t _Usage;
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
        virtual void compute()           = 0;  
        virtual bool commit()            = 0;
        virtual bool checkDependencies() = 0;
        virtual bool powerCycle(); 
        // ---- Hardware Usage Functions
        void used() { _Usage++; }
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


    public:


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
//---------------------------------------------------------------------------//
//--------- End Other Instruction Base --------------------------------------//
//---------------------------------------------------------------------------//

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

// ---- Binary ---- Integer

class Add : public Binary, public Integer {

};
class Sub : public Binary, public Integer {

};
class Mul : public Binary, public Integer {

};
class UDiv : public Binary, public Integer, public Unsigned {

};
class SDiv : public Binary, public Integer, public Signed {

};
class URem : public Binary, public Integer, public Unsigned {

};
class SRem : public Binary, public Integer, public Signed {

};

// ---- Binary ---- Floating Point

class FAdd : public Binary, public FloatingPointSP, public FloatingPointDP {

};

class FSub : public Binary, public FloatingPointSP, public FloatingPointDP {

};

class FMul : public Binary, public FloatingPointSP, public FloatingPointDP {

};

class FDiv : public Binary, public FloatingPointSP, public FloatingPointDP {

};

class FRem : public Binary, public FloatingPointSP, public FloatingPointDP {

};

// ---- Bitwise

class Shl : public Bitwise, public Integer {

};

class LShr : public Bitwise, public Integer {

};

class AShr : public Bitwise, public Integer {

};

class And : public Bitwise, public Integer {

};

class Or : public Bitwise, public Integer {

};

class Xor : public Bitwise, public Integer {

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