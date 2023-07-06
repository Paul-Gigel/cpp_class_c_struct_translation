#include "Extract_Types.h"
#include "Extract_Arguments.h"
#include "Check_If_Has_Member.h"
#include "ForwardBy.h"
#include "Concepts.h"
#include <iostream>
#include <concepts>
using namespace std;

//this BaseA thing needs to be somekind of templated child, that either inherites from the "Real" class ore a Dummy pointerclass
class BaseA_ptr {
	union {
		std::intptr_t offset;
		std::intptr_t pointer;
	};
};
class BaseA_real {
	alignas(sizeof(void*)) bool failed;
	union alignas(void*) {
		void* codeVoidptr;
		unsigned long codeUnsignedLong;
		int codeInt;
	} code;
};

class BaseA : public BaseA_real/*, public BaseA_ptr*/ {

};
extern "C" {
	struct CBaseA {
		volatile alignas(void*) bool failed;
		union alignas(void*) Code {
			void* codeVoidptr;
			unsigned long codeUnsignedLong;
			int codeInt;
		} code;
	};
	typedef struct DerivedFromCBaseA
	{
		unsigned int PEParserStructurType;
		union {
			CBaseA cBaseA;
			/*other struct instances...*/
		} Dummy;
	} *pDerivedFromCBaseA;
	typedef struct CFinal {
		DerivedFromCBaseA derivedFromCBaseA;
	};
}


template<typename... T>
void myFunction(T... t) {
	// Function body
}

int main() {
	
	return 0;
}