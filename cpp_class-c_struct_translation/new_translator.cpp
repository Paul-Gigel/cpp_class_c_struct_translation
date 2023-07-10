#include "Extract_Types.h"
#include "Extract_Arguments.h"
#include "Check_If_Has_Member.h"
#include "ForwardBy.h"
#include "Concepts.h"
#include <iostream>
#include <concepts>
#include <tuple>
using namespace std;
// agregate
// trivial type
// standart layout type
// pod type
// literal type
//this BaseA thing needs to be somekind of templated child, that either inherites from the "Real" class or a Dummy pointerclass
class BaseA_ptr {
	union {
		std::intptr_t offset;
		std::intptr_t pointer;
	};
public:
	template<typename T>
	BaseA_ptr() {
		
	};
};
class BaseA_real {
	alignas(sizeof(void*)) bool failed;
	union {
		void* codeVoidptr;
		unsigned long codeUnsignedLong;
		int codeInt;
	} code;
public:
	BaseA_real() = default;
	template <typename = void> 
	bool getFailed() const {
		return this->failed;
	}
	template <typename = void>
	decltype(code) getCode() const {
		return this->code;
	}
	template <typename = void>
	void setFailed(const bool& value) {
		this->failed = value;
	}
	template <typename = void>
	void setCode(const decltype(code)& value) {
		this->code = value;
	}
};
template<bool UseAsPtr> class BaseA;
template<> class BaseA<0> : public BaseA_real	{};
template<> class BaseA<1> : public BaseA_ptr	{};


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
int main() {
	BaseA<0> baseA = {};
	baseA.setFailed(true);
	baseA.setCode({});
	std::cout << baseA.getFailed() << "\n";
	return 0;
};