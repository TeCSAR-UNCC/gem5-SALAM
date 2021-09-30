


class FunctionalUnitGenerator():
    # Functional unit alias from yml is passed for classname variable
    def __init__(self, alias="", directory=""):
        self.alias = alias
        self.directory = directory
        self.classname = ''.join(words.capitalize() for words in self.alias.split('_'))

    def header_generator(self):
        # create C++ header
        #self.header_name = self.directory + '/' + self.alias + ''.hh'
        self.header_name = self.alias + '.hh'

        self.new_header = open(self.header_name, 'w')

        # start header gaurd
        self.new_header.write("#ifndef __HWMODEL_" + self.alias.upper() + "_HH__\n")
        self.new_header.write("#define __HWMODEL_" + self.alias.upper() + "_HH__\n\n")

        # warning label
        self.new_header.write("// GENERATED FILE - DO NOT MODIFY\n\n")
        
        # includes
        self.new_header.write("#include \"params/" + self.classname + ".hh\"\n")
        self.new_header.write("#include \"sim/sim_object.hh\"\n")
        self.new_header.write("#include \"base.hh\"\n\n")

        # namespaces
        self.new_header.write("using namespace gem5;\n\n")

        # define class name and inheritance

        self.new_header.write("class " + self.classname + ": public SimObject, public FunctionalUnitBase\n")
        self.new_header.write("{\n")

        # define class body
        # define private members and methods
        self.new_header.write("\tprivate:\n")
        # members

        # methods
        
        # define protected members and methods
        self.new_header.write("\tprotected:\n")
        # members

        # methods

        # define public members and methods
        self.new_header.write("\tpublic:\n")
        # members

        # methods
        # default constructor
        self.new_header.write("\t\t" + self.classname + "();\n")
        # SimObject Constructor
        self.new_header.write("\t\t" + self.classname + "(const " + self.classname + "Params &params);\n")


        # end class body
        self.new_header.write("};\n")
        # end header gaurd
        self.new_header.write("#endif // __HWMODEL_" + self.alias.upper() + "_HH__")

        self.new_header.close()

    def method_generation(self):