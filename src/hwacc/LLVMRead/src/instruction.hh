#ifndef __DECODER_HH__

#define __DECODER_HH__

#include "register.hh"

#define MAXCASES 4

struct Attributes {
	struct Clock {
		bool finished;
		//Tick cycleCount;
		//Tick maxCycles;
	};
	struct Switch {
		std::string argument;
		std::string defaultCase;
		std::string caseStatements;
	};
	struct Branch {
		Register * labelTrue;
		Register * labelFalse;
		Register * conditionCheck;
		std::string uncond;
		std::string iftrue;
		std::string iffalse;
		std::string cond;
		std::string addr;
		std::string destinationList;
	};
	struct Params {
		std::string operand1;
		std::string operand2;
		std::string dataType;
		std::string returnValue;
		Register * returnRegister;
		std::string align;
		std::string numElements;
		std::string type2;

	};
	struct Phi {
		std::string paths[5];
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
	struct CConv {
		bool ccc = false,
			fastcc = false,
			coldcc = false,
			cc10 = false,
			cc11 = false;
	};
	struct ParamAttributes {
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
	struct FunctionAttributes {
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
	struct Invoke {
		std::string unwindlbl;
		std::string normallbl;
		std::string prototype;
	};
	Invoke invoke;
	FunctionAttributes fattrs;
	ParamAttributes pattrs;
	CConv cconv;
	Phi phi;
	Flags flags;
	Condition condition;
	Params params;
	Clock clock;
	Branch branch;
	Switch switchStmt;
};



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
		bool custom = false;
		bool flowControl = false;

	};
	struct Dependencies {
		Register* registers[5];
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
	struct Terminator {
		std::string type;
		std::string ivalue;
		std::string intty; //int type
		std::string somety;
		bool intermediate = false;
		Register* value; 
		Register* iftrue;
		Register* iffalse;
		Register* cond;
		Register* dest;
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

	};
	struct Operations {

	};
	struct Other {

	};
	struct Custom {

	};
	General general;
	Dependencies dependencies;
	Cycle cycle;
	Attributes attributes;
	Flags flags;
	Condition condition;
	Terminator terminator;
	Binary binary;
	Bitwise bitwise;
	Vector vector;
	Aggregate aggregate;
	Memory memory;
	Operations operations;
	Other other;
	Custom custom;


};

#endif //__DECODER_HH__