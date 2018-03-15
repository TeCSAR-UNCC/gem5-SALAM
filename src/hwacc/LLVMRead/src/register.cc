#include "register.hh"

Register::Register(std::string id) {
    name = id;
    value = 0;
    //next = NULL;
}

Register::Register(std::string id, int val) {
    name = id;
    value = val;
    //next = NULL;
}

//RegisterList::RegisterList() {
//    head = NULL;
//    tail = NULL;
//    numRegisters = 0;
//}

//void 
//RegisterList::addRegister(Register *reg) {
//    if (head==NULL) {
//        head = reg;
//        tail = reg;
//    } else {
//        tail->next = reg;
//        tail = reg;
//    }
//    numRegisters++;
//}

//Register *
//RegisterList::findRegister(std::string name) {
//    Register * test = head;
//    while(test->name.compare(name) != 0) {
//        test = test->next;
//    }
//    return test;
//}

Register *
RegisterList::findRegister(std::string name) {
    for(auto it=regList->begin(); it!=regList->end(); ++it) {
        if ((*it)->name.compare(name) == 0)
            return *it;
    }
    return NULL;
}
