//------------------------------------------//
#include "utilization.hh"
//#include "algorithm"
//------------------------------------------//


Hardware::Hardware(int Latency, int Pipelined) {

    // Initialized in the same order they are defined in macros.hh
    // Do not change the order.
    uint8_t stages = 1;
    // Counters 
    if(Pipelined) stages = COUNTER_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, COUNTER, stages, "Counters"));
    // Integer Addition
    if(Pipelined) stages = INTADDER_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, INTADDER, stages, "Integer Adder"));
    // Integer Multiplication
    if(Pipelined) stages = INTMULTI_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, INTMULTI, stages, "Integer Multiplier"));
    // Integer Shifter
    if(Pipelined) stages = INTSHIFTER_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, INTSHIFTER, stages, "Integer Shifter"));
    // Integer Bitwise
    if(Pipelined) stages = INTBITWISE_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, INTBITWISE, stages, "Integer Bitwise"));
    // Floating Point Single Precision Addition
    if(Pipelined) stages = FPSPADDER_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, FPSPADDER, stages, "Floating Point Single Precision Adder"));
    // Floating Point Double Precision Addition
    if(Pipelined) stages = FPDPADDER_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, FPDPADDER, stages, "Floating Point Double Precision Adder"));
    // Floating Point Single Precision Multiplier
    if(Pipelined) stages = FPSPMULTI_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, FPSPMULTI, stages, "Floating Point Single Precision Multiplier"));
    // Floating Point Single Precision Division
    if(Pipelined) stages = FPSPDIVID_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, FPSPDIVID, stages, "Floating Point Double Precision Divider"));
    // Floating Point Double Precision Multiplier
    if(Pipelined) stages = FPDPMULTI_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, FPDPMULTI, stages, "Floating Point Double Precision Multiplier"));
    // Floating Point Double Precision Division
    if(Pipelined) stages = FPDPDIVID_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, FPDPDIVID, stages, "Floating Point Double Precision Divider"));
    // Comparison Instructions
    if(Pipelined) stages = COMPARE_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, COMPARE, stages, "Comparison"));
    // GEP Instruction
    if(Pipelined) stages = GETELEMENTPTR_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, GETELEMENTPTR, stages, "GEP"));
    // Conversion Instructions
    if(Pipelined) stages = CONVERSION_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, CONVERSION, stages, "Conversion"));
    // Other Instructions
    if(Pipelined) stages = OTHER_STAGES;
    runtime_list.push_back(new FunctionalUnit(Latency, OTHER, stages, "Other"));
    // Registers
    registers = new FunctionalUnit(Latency, REGISTER, REGISTER_STAGES, "Registers");
    loads = 0; stores = 0; control_flow = 0;
    pipelined = Pipelined;
}

void
Hardware::updateParsed(uint8_t HardwareUnit) {
    for (auto& fu:runtime_list) {
        if ( HardwareUnit == fu->getHardwareUnit()) {
            fu->updateParse();
            return;
        }
    }
    runtime_list.back()->updateParse(); // Default: Final Element is Other
    // switch(HardwareUnit) {
    //     case COUNTER: (counter_list.front())->updateParse(); break;
    //     case INTADDER: ( int_adder_list.front())->updateParse(); break;
    //     case INTMULTI: ( int_multiplier_list.front())->updateParse(); break;
    //     case INTSHIFTER:git ( int_shifter_list.front())->updateParse(); break;
    //     case INTBITWISE: ( int_bitwise_list.front())->updateParse(); break;
    //     case FPSPADDER: ( fp_sp_adder_list.front())->updateParse(); break;
    //     case FPDPADDER: ( fp_dp_adder_list.front())->updateParse(); break;
    //     case FPSPMULTI: ( fp_sp_multiplier_list.front())->updateParse(); break;
    //     case FPSPDIVID: ( fp_sp_division_list.front())->updateParse(); break;
    //     case FPDPMULTI: ( fp_dp_multiplier_list.front())->updateParse(); break;
    //     case FPDPDIVID: ( fp_dp_division_list.front())->updateParse(); break;
    //     case COMPARE: ( comparison_list.front())->updateParse(); break;
    //     case GETELEMENTPTR: ( getelementptr_list.front())->updateParse(); break;
    //     case CONVERSION: ( conversion_list.front())->updateParse(); break;
    //     default: ( other_list.front())->updateParse(); break;
    // }
}

void
Hardware::activateUnits(int Units, 
                        int Latency,
                        int Stages,
                        int Static_Limit,
                        uint8_t ID,
                        std::string Name,
                        std::vector<FunctionalUnit*> &FU_List) {
    for (int i = 0; i < Units; i++ ) {
        FU_List.push_back(new FunctionalUnit(Latency, ID, Stages, Name));
        FU_List.back()->setStatic(Static_Limit);
    }
}

void
Hardware::updateLimit(int Counter, 
                            int IntAdder, 
                            int IntMul, 
                            int IntShift, 
                            int IntBit, 
                            int FPSPAdd, 
                            int FPDPAdd, 
                            int FPSPMul, 
                            int FPSPDiv, 
                            int FPDPMul, 
                            int FPDPDiv, 
                            int Compare, 
                            int GEP, 
                            int Conversion) {

    for (auto& fu:runtime_list) {
        switch(fu->getHardwareUnit()) {
            case COUNTER: { 
                activateUnits(fu->setStatic(Counter), 
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              counter_list);
                hardware_list.push_back(counter_list);
                break;
            }
            case INTADDER: {
                activateUnits(fu->setStatic(IntAdder), 
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              int_adder_list);
                hardware_list.push_back(int_adder_list);
                break;
            }
            case INTMULTI: {
                activateUnits(fu->setStatic(IntMul),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              int_multiplier_list);
                hardware_list.push_back(int_multiplier_list);
                break;
            }
            case INTSHIFTER: {
                activateUnits(fu->setStatic(IntShift),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              int_shifter_list);
                hardware_list.push_back(int_shifter_list);
                break;
            }
            case INTBITWISE: {
                activateUnits(fu->setStatic(IntBit),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              int_bitwise_list);
                hardware_list.push_back(int_bitwise_list);
                break;
            }
            case FPSPADDER: { 
                activateUnits(fu->setStatic(FPSPAdd),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              fp_sp_adder_list);
                hardware_list.push_back(fp_sp_adder_list);
                break;
            }
            case FPDPADDER: {
                activateUnits(fu->setStatic(FPDPAdd),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              fp_dp_adder_list);
                hardware_list.push_back(fp_dp_adder_list);
                break;
            }
            case FPSPMULTI: {
                activateUnits(fu->setStatic(FPSPMul),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              fp_sp_multiplier_list);
                hardware_list.push_back(fp_sp_multiplier_list);
                break;
            }
            case FPSPDIVID: {
                activateUnits(fu->setStatic(FPSPDiv),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              fp_sp_division_list);
                hardware_list.push_back(fp_sp_division_list);
                break;
            }
            case FPDPMULTI: {
                activateUnits(fu->setStatic(FPDPMul),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              fp_dp_multiplier_list);
                hardware_list.push_back(fp_dp_multiplier_list);
                break;
            }
            case FPDPDIVID: {
                activateUnits(fu->setStatic(FPDPDiv),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              fp_dp_division_list);
                hardware_list.push_back(fp_dp_division_list);
                break;
            }
            case COMPARE: {
                activateUnits(fu->setStatic(Compare),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              comparison_list);
                hardware_list.push_back(comparison_list);
                break;
            }
            case GETELEMENTPTR: {
                activateUnits(fu->setStatic(GEP),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              getelementptr_list);
                hardware_list.push_back(getelementptr_list);
                break;
            }
            case CONVERSION: {
                activateUnits(fu->setStatic(Conversion),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              conversion_list);
                hardware_list.push_back(conversion_list);
                break;
            }
            default: { 
                activateUnits(fu->setStatic(-1),  
                              fu->getLatency(), 
                              fu->getStages(), 
                              fu->getStaticLimit(), 
                              fu->getID(), 
                              fu->getName(), 
                              other_list);
                hardware_list.push_back(other_list);
                break;
            }
        }
    }
    setIDs();
}

bool
Hardware::updateStage(uint8_t HardwareUnit, int CurrentStage, int ID) {
  switch(HardwareUnit) {
        case COUNTER: {
            for ( auto it = counter_list.begin(); it != counter_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        case INTADDER: {
            for ( auto it = int_adder_list.begin(); it != int_adder_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        case INTMULTI:  {
            for ( auto it = int_multiplier_list.begin(); it != int_multiplier_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        case INTSHIFTER:  {
            for ( auto it = int_shifter_list.begin(); it != int_shifter_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        case INTBITWISE:  {
            for ( auto it = int_bitwise_list.begin(); it != int_bitwise_list.end(); ++ it) {
                 if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        case FPSPADDER:  {
            for ( auto it = fp_sp_adder_list.begin(); it != fp_sp_adder_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        case FPDPADDER:  {
            for ( auto it = fp_dp_adder_list.begin(); it != fp_dp_adder_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        case FPSPMULTI:  {
            for ( auto it = fp_sp_multiplier_list.begin(); it != fp_sp_multiplier_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage-1), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage)) {
                        (*it)->setStage(CurrentStage, true);
                        return true;
                    }
                }
            }
            break;
        }
        case FPSPDIVID:  {
            for ( auto it = fp_sp_division_list.begin(); it != fp_sp_division_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        case FPDPMULTI:  {
            for ( auto it = fp_dp_multiplier_list.begin(); it != fp_dp_multiplier_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        case FPDPDIVID:  {
            for ( auto it = fp_dp_division_list.begin(); it != fp_dp_division_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        case COMPARE: {
            for ( auto it = comparison_list.begin(); it != comparison_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        case GETELEMENTPTR: {
            for ( auto it = getelementptr_list.begin(); it != getelementptr_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        case CONVERSION: {
            for ( auto it = conversion_list.begin(); it != conversion_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
        default:  {
            for ( auto it = other_list.begin(); it != other_list.end(); ++ it) {
                if ((*it)->getID() == ID) {
                    if(CurrentStage == ((*it)->getStages()-1)) {
                        (*it)->setStage((CurrentStage), true);
                        return true;
                    } else if (!(*it)->isActive(CurrentStage + 1)) {
                        (*it)->setStage(CurrentStage + 1, true);
                        (*it)->setStage(CurrentStage, false);
                        return true;
                    }
                }
            }
            break;
        }
    }
    return false;
}

void
Hardware::setIDs() {
    int ID = 0;
    for ( auto hw_it = hardware_list.begin(); hw_it != hardware_list.end(); ++hw_it ) {
        for ( auto fu_it = hw_it->begin(); fu_it != hw_it->end(); ++fu_it) {
            (*fu_it)->setID(ID); 
            ID++;
        }
        ID = 0;
    }
}

int
Hardware::reserveFU(uint8_t HardwareUnit) {
    switch(HardwareUnit) {
        case COUNTER: {
            for ( auto it = counter_list.begin(); it != counter_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(COUNTER)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case INTADDER: {
            for ( auto it = int_adder_list.begin(); it != int_adder_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(INTADDER)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case INTMULTI:  {
            for ( auto it = int_multiplier_list.begin(); it != int_multiplier_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(INTMULTI)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case INTSHIFTER:  {
            for ( auto it = int_shifter_list.begin(); it != int_shifter_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(INTSHIFTER)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case INTBITWISE:  {
            for ( auto it = int_bitwise_list.begin(); it != int_bitwise_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(INTBITWISE)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case FPSPADDER:  {
            for ( auto it = fp_sp_adder_list.begin(); it != fp_sp_adder_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(FPSPADDER)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case FPDPADDER:  {
            for ( auto it = fp_dp_adder_list.begin(); it != fp_dp_adder_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(FPDPADDER)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case FPSPMULTI:  {
            for ( auto it = fp_sp_multiplier_list.begin(); it != fp_sp_multiplier_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(FPSPMULTI)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case FPSPDIVID:  {
            for ( auto it = fp_sp_division_list.begin(); it != fp_sp_division_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(FPSPDIVID)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case FPDPMULTI:  {
            for ( auto it = fp_dp_multiplier_list.begin(); it != fp_dp_multiplier_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(FPDPMULTI)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case FPDPDIVID:  {
            for ( auto it = fp_dp_division_list.begin(); it != fp_dp_division_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(FPDPDIVID)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case COMPARE: {
            for ( auto it = comparison_list.begin(); it != comparison_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(COMPARE)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case GETELEMENTPTR: {
            for ( auto it = getelementptr_list.begin(); it != getelementptr_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(GETELEMENTPTR)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        case CONVERSION: {
            for ( auto it = conversion_list.begin(); it != conversion_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(CONVERSION)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
        default:  {
            for ( auto it = other_list.begin(); it != other_list.end(); ++ it) {
                if ((*it)->available()) {
                    (*it)->reserve();
                    runtime_list.at(OTHER)->updateDynamic();
                    return (*it)->getID();
                }
            }
            break;
        }
    }
    return (-1);
}

bool
Hardware::isMultistaged(uint8_t HardwareUnit) {
     switch(HardwareUnit) {
        case COUNTER: return counter_list.front()->isMultistaged(); break;
        case INTADDER: return int_adder_list.front()->isMultistaged(); break;
        case INTMULTI: return int_multiplier_list.front()->isMultistaged(); break;
        case INTSHIFTER: return int_shifter_list.front()->isMultistaged(); break;
        case INTBITWISE: return int_bitwise_list.front()->isMultistaged(); break;
        case FPSPADDER: return fp_sp_adder_list.front()->isMultistaged(); break;
        case FPDPADDER: return fp_dp_adder_list.front()->isMultistaged(); break;
        case FPSPMULTI: return fp_sp_multiplier_list.front()->isMultistaged(); break;
        case FPSPDIVID: return fp_sp_division_list.front()->isMultistaged(); break;
        case FPDPMULTI: return fp_dp_multiplier_list.front()->isMultistaged(); break;
        case FPDPDIVID: return fp_dp_division_list.front()->isMultistaged(); break;
        case COMPARE: return comparison_list.front()->isMultistaged(); break;
        case GETELEMENTPTR: return getelementptr_list.front()->isMultistaged(); break;
        case CONVERSION: return conversion_list.front()->isMultistaged(); break;
        default: return other_list.front()->isMultistaged(); break;
    }
    return false;
}

void
Hardware::updateMax() {
    for (auto& fu:runtime_list) fu->maxDynamic();
}

void
Hardware::reset() {
    for (auto hw_list = hardware_list.begin(); hw_list != hardware_list.end(); ++hw_list){
        for (auto fu_list = hw_list->begin(); fu_list != hw_list->end(); ++fu_list) {
            (*fu_list)->resetStages();
        }
    }
    for (auto& fu:runtime_list) fu->resetRuntime();
}

void
Hardware::update(){
    updateMax();
    // Power
    // Registers
    //regList->resetAccess();
    reset();
}

void
Hardware::powerUpdate() {
    for (auto& hardware:hardware_list){
        for (auto& fu:hardware) {
            fu->powerUpdate();
        }
    }
}

void
Hardware::printFunctionalUnits(){
  //  for (auto& hardware:hardware_list){
  //      for (auto& fu:hardware) {
  //          fu->printHardwareStats();
  //      }
  //  }
    for (auto& fu:runtime_list) {
        std::cout << fu->getName() << " Functional Unit(s)\n";
        std::cout << "Static Max: " << fu->getStaticLimit() << "\n"; 
        std::cout << "Runtime Max: " << fu->getDynamicMax() << "\n";
    }

}

void
Hardware::finalize() {
    printFunctionalUnits();


}

void
Hardware::printResults() {
  //  printOccupancyResults();
  //  printLLVMFunctionalUnits();
  //  printDefinedFunctionalUnits();
  //  printDynamicFunctionalUnits();
}

void
Hardware::printTotals() {
    /*
    std::cout << "   SPM Leakage Power:               " << spm_leakage << " mW" << std::endl;
    std::cout << "   SPM Optimized Leakage Power:     " << spm_opt_leakage << " mW" << std::endl;
    std::cout << "   SPM Read Dynamic Power:          " << spm_read_dynamic << " mW" << std::endl;
    std::cout << "   SPM Write Dynamic Power:         " << spm_write_dynamic << " mW" << std::endl;
    std::cout << "   SPM Total Power:                 " << spm_leakage+spm_read_dynamic+spm_write_dynamic << " mW" << std::endl;
    std::cout << "   Total Accelerator Power:         " << total_power+spm_leakage+spm_read_dynamic+spm_write_dynamic << " mW" << std::endl;
    std::cout << std::endl;
    std::cout << "   ========= Area Analysis ===================" << std::endl;
    std::cout << "   SPM Area:                        " << spm_area/1000 << " um^2" << std::endl;
    std::cout << "   Total Accelerator Area:          " << total_area + spm_area/1000 << " um^2 (" << total_area/1000000 << " mm^2)" << std::endl;
    std::cout << std::endl;
    */
}



void
Hardware::printOccupancyResults() {
    // std::cout << "   ========= Stalled Cycle Breakdown ==========" << std::endl;    
    // std::cout << "       Load Only:                   " << occ_stalled.loadOnly << " cycles" << std::endl;
    // std::cout << "       Store Only:                  " << occ_stalled.storeOnly << " cycles" << std::endl;
    // std::cout << "       Compute Only:                " << occ_stalled.compOnly << " cycles" << std::endl;
    // std::cout << "       Compute & Store:             " << occ_stalled.storeComp << " cycles" << std::endl;
    // std::cout << "       Load & Store:                " << occ_stalled.loadStore << " cycles" << std::endl;
    // std::cout << "       Load & Compute:              " << occ_stalled.loadComp << " cycles" << std::endl;
    // std::cout << "       Load & Compute & Store:      " << occ_stalled.loadStoreComp << " cycles" << std::endl;
    // std::cout << "   ========= Execution Cycle Breakdown ========" << std::endl;    
    // std::cout << "       Load Only:                   " << occ_scheduled.loadOnly << " cycles" << std::endl;
    // std::cout << "       Store Only:                  " << occ_scheduled.storeOnly << " cycles" << std::endl;
    // std::cout << "       Compute Only:                " << occ_scheduled.compOnly << " cycles" << std::endl;
    // std::cout << "       Compute & Store:             " << occ_scheduled.storeComp << " cycles" << std::endl;
    // std::cout << "       Load & Store:                " << occ_scheduled.loadStore << " cycles" << std::endl;
    // std::cout << "       Load & Compute:              " << occ_scheduled.loadComp << " cycles" << std::endl;
    // std::cout << "       Load & Compute & Store:      " << occ_scheduled.loadStoreComp << " cycles" << std::endl;
    // std::cout << std::endl;
}

void 
Hardware::printPowerAnalysis() {
    /*
    std::cout << "   ========= Power Analysis ==================" << std::endl;
    std::cout << "   FU Leakage Power:                " << fu_leakage << " mW " << std::endl;
    std::cout << "   FU Dynamic Power:                " << fu_dynamic << " mW " << std::endl;
    std::cout << "   FU Total Power:                  " << fu_total_power << " mW " << std::endl;
    */
}

void 
Hardware::printAreaAnalysis() {
    /*
    std::cout << "   FU Area:                         " << fu_area << " um^2 (" << fu_area/1000000 << " mm^2)" << std::endl;
    */
}

void 
Hardware::printRegisterUsageAnalysis() {
    /*
    std::cout << "   ========= Pipeline Register Usage =========" << std::endl;
    std::cout << "   Total Number of Registers:       " << reg_total << std::endl;
    std::cout << "   Max Register Usage Per Cycle:    " << reg_max_usage << std::endl;
    std::cout << "   Avg Register Usage Per Cycle:    " << reg_avg_usage << std::endl;
    std::cout << "   Avg Register Size Per Cycle:     " << reg_avg_size << std::endl;
    std::cout << "       Total Register Reads:        " << reg_reads << std::endl;
    std::cout << "       Total Register Writes:       " << reg_writes << std::endl;
    std::cout << std::endl;
    */
}

void 
Hardware::printRegisterPowerAnalysis() {
    /*
    std::cout << "   Pipeline Register Leakage Power: " << reg_leakage  << " mW " << std::endl;
    std::cout << "   Pipeline Register Dynamic Power: " << reg_dynamic << " mW " << std::endl;
    std::cout << "   Pipeline Register Total Power:   " << reg_total_power <<  " mW" << std::endl;
    */
}

void 
Hardware::printRegisterAreaAnalysis() {
    /*
    std::cout << "   Register Area:                   " << reg_area << " um^2 (" << reg_area/1000000 << " mm^2)" << std::endl;
    */
}   

void
Hardware::printLLVMFunctionalUnits(){
    // std::cout << "   ========= LLVM Parsed FU's =================" << std::endl;
    // std::cout << "   Counter FU's:                    " << counter->getParsedLimit() << std::endl;
    // std::cout << "   Integer Add/Sub FU's:            " << int_adder->getParsedLimit() << std::endl;
    // std::cout << "   Integer Mul/Div FU's:            " << int_multiplier->getParsedLimit() << std::endl;
    // std::cout << "   Integer Shifter FU's:            " << int_shifter->getParsedLimit() << std::endl;
    // std::cout << "   Integer Bitwise FU's:            " << int_bitwise->getParsedLimit() << std::endl;
    // std::cout << "   Floating Point Float Add/Sub:    " << fp_sp_adder->getParsedLimit() << std::endl;
    // std::cout << "   Floating Point Double Add/Sub:   " << fp_dp_adder->getParsedLimit() << std::endl;
    // std::cout << "   Floating Point Float Mul:        " << fp_sp_multiplier->getParsedLimit() << std::endl;
    // std::cout << "   Floating Point Float Div:        " << fp_sp_division->getParsedLimit() << std::endl;
    // std::cout << "   Floating Point Double Mul:       " << fp_dp_multiplier->getParsedLimit() << std::endl;
    // std::cout << "   Floating Point Double Div:       " << fp_dp_division->getParsedLimit() << std::endl;
    // std::cout << "   Compare FU's:                    " << comparison->getParsedLimit() << std::endl;
    // std::cout << "   GEP Instruction FU's:            " << getelementptr->getParsedLimit() << std::endl;
    // std::cout << "   Type Conversion FU's:            " << conversion->getParsedLimit() << std::endl;
    // std::cout << "   Other FU's:                      " << other->getParsedLimit() << std::endl;
    // std::cout << std::endl;
}

void
Hardware::printDefinedFunctionalUnits(){
    // std::cout << "   ========= Statically Limited FU's ==========" << std::endl;
    // std::cout << "   Counter FU's:                    " << counter->getStaticLimit() << std::endl;
    // std::cout << "   Integer Add/Sub FU's:            " << int_adder->getStaticLimit() << std::endl;
    // std::cout << "   Integer Mul/Div FU's:            " << int_multiplier->getStaticLimit() << std::endl;
    // std::cout << "   Integer Shifter FU's:            " << int_shifter->getStaticLimit() << std::endl;
    // std::cout << "   Integer Bitwise FU's:            " << int_bitwise->getStaticLimit() << std::endl;
    // std::cout << "   Floating Point Float Add/Sub:    " << fp_sp_adder->getStaticLimit() << std::endl;
    // std::cout << "   Floating Point Double Add/Sub:   " << fp_dp_adder->getStaticLimit() << std::endl;
    // std::cout << "   Floating Point Float Mul:        " << fp_sp_multiplier->getStaticLimit() << std::endl;
    // std::cout << "   Floating Point Float Div:        " << fp_sp_division->getStaticLimit() << std::endl;
    // std::cout << "   Floating Point Double Mul:       " << fp_dp_multiplier->getStaticLimit() << std::endl;
    // std::cout << "   Floating Point Double Div:       " << fp_dp_division->getStaticLimit() << std::endl;
    // std::cout << "   Compare FU's:                    " << comparison->getStaticLimit() << std::endl;
    // std::cout << "   GEP Instruction FU's:            " << getelementptr->getStaticLimit() << std::endl;
    // std::cout << "   Type Conversion FU's:            " << conversion->getStaticLimit() << std::endl;
    // std::cout << "   Other FU's:                      " << other->getStaticLimit() << std::endl;
    // std::cout << std::endl;
}

void
Hardware::printDynamicFunctionalUnits(){
    // std::cout << "   ========= Runtime Allocated FU's ===========" << std::endl;
    // std::cout << "   Counter FU's:                    " << counter->getDynamicMax() << std::endl;
    // std::cout << "   Integer Add/Sub FU's:            " << int_adder->getDynamicMax() << std::endl;
    // std::cout << "   Integer Mul/Div FU's:            " << int_multiplier->getDynamicMax() << std::endl;
    // std::cout << "   Integer Shifter FU's:            " << int_shifter->getDynamicMax() << std::endl;
    // std::cout << "   Integer Bitwise FU's:            " << int_bitwise->getDynamicMax() << std::endl;
    // std::cout << "   Floating Point Float Add/Sub:    " << fp_sp_adder->getDynamicMax() << std::endl;
    // std::cout << "   Floating Point Double Add/Sub:   " << fp_dp_adder->getDynamicMax() << std::endl;
    // std::cout << "   Floating Point Float Mul:        " << fp_sp_multiplier->getDynamicMax() << std::endl;
    // std::cout << "   Floating Point Float Div:        " << fp_sp_division->getDynamicMax() << std::endl;
    // std::cout << "   Floating Point Double Mul:       " << fp_dp_multiplier->getDynamicMax() << std::endl;
    // std::cout << "   Floating Point Double Div:       " << fp_dp_division->getDynamicMax() << std::endl;
    // std::cout << "   Compare FU's:                    " << comparison->getDynamicMax() << std::endl;
    // std::cout << "   GEP Instruction FU's:            " << getelementptr->getDynamicMax() << std::endl;
    // std::cout << "   Type Conversion FU's:            " << conversion->getDynamicMax() << std::endl;
    // std::cout << "   Other FU's:                      " << other->getDynamicMax() << std::endl;
    // std::cout << std::endl;
}

/*
uca_org_t
Hardware::getCactiResults(int cache_size, int word_size, int ports, int cache_type) {
    return cactiWrapper((cache_size), word_size, ports, cache_type);
}
*/

