#ifndef __DECODER_HH__

#define __DECODER_HH__

#include "register.hh"

#define MAXCASES 4
#define MAXDEPENDENTS 5
#define MAXPHI 5
#define MAXGPE 5


struct Instruction {
	struct General {
		std::string llvm_Line; //
		std::string opCode; //
		Register* returnRegister; //
		int lineNumber; 
		int fields; //
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
		bool flowControl = false;

	};
	struct Dependencies {
		Register* registers[MAXDEPENDENTS];
	};
	struct Cycle {
		int current;
		int max;
	};
	struct Attributes {
		struct Parameters {
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
		struct Function {
			int allign = 0;
			bool allignstack = false,
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
		struct CConv {
			bool ccc = false,
				fastcc = false,
				coldcc = false,
				cc10 = false,
				cc11 = false;
		};
		CConv cconv;
		Parameters parameters;
		Function fuction;
	};
	struct Flags {
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
	
	struct Terminator {
		std::string type;
		std::string ivalue;
		std::string intty; //int type
		std::string somety;
		bool intermediate = false;
		bool unconditional = false;
		Register* value; 
		std::string iftrue;
		std::string iffalse;
		Register* cond;
		mutable std::string dest;
		Register* defaultdest;
		Register* Addr;
		Register* exception_label;
		Register* normal_label;

		struct Cases {
			int statements = 0;
			std::string intty[MAXCASES];
			Register* value[MAXCASES];
			Register* dest[MAXCASES];
		};
		Cases cases;
	};
	struct Binary {
		std::string iop1;
		std::string iop2;
		std::string ty;
		bool immediate1 = false;
		bool immediate2 = false;
		Register* op1;
		Register* op2;

	};
	struct Bitwise {
		std::string iop1;
		std::string iop2;
		std::string ty;
		bool immediate1 = false;
		bool immediate2 = false;
		Register* op1;
		Register* op2;
	};
	struct Vector {

	};
	struct Aggregate {

	};
	struct Memory {
		struct Load{
			bool volatileVar = false;
			std::string ty;
			Register* pointer;
			int align;
		};
		struct Store{
			bool volatileVar = false;
			std::string ty;
			Register* pointer;
			int align;
		};
		struct GetElementPtr{
			bool inbounds = false;
			int index;
			mutable unsigned long long int reference[MAXGPE];
			std::string pty;
			std::string ptrval;
			std::string ty[MAXGPE];
			Register* idx[MAXGPE];
		};
		Load load;
		Store store;
		GetElementPtr getptr;
	};
	struct Conversion {

	};
	struct Other {

		struct Compare {
			std::string ty;
			std::string iop1;
			std::string iop2;
			Register* op1;
			Register* op2;
			bool immediate1 = false;
			bool immediate2 = false;
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
			Condition condition;
		};
		Compare compare;

		struct Phi {
			std::string ty;
			std::string ival[MAXPHI];
			std::string ilabel[MAXPHI];
			bool immVal[MAXPHI];
			bool immLabel[MAXPHI];
			Register* val[MAXPHI];
			std::string label[MAXPHI];
			mutable Register* takenVal;
		};
		Phi phi;
	};
	struct Custom {

	};
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


};

#endif //__DECODER_HH__