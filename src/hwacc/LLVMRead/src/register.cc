#include "register.hh"
#include <string>
#include "debug/LLVMRegister.hh"
#include "debug/LLVMInterface.hh"



Register::Register(std::string id) {
    name = id;
    value = 0;
}
Register::Register(std::string id, uint64_t val){
    name = id;
    value = val;
}
void 
Register::setSize(){
    // Code here will infer size based around the stored dataType string
    std::string temp = dataType;
    // Pointers
    if (temp.find('*')) size = 8;
    //Boolean and integer data types
    // Set size if dataType is integer
    else if (temp[0] == 'i') {
        temp = temp.substr(1);
        size = std::stoi(temp)/8;
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
