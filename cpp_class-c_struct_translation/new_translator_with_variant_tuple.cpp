#include <iostream>
#include <tuple>
#include <variant>

#define HAS_MEMBER_VARIABLE_MACRO(member)\
template<typename T> struct Has##member {\
	struct FallbackType {};\
	struct Fallback { FallbackType member; };\
	struct Derived : T, Fallback {\
		constexpr Derived(T t) : T(t) {};\
	};\
	template<typename C, C> struct ChT;\
	template<typename C> consteval static char(&f(ChT<FallbackType Fallback::*, &C::member>*))[1];\
	template<typename C> consteval static char(&f(...))[2];\
	constexpr static bool const value = sizeof(f<Derived>(0)) == 2;\
};\
template<> struct Has##member<void>	{\
	constexpr static bool const value = false;\
};

HAS_MEMBER_VARIABLE_MACRO(pDerived);
HAS_MEMBER_VARIABLE_MACRO(pSelf);

template<typename Self, typename Derived>
struct ForwardByOffset {
	union pSelf{
		Self* pSelf;
		std::intptr_t pSelfAdress;
	};
	union pDerived{
		Derived* pDerived;
		std::intptr_t pDerivedAdress;
	};
	ForwardByOffset(Self* pSelf, Derived* pDerived) : pSelf::pSelf(pSelf), pDerived::pDerived(pDerived) {};
};
template<typename Self>
struct ForwardByAddress {
	union{
		Self* pSelf;
		std::intptr_t pSelfAdress;
	}pSelf;
	ForwardByAddress(Self* pSelf) : 
		pSelf({ pSelf }) {};
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
		class ForwardBySafe : public 
			HaspSelf<T>::Derived,
			HaspDerived<T>::Derived
		{
			constexpr ForwardBySafe(const T& t) : 
				HaspSelf<T>::Derived(t),
				HaspDerived<T>::Derived(t) {}
		};
		//auto& [pSelf, pDerived] = ForwardBySafe{T}; //thats what I want...
	};
	template<typename T>
	bool getFailed(const T& t) const {
		return false;
	}
	template<typename ...T>
	void setFailed(const bool& value, T ...t) {
	}
};

int main() {
	CBaseA cBaseA;
	ForwardByAddress forwardByAddress(&cBaseA);
	BaseA_ptr baseA_ptr(forwardByAddress);
	return 0;
}