﻿#include "Extract_Types.h"
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
template<typename ...T> concept HAS_pDerived_pSelf_constraint =
	requires(T ...t) {
		sizeof...(t) >= 1;
		(HaspDerived<NthType_t<0, T...>>::value && 
		HaspSelf<NthType_t<0, T...>>::value); 
	};
template<typename ...T> concept VOID_constraint = requires(T ...t) {
	sizeof...(t) == 0;
};
template<size_t n, typename ...T> concept MAX_ARGUMENT_COUNT_constraint = requires(T ...t) {
	sizeof...(t) == n;
};
class BaseA {
	alignas(sizeof(void*)) bool failed;
	union alignas(void*) Code {
		void* codeVoidptr;
		unsigned long codeUnsignedLong;
		int codeInt;
	} code;
public :
	template<typename ...T> 
	requires VOID_constraint<T...> || HAS_pDerived_pSelf_constraint<T...>
	BaseA(T ...t) {
		if constexpr (sizeof...(t) == 0) {
			//standart ctor initializes everithing with null
			this->failed = 0;
			this->code.codeInt = 0;
		}
		else if constexpr (HaspDerived<NthType_t<0, T...>>::value && HaspSelf<NthType_t<0, T...>>::value) {
			unsigned long long displacement = (unsigned long long)unpack<0, T...>(t...).pSelf - (unsigned long long)unpack<0, T...>(t...).pDerived;
			memcpy(&(this->failed), &displacement, sizeof(void*));
		}
		else if constexpr (HaspSelf<NthType_t<0, T...>>::value) {
			unsigned long long displacement = (unsigned long long)unpack<0, T...>(t...).pSelf;
			memcpy(&(this->failed), &displacement, sizeof(void*));
		};
	}
	template<typename ...T> 
	requires VOID_constraint<T...> || HAS_pDerived_pSelf_constraint<T...>
	bool getFailed(T ...t) {
		if constexpr (sizeof...(t) == 0) {
			return this->failed;
		}
		else if constexpr (HaspDerived<NthType_t<0, T...>>::value && HaspSelf<NthType_t<0, T...>>::value) {
			return ((BaseA*)(
				(void*)((unsigned char*)unpack<0, T...>(t...).pDerived + this->failed)
				))->failed;
		}
		else if constexpr (HaspSelf<NthType_t<0, T...>>::value) {
			unsigned long long adress;
			memcpy(&adress, &(this->failed), sizeof(void*));
			return ((BaseA*)adress)->failed;
		}
	}
	template<typename ...T> 
	requires MAX_ARGUMENT_COUNT_constraint<2, T...>
	void setFailed(T ...t) {
		if constexpr (sizeof...(t) == 1) {
			this->failed = unpack<0, T...>(t...);
		}
		else if constexpr (HaspDerived<NthType_t<0, T...>>::value && HaspSelf<NthType_t<0, T...>>::value) {
			((BaseA*)(
				(void*)((unsigned char*)unpack<0, T...>(t...).pDerived + this->failed)
				))->failed = unpack<1, T...>(t...);
		}
		else if constexpr (HaspSelf<NthType_t<0, T...>>::value) {
			unsigned long long adress;
			memcpy(&adress, &(this->failed), sizeof(void*));
			((BaseA*)adress)->failed = unpack<1, T...>(t...);
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
	ForwardByOffset<BaseA, CFinal> forwardByOffset = {
		(BaseA*)&cFinal.derivedFromCBaseA.Dummy.cBaseA,
		&cFinal
	};
	BaseA baseA = BaseA(forwardByOffset);
	cFinal.derivedFromCBaseA.Dummy.cBaseA.failed = true;
	cout << baseA.getFailed(forwardByOffset) << endl;
	baseA.setFailed(forwardByOffset, false);
	cout << baseA.getFailed(forwardByOffset) << endl;
	baseA.setFailed(forwardByOffset, true);
	cout << baseA.getFailed(forwardByOffset) << endl;

	CFinal c1Final;
	ForwardByAddress<BaseA> forwardByAddress = {
		(BaseA*)&c1Final.derivedFromCBaseA.Dummy.cBaseA
	};
	BaseA base1A = BaseA(forwardByAddress);
	c1Final.derivedFromCBaseA.Dummy.cBaseA.failed = true;
	cout << base1A.getFailed(forwardByAddress) << endl;
	base1A.setFailed(forwardByAddress, false);
	cout << base1A.getFailed(forwardByAddress) << endl;
	base1A.setFailed(forwardByAddress, true);
	cout << base1A.getFailed(forwardByAddress) << endl;

	BaseA base2A = BaseA();
	base2A.setFailed(true);
	cout << base2A.getFailed() << endl;
	base2A.setFailed(false);
	cout << base2A.getFailed() << endl;
	base2A.setFailed(true);
	cout << base2A.getFailed() << endl;
	
	return 0;
}