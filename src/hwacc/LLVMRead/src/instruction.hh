#ifndef __DECODER_HH__

#define __DECODER_HH__

#include "register.hh"
#include "llvm_types.hh"
#include "debugFlags.hh"

#define MAXCASES 5
#define MAXDEPENDENTS 5
#define MAXPHI 10
#define MAXGPE 5
// ******************************************************************
// 
//
struct Instruction {
	// **************************************************************
	struct General {
		std::string llvm_Line;	
		std::string opCode;		  
		std::string returnType;
		Register *returnRegister; 
		Register *immediateCount;
		Register *labelCount;
		// Flags
		bool flowControl = false;
		bool terminator = false;
		bool binary = false;
		bool bitwise = false;
		bool vector = false;
		bool aggregate = false;
		bool memory = false;
		bool conversion = false;
		bool other = false;
		bool compare = false;
		bool phi = false;
		bool custom = false;
		bool integer = false;
		bool floatingPoint = false;
		bool multiplier = false;
		bool adder = false;
		bool bit = false;
		bool shifter = false;
	};
	// **************************************************************
	struct Dependencies {
		Register *registers[MAXDEPENDENTS];
	};
	// **************************************************************
	struct Cycle {
		int current;
		int max;
	};
	// **************************************************************
	struct Attributes {
		// **********************************************************
		struct Parameters {
			// Flags
			bool zeroext = false,
				 signext = false,
				 inreg = false,
				 byval = false,
				 sret = false,
				 noalias = false,
				 nocapture = false,
				 nest = false,
				 returned = false;
		};
		// **********************************************************
		struct Function {
			int align = 0;
			// Flags
			bool alignstack = false,
				 alwaysinline = false,
				 builtin = false,
				 cold = false,
				 inlineint = false,
				 minsize = false,
				 naked = false,
				 nobuiltin = false,
				 noduplicate = false,
				 noimplicitfloat = false,
				 noinline = false,
				 nonlazybind = false,
				 noredzone = false,
				 noreturn = false,
				 nounwind = false,
				 optnone = false,
				 optsize = false,
				 readnone = false,
				 readonly = false,
				 returns_twice = false,
				 sanitize_address = false,
				 sanitize_memory = false,
				 sanitize_thread = false,
				 ssp = false,
				 sspreq = false,
				 sspstrong = false,
				 uwtable = false;
		};
		// **********************************************************
		struct CConv {
			// Flags
			bool ccc = false,
				 fastcc = false,
				 coldcc = false,
				 cc10 = false,
				 cc11 = false;
		};
		// **********************************************************
		// Nested struct definitions
		CConv cconv;
		Parameters parameters;
		Function fuction;
		// **********************************************************
	};
	// **************************************************************
	struct Flags {
		// Flags
		bool nnan = false,
			 ninf = false,
			 nsz = false,
			 arcp = false,
			 contract = false,
			 afn = false,
			 reassoc = false,
			 fast = false,
			 nsw = false,
			 nuw = false,
			 exact = false;
	};
	// **************************************************************
	struct Terminator {
		std::string type;
		std::string ivalue;
		std::string intty; 
		std::string somety;
		std::string iftrue;
		std::string iffalse;
		std::string defaultdest;
		// **********************************************************
		// Mutable because destination can changed during runtime
		mutable std::string dest;
		// **********************************************************
		Register *cond;
		Register *value;
		Register *Addr;
		Register *exception_label;
		Register *normal_label;
		// Flags
		bool intermediate = false;
		bool unconditional = false;
		// **********************************************************
		struct Cases {
			int statements = 0;
			int value[MAXCASES];
			std::string dest[MAXCASES];
			std::string intty[MAXCASES];
		};
		// **********************************************************
		// Nested struct defintion
		Cases cases;
		// **********************************************************
	};
	// **************************************************************
	struct Binary {
		std::string iop1;
		std::string iop2;
		std::string ty;
		Register *op1;
		Register *op2;
		// Flags
		bool immediate1 = false;
		bool immediate2 = false;
	};
	// **************************************************************
	struct Bitwise {
		std::string iop1;
		std::string iop2;
		std::string ty;
		Register *op1;
		Register *op2;
		// Flags
		bool immediate1 = false;
		bool immediate2 = false;
	};
	// **************************************************************
	struct Vector { };
	// **************************************************************
	struct Aggregate { };
	// **************************************************************
	struct Memory {
		// **********************************************************
		struct Load {
			std::string ty;
			Register *pointer;
			int align;
			// Flags
			bool volatileVar = false;
		};
		// **********************************************************
		struct Store {
			std::string ty;
			Register *pointer;
			Register *value;
			int align;
			int ival;
			// Flags
			bool volatileVar = false;
			bool immediate = false;

		};
		// **********************************************************
		struct GetElementPtr {
			int immdx[MAXGPE];
			int index;
			mutable unsigned long long int reference[MAXGPE];
			std::string pty;
			std::string ty[MAXGPE];
			Register *ptrval;
			Register *idx[MAXGPE];
			LLVMType *llvmType;
			// Flags
			bool inbounds = false;
			bool immediate[MAXGPE];
		};
		// **********************************************************
		// Nested struct definitions
		Load load;
		Store store;
		GetElementPtr getptr;
		// **********************************************************
	};
	// **************************************************************
	struct Conversion { 
		bool immediate = false;
		int immVal;
		Register* value;
		std::string ty;
		std::string ty2;
	};
	// **************************************************************
	struct Other {
		// **********************************************************
		struct Compare {
			std::string ty;
			std::string iop1;
			std::string iop2;
			Register *op1;
			Register *op2;
			bool immediate1 = false;
			bool immediate2 = false;
			// ******************************************************
			struct Condition {
				std::string cond;
				bool eq = false,
					 ne = false,
					 ugt = false,
					 uge = false,
					 ult = false,
					 ule = false,
					 sgt = false,
					 sge = false,
					 slt = false,
					 sle = false,
					 condFalse = false,
					 condTrue = false,
					 oeq = false,
					 ogt = false,
					 oge = false,
					 olt = false,
					 ole = false,
					 one = false,
					 ord = false,
					 ueq = false,
					 une = false,
					 uno = false;
			};
			// ******************************************************
			// Nested struct definition
			Condition condition;
			// ******************************************************
		};
		// **********************************************************
		struct Phi {
			std::string ty;
			std::string ival[MAXPHI];
			std::string ilabel[MAXPHI];
			bool immVal[MAXPHI];
			bool immLabel[MAXPHI];
			Register *val[MAXPHI];
			std::string label[MAXPHI];
			mutable Register *takenVal;
		};
		// **********************************************************
		struct Select {
			Register *cond;
			Register *val1;
			Register *val2;
			bool icondFlag = false;
			bool icond = false;
			int immVal[2];
			std::string ty;
			bool intTy = false;
			bool floatTy = false;
			bool doubleTy = false;
			bool immediate[2];
		};
		// **********************************************************
		// Nested struct definition
		Compare compare;
		Phi phi;
		Select select;
		// **********************************************************
	};
	// **************************************************************
	struct Custom { };
	// **************************************************************
	// Struct definitions
	General general;
	Dependencies dependencies;
	Cycle cycle;
	Attributes attributes;
	Flags flags;
	Terminator terminator;
	Binary binary;
	Bitwise bitwise;
	Vector vector;
	Aggregate aggregate;
	Memory memory;
	Conversion conversion;
	Other other;
	Custom custom;
	// **************************************************************
};
// ******************************************************************

#endif //__DECODER_HH__
