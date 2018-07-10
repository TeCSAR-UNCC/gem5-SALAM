#ifndef __UTILIZATION_HH__

#define __UTILIZATION_HH__

#include "debugFlags.hh" 
#include "debug/Hardware.hh"
#include "power_func.hh"
#include "instruction.hh"
#include <map>

class instructionUtilization {
     public:
      typedef std::map<std::string, int> Floats;
      typedef std::map<std::string, int> Integer;
      typedef std::map<std::string, int> Others;
      typedef std::map<std::string, int> opCodeCount;
      typedef std::map<std::string, int> BitCount;
      typedef std::map<std::string, int> ShiftCount;
    
    Floats floats;
    Integer integer;
    Others others;
    opCodeCount opCount;
    BitCount bitCount;
    ShiftCount shiftCount;
    instructionUtilization();
    void update(Instruction instr);
};

class PowerUtilization {
    private:
     float cycleTime;
     float internal_power;
     float switch_power;
     float leakage_power;
     float area;
     float max_leakage_power;
};

#endif //__UTILIZATION_HH__