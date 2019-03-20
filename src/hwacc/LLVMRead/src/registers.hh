#ifndef __REGISTERS_HH__
#define __REGISTERS_HH__
#include <iostream>
#include <cstdint>
#include <cstring>
#include <string>
#include <list>
#include "power.hh"
#include "macros.hh"
#include "debugFlags.hh"

class Register {
    //-----------------------------------------------------------------------//
    //----- Begin Private ---------------------------------------------------//
    private:
        std::string _Name;      // Name of reg in reg list
        std::string _Data_Type; // Data type to be stored in reg
        uint64_t _Value = 0;    // Register value (reflects memory)
        uint64_t _Size;         // Reg size in bytes
        bool _Hot;              // Register is ready/not ready

    //----- End Private -----------------------------------------------------//
    //-----------------------------------------------------------------------//
    //----- Begin Public ----------------------------------------------------//
    public:
            Reg_Usage _Reg_Usage;   // Read/Write Usage
        // ---- Constructor
        Register(const std::string& Name, uint64_t Value):
                _Name(Name),
                _Data_Type("NA"),
                _Value(Value),
                _Size(8),
                _Hot(false) { }
        // ---- Constructor
        Register(const std::string& Name): 
                _Name(Name),
                _Data_Type("NA"),
                _Value(0),
                _Size(8),
                _Hot(false) { }
        // ---- Get Functions
        std::string getName()           { return _Name; }
        std::string getType()           { return _Data_Type; }
        Reg_Usage* usage()              { return &_Reg_Usage; }
        uint64_t getValue()             { return _Value; }
        uint64_t getSize()              { return _Size; }
        bool getStatus()                { return _Hot; }
        // ---- Set Functions
        void commit()                   { _Hot = false; _Reg_Usage.writes++;}
        void reset()                    { _Hot = true; }
        void setSize(const std::string& Data_Type); 
        void setValue(void *data);
        // ---- Increment Functions
        void write()                    { _Reg_Usage.writes++; }
        void read()                     { _Reg_Usage.reads++; }     
    //----- End Public ------------------------------------------------------//
    //-----------------------------------------------------------------------//
};


class RegisterList{
    //-----------------------------------------------------------------------//
    //----- Begin Private ---------------------------------------------------//
    private:  
        std::list<Register*> *_RegList; // List of all registers

    //----- End Private -----------------------------------------------------//
    //-----------------------------------------------------------------------//
    //----- Begin Public ----------------------------------------------------//
    public:
        // ---- Constructor
        RegisterList()                  { _RegList = new std::list<Register*>(); }
        // ---- Get Functions
        uint64_t size()                 { return _RegList->size(); }
        // ---- Helper Functions
        Register* findRegister(std::string Name);
        void addRegister(Register *Reg) { _RegList->push_back(Reg); }
        void printRegNames();
        void totalAccess(Reg_Usage *regUsage);
    //----- End Public ------------------------------------------------------//
    //-----------------------------------------------------------------------//
};

#endif //__REGISTERS_HH__