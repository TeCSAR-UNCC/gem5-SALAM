#include "register.hh"
#include <string>
#include "debug/LLVMRegister.hh"
#include "debug/LLVMInterface.hh"



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
    if (temp.compare("pointer") == 0) size = 8;
    else if (temp.back() =='*') size = 8;
    //Boolean and integer data types
    // Set size if dataType is integer
    else if (temp.front() == 'i') {
        temp = temp.substr(1);
        size = ((std::stoi(temp)-1)/8)+1;
    }
    // Floating point data types
    // Set size if dataType is half
    else if (temp.find("half")) size = 2;
    
    // Set size if dataType is float
    else if (temp.find("float")) size = 4;
    
    // Set size if dataType is double
    else if (temp.find("double")) size = 8;
    
    // Set size if dataType is void
    else if (temp.find("void")) size = 0;
    
    // Aggregate data types
    // Array dataType
    else if (temp[0] == '[') { }

    // Struct dataType
    else if (temp.find("{")) { }

    // Unspecified dataType
    // Label
    // Treat size equivalent to a pointer
    else if (temp.find("label")) size = 8;
    
    // Unknown dataType
    else { }
    // // For any additional dataTypes follow this format in else if statements
    //   else if(temp.find("dataType")){
    // }

    // Potential for moving metadata into function as datatype in future
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
