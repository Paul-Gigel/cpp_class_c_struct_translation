//https://stackoverflow.com/questions/1005476/how-to-detect-whether-there-is-a-specific-member-variable-in-class
//SFINEA!!!

#define HAS_MEMBER_VARIABLE_MACRO(member)\
template<typename T> struct Has##member {\
	struct FallbackType {};\
	struct Fallback { FallbackType member; };\
	struct Derived : T, Fallback {};\
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