#include <iostream>
#include <concepts>
using namespace std;
// cpp_class-c_struct translation.cpp: Definiert den Einstiegspunkt für die Anwendung.
//
template<typename ...T> class Tuple{};

template <std::size_t N, typename... Ts>
struct NthType;

template <std::size_t N, typename T, typename... Ts>
struct NthType<N, T, Ts...> : NthType<N - 1, Ts...> {};

template <typename T, typename... Ts>
struct NthType<0, T, Ts...> {
	using type = T;
};

template <std::size_t N, typename... Ts>
using NthType_t = typename NthType<N, Ts...>::type;

template<int nT, typename T1, typename ...Tn> constexpr NthType<nT, T1, Tn...> unpack(T1 t1, Tn ...tn) noexcept {
	if (nT == 0) {
		return t1;
	}
	else {
		nT--;
		return unpack<Tn..., nT>(tn...);
	};
}	//compiletime function

//SFINEA!!!
template<typename T> struct HasPDerived {
	struct Fallback	{int pDerived;};
	struct pDerived : T, Fallback {};

	template<typename C, C> struct ChT;

	template<typename C> static char(&f(ChT<int Fallback::*, &C::pDerived>*))[1];
	template<typename C> static char(&f(...))[2];

	static bool const value = sizeof(f<pDerived>(0)) == 2;
};
template<typename T> struct HasPSelf {
	struct Fallback { int pSelf; };
	struct pSelf : T, Fallback {};

	template<typename C, C> struct ChT;

	template<typename C> static char(&f(ChT<int Fallback::*, &C::pSelf>*))[1];
	template<typename C> static char(&f(...))[2];

	static bool const value = sizeof(f<pSelf>(0)) == 2;
};

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
			unsigned long long displacement = (unsigned long long)unpack<0, T...>(t...).pSelf - (unsigned long long)unpack<T..., 0>(t...).pDerived;
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
		if constexpr (std::is_same<Tuple<T...>, Tuple<ForwardByOffset<BaseA, int>>>::value) {
			return ((BaseA*)(
				(void*)((unsigned char*)unpack<0, T...>(t...).pderived + this->failed)
				))->failed;
		}
		if constexpr (std::is_same<Tuple<T...>, Tuple<ForwardByAddress<BaseA>>>::value)	{
			unsigned long long adress;
			memcpy(&adress, &(this->failed), sizeof(void*));
 			return ((BaseA*)adress)->failed;
		} else	{
			return this->failed;
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
	cFinal.derivedFromCBaseA.Dummy.cBaseA.failed = false;
	ForwardByOffset<BaseA, CFinal> forwardByOffset = {
		(BaseA*)&cFinal.derivedFromCBaseA.Dummy.cBaseA,
		&cFinal
	};
	BaseA baseA = BaseA(forwardByOffset);
	//cout << baseA.getFailed(&cFinal) << endl;
	
	CFinal c1Final;
	c1Final.derivedFromCBaseA.Dummy.cBaseA.failed = true;
	/*ForwardByAddress<BaseA> forwardByAddress = {
		(BaseA*)&c1Final.derivedFromCBaseA.Dummy.cBaseA
	};
	BaseA base1A = BaseA(forwardByAddress);
	cout << base1A.getFailed() << endl;
	*/
	return 0;
}