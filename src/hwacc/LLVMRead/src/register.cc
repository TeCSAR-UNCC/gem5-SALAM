#include "register.hh"
#include <string>
#include <iostream>

Register::Register(std::string id) {
    name = id;
    value = 0;
    size = 8;
}
Register::Register(std::string id, uint64_t val){
    name = id;
    value = val;
    size = 8;
}
void 
Register::setSize(){
    // Code here will infer size based around the stored dataType string
    std::string temp = dataType;
    // Pointers
    if (temp.compare("pointer") == 0) size = SystemSize/8;
    else if (temp.back() =='*') size = SystemSize/8;
    // Boolean and integer data types
    // Set size if dataType is integer
    else if (temp.front() == 'i') {
        temp = temp.substr(1);
        size = ((std::stoi(temp)-1)/8)+1;
    }
    // Floating point data types    
    // Set size if dataType is float
    else if (temp.find("float") > -1) size = SystemSize/16;
    // Set size if dataType is double
    else if (temp.find("double") > -1) size = SystemSize/8;
    // Set size if dataType is void
    else if (temp.find("void") > -1) size = 0;
    // Aggregate data types
    // Array dataType
    else if (temp[0] == '[') { }
    // Struct dataType
    else if (temp.find("{") > -1) { }
    // Unspecified dataType
    // Label
    // Treat size equivalent to a pointer
    else if (temp.find("label") > -1) size = SystemSize/8;
    // Unknown dataType
    else { }
}
void 
Register::setSize(std::string type) {
    dataType = type;
    setSize();
}
Register *
RegisterList::findRegister(std::string name) {
    for (auto it = regList->begin(); it != regList->end(); ++it) {
        if ((*it)->name.compare(name) == 0) return *it;
    }
    return NULL;
}
void
Register::setValue(void *data) { 
    memcpy(&value, data, size);
}
void
RegisterList::printRegNames() {
    for (auto it=regList->begin(); it!=regList->end(); ++it) {
        std::cout << (*it)->getName() << "\n";
    }
}
