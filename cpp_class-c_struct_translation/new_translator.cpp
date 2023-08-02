#include "Check_If_Has_Member.h"
#include "Extract_Types.h"
#include "Extract_Arguments.h"
#include "Concepts.h"
#include <iostream>

//c compatible header
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

//translator lib
template<typename Self, typename Derived>
struct ForwardByOffset {
	Self* pSelf;
	Derived* pDerived;
	ForwardByOffset(Self* pSelf, Derived* pDerived) : pSelf(pSelf), pDerived(pDerived) {};
};
template<typename Self>
struct ForwardByAddress {
	Self* pSelf;
	ForwardByAddress(Self* pSelf) : pSelf(pSelf) {};
};

using namespace std;
// agregate
// trivial type
// standart layout type
// pod type
// literal type
// this BaseA thing needs to be somekind of templated child, that either inherites from the "Real" class or a Dummy pointerclass
// HAS_MEMBER_VARIABLE_MACRO implement as operator->() or operator.() as part of a constexpr wrapper class

//internal
class BaseA_c : public CBaseA {
public:
	BaseA_c() = default;
	template <typename = void>
	constexpr bool getFailed() const {
		return this->failed;
	}
	template <typename = void>
	constexpr decltype(code) getCode() const {
		return this->code;
	}
	template <typename = void>
	constexpr void setFailed(const bool& value) {
		this->failed = value;
	}
	template <typename = void>
	constexpr void setCode(const decltype(code)& value) {
		this->code = value;
	}
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
class BaseA_ptr {
	union {
		std::intptr_t offset;
		std::intptr_t pointer;
	};
public:
	template<typename T>
	BaseA_ptr(T t) {
		if constexpr (HaspDerived<T>::value && HaspSelf<T>::value) {
			std::intptr_t displacement = (std::intptr_t)t.pSelf - (std::intptr_t)t.pDerived;
			memcpy(&(offset), &displacement, sizeof(void*));
		}
		else if constexpr (HaspSelf<T>::value) {
			std::intptr_t displacement = (std::intptr_t)T.pSelf;
			memcpy(&(pointer), &displacement, sizeof(void*));
		};
	};
	template<typename T>
	bool getFailed(const T& t) const {
		if constexpr (HaspDerived<T>::value && HaspSelf<T>::value) {
			return ((BaseA_c*)(
				(void*)((unsigned char*)(std::intptr_t)t.pDerived + (std::intptr_t)offset)
				))->getFailed();
		}
		else if constexpr (HaspSelf<T>::value) {
			unsigned long long adress;
			memcpy(&adress, &(pointer), sizeof(void*));
			return ((decltype(t.pSelf)*)adress)->getFailed();
		}
	}
	template<typename ...T> 
	requires HAS_pDerived_pSelf_constraint<T...>
	void setFailed(T ...t, const bool& value) {
		if constexpr (HaspDerived<NthType_t<0, T...>>::value && HaspSelf<NthType_t<0, T...>>::value) {
			return ((BaseA_real*)(
				(void*)((unsigned char*)unpack<0, T...>(t...).pDerived + offset)
				))->setFailed(value);
		}
		else if constexpr (sizeof...(t) == 0){
			unsigned long long adress;
			memcpy(&adress, &(pointer), sizeof(void*));
			return ((BaseA_real*)adress)->failed;
		}
	}
};

template<unsigned int UseAsPtr> class BaseA;
template<> class BaseA<0> : public BaseA_real	{};
template<> class BaseA<1> : public BaseA_ptr	{};
template<> class BaseA<2> : public BaseA_c		{};

int main() {
	CFinal cFinal;
	ForwardByOffset forwardByOffset = {
		&cFinal.derivedFromCBaseA.Dummy.cBaseA,
		&cFinal
	};
	BaseA<1> baseA = BaseA<1>(forwardByOffset);
	cFinal.derivedFromCBaseA.Dummy.cBaseA.failed = false;
	cout << baseA.getFailed(forwardByOffset) << endl;
	/*baseA.setFailed(forwardByOffset, false);
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
	return 0;*/
};