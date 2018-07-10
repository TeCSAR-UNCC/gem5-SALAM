#include "utilization.hh"

instructionUtilization::instructionUtilization() {

}

 void 
 instructionUtilization::update(Instruction instr) {
     if(instr.general.binary){
        if(instr.binary.ty == "double") {
            floats[instr.general.opCode]++;
        } else if(instr.binary.ty == "float") {
            floats[instr.general.opCode]++;
        } else if(instr.binary.ty[0] == 'i') {
            integer[instr.general.opCode]++;
        } else {
            others[instr.general.opCode]++;
        }
     } else if(instr.general.bitwise){
         if(instr.general.opCode == "or" || instr.general.opCode == "and" || instr.general.opCode == "xor") {
            bitCount[instr.general.opCode]++; 
         }
         else shiftCount[instr.general.opCode]++;
     } else {
         opCount[instr.general.opCode]++;
     }
 }
