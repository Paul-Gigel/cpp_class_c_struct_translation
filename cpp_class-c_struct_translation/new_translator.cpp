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
	union {
		Self* pSelf;
		std::intptr_t pSelfAdress;
	};
	union {
		Derived* pDerived;
		std::intptr_t pDerivedAdress;
	};
	ForwardByOffset(Self* pSelf, Derived* pDerived) : pSelf(pSelf), pDerived(pDerived) {};
};
template<typename Self>
struct ForwardByAddress {
	union {
		Self* pSelf;
		std::intptr_t pSelfAdress;
	};
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
class BaseA_real_test {
protected:
	alignas(sizeof(void*)) bool failed;
	union {
		void* codeVoidptr;
		unsigned long codeUnsignedLong;
		int codeInt;
	} code;
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
			std::intptr_t displacement = t.pSelfAdress - t.pDerivedAdress;
			memcpy(&(offset), &displacement, sizeof(void*));
		}
		else if constexpr (HaspSelf<T>::value) {
			std::intptr_t displacement = t.pSelfAdress;
			memcpy(&(pointer), &displacement, sizeof(void*));
		};
	};
	template<typename T>
	bool getFailed(const T& t) const {
		if constexpr (HaspDerived<T>::value && HaspSelf<T>::value) {
			return ((BaseA_c*)(
				(void*)((unsigned char*)t.pDerivedAdress + offset)
				))->getFailed();
		}
		else if constexpr (HaspSelf<T>::value) {
			unsigned long long adress;
			memcpy(&adress, &(pointer), sizeof(void*));
			return ((BaseA_c*)adress)->getFailed();
		}
	}
	template<typename ...T>
	requires HAS_pDerived_pSelf_constraint<T...>
	void setFailed(const bool& value, T ...t) {
		if constexpr (HaspDerived<NthType_t<0, T...>>::value && HaspSelf<NthType_t<0, T...>>::value) {
			((BaseA_c*)(
				(void*)((unsigned char*)unpack<0, T...>(t...).pDerivedAdress + offset)
				))->setFailed(value);
		}
		else if constexpr (HaspSelf<NthType_t<0, T...>>::value){
			unsigned long long adress;
			memcpy(&adress, &(pointer), sizeof(void*));
			((BaseA_c*)adress)->setFailed(value);
		}
	}
};
class BaseA_ptr_test {
protected:
	union {
		std::intptr_t offset;
		std::intptr_t pointer;
	};
};
template<typename Base, typename ...ForwardBy>
class BaseA_impl : public Base {
public:
	template<typename ...T>
	struct firstTypeClosure {
		using firstType = NthType_t<0, T...>;
	};
	template<>
	struct firstTypeClosure</*void*/> {
		using firstType = void;
	};
	template<typename ...T>
	using firstType = firstTypeClosure<T...>::firstType;

	BaseA_impl(ForwardBy ...t) {
		using firstTypeT = firstType<ForwardBy...>;
		constexpr const firstTypeT& firstParamT = [&]() {
			if constexpr ((sizeof...(t) != 0)) {
				return unpack<0, ForwardBy...>(t...);
			}
		}();
		if constexpr (sizeof...(t) == 0) {
			//static_assert(false, "this did not work");
			//do noghing (=default) I can not get the spezialisation to work, so thats the solution for now
		}
		else if constexpr (HaspDerived<firstTypeT>::value && HaspSelf<firstTypeT>::value) {
			const std::intptr_t& displacement = firstParamT.pSelfAdress - firstParamT.pDerivedAdress;
			memcpy(&(this->offset), &displacement, sizeof(void*));
		}
		else if constexpr (HaspSelf<firstTypeT>::value)	{
			const std::intptr_t& displacement = firstParamT.pSelfAdress;
			memcpy(&(this->pointer), &displacement, sizeof(void*));
		}
		else {
			static_assert(false, "to much arguments");
		};
	};
	//template<> BaseA_impl() { static_assert(false, "NICEE"); }//= default;

	template<typename ...T> bool getFailed(const T& ...t) const {
		using firstTypeT = firstType<T...>;
		constexpr auto firstParamT = []() constexpr {
			constexpr const bool& sizeNZero = (sizeof...(t) != 0);
			if constexpr (sizeNZero) return unpack<0, T...>(t...);
			else return false;
		}();
		if constexpr (sizeof...(t) == 0) {
			return ((Base*)this)->failed;
		}
		else if constexpr (HaspDerived<firstTypeT>::value && HaspSelf<firstTypeT>::value) {
			return ((Base*)(
				(void*)((unsigned char*)firstParamT.pDerivedAdress + this->offset)
				))->getFailed();
		}
		else if constexpr (HaspSelf<firstTypeT>::value) {
			unsigned long long adress;
			memcpy(&adress, &(this->pointer), sizeof(void*));
			return ((Base*)adress)->getFailed();
		}
		else {
			static_assert(false, "to much arguments");
		};
	}

	template<typename ...T> void setFailed(const bool& value, T ...t) {
		using firstTypeT = firstType<T...>;
		constexpr auto firstParamT = []() constexpr {
			constexpr const bool& sizeNZero = (sizeof...(t) != 0);
			if constexpr (sizeNZero) return unpack<0, T...>(t...);
			else return false;
		}();
		if constexpr (HaspDerived<firstTypeT>::value && HaspSelf<firstTypeT>::value) {
			((Base*)(
				(void*)((unsigned char*)firstParamT.pDerivedAdress + this->offset)
				))->setFailed(value);
		}
		else if constexpr (HaspSelf<firstTypeT>::value) {
			unsigned long long adress;
			memcpy(&adress, &(this.pointer), sizeof(void*));
			((Base*)adress)->setFailed(value);
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
	cFinal.derivedFromCBaseA.Dummy.cBaseA.failed = true;
	cout << baseA.getFailed(forwardByOffset) << endl;
	baseA.setFailed(false, forwardByOffset);
	cout << baseA.getFailed(forwardByOffset) << endl;
	baseA.setFailed(true, forwardByOffset);
	cout << baseA.getFailed(forwardByOffset) << endl;
	
	CFinal c1Final;
	ForwardByAddress forwardByAddress = {
		&c1Final.derivedFromCBaseA.Dummy.cBaseA
	};
	BaseA<1> base1A = BaseA<1>(forwardByAddress);
	c1Final.derivedFromCBaseA.Dummy.cBaseA.failed = false;
	cout << base1A.getFailed(forwardByAddress) << endl;
	base1A.setFailed(true, forwardByAddress);
	cout << base1A.getFailed(forwardByAddress) << endl;
	base1A.setFailed(false, forwardByAddress);
	cout << base1A.getFailed(forwardByAddress) << endl;

	BaseA<0> base2A = BaseA<0>();
	base2A.setFailed(true);
	cout << base2A.getFailed() << endl;
	base2A.setFailed(false);
	cout << base2A.getFailed() << endl;
	base2A.setFailed(true);
	cout << base2A.getFailed() << endl;

	std::cout << "259\n";
	BaseA_impl<BaseA_ptr_test, decltype(forwardByAddress)> baseA_impl = BaseA_impl<BaseA_ptr_test, decltype(forwardByAddress)>(forwardByAddress);
	//baseA_impl.getFailed();
	std::cout << "262\n";
	return 0;
};