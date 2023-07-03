#include "Extract_Types.h"
#include "Extract_Arguments.h"
#include "Check_If_Has_Member.h"
#include <iostream>
#include <concepts>
using namespace std;

template<typename Self, typename Derived>
struct ForwardByOffset {
	Self*		pSelf;
	Derived*	pDerived;
};
template<typename Self>
struct ForwardByAddress {
	Self* pSelf;
};
class BaseA {
	alignas(sizeof(void*)) bool failed;
	union alignas(void*) Code {
		void* codeVoidptr;
		unsigned long codeUnsignedLong;
		int codeInt;
	} code;
public :
	template<typename ...T> BaseA(T ...t) {
		if constexpr (HasPDerived<NthType_t<0, T...>>::value && HasPSelf<NthType_t<0, T...>>::value) {
			unsigned long long displacement = (unsigned long long)unpack<0, T...>(t...).pSelf - (unsigned long long)unpack<0, T...>(t...).pDerived;
			memcpy(&(this->failed), &displacement, sizeof(void*));
		} else {
			if constexpr (HasPSelf<NthType_t<0, T...>>::value) {
				unsigned long long displacement = (unsigned long long)unpack<0, T...>(t...).pSelf;
				memcpy(&(this->failed), &displacement, sizeof(void*));
			} else {
				//do nothing (standart ctor)
			}
		}
	}
	template<typename ...T> bool getFailed(T ...t) {
		if constexpr (HasPDerived<NthType_t<0, T...>>::value && HasPSelf<NthType_t<0, T...>>::value) {
			return ((BaseA*)(
				(void*)((unsigned char*)unpack<0, T...>(t...).pDerived + this->failed)
				))->failed;
		}
		else {
			if constexpr (HasPSelf<NthType_t<0, T...>>::value) {
				unsigned long long adress;
				memcpy(&adress, &(this->failed), sizeof(void*));
				return ((BaseA*)adress)->failed;
			}
			else {
				return this->failed;
			}
		}
	}
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

int main()	{
	CFinal cFinal;
	cFinal.derivedFromCBaseA.Dummy.cBaseA.failed = true;
	ForwardByOffset<BaseA, CFinal> forwardByOffset = {
		(BaseA*)&cFinal.derivedFromCBaseA.Dummy.cBaseA,
		&cFinal
	};
	BaseA baseA = BaseA(forwardByOffset);
	cout << baseA.getFailed(forwardByOffset) << endl;
	
	CFinal c1Final;
	c1Final.derivedFromCBaseA.Dummy.cBaseA.failed = true;
	ForwardByAddress<BaseA> forwardByAddress = {
		(BaseA*)&c1Final.derivedFromCBaseA.Dummy.cBaseA
	};
	BaseA base1A = BaseA(forwardByAddress);
	cout << base1A.getFailed(forwardByAddress) << endl;
	
	return 0;
}