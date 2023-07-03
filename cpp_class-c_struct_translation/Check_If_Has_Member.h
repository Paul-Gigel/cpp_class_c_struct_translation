//https://stackoverflow.com/questions/1005476/how-to-detect-whether-there-is-a-specific-member-variable-in-class
//SFINEA!!!
template<typename T> struct HasPDerived {
	struct Fallback { int pDerived; };
	struct Derived : T, Fallback {};

	template<typename C, C> struct ChT;

	template<typename C> static char(&f(ChT<int Fallback::*, &C::pDerived>*))[1];
	//if the above Template results in ambiguity, the Compiler rejects it and taks the below one
	template<typename C> static char(&f(...))[2];

	static bool const value = sizeof(f<Derived>(0)) == 2;
};
template<typename T> struct HasPSelf {
	struct Fallback { int pSelf; };
	struct Derived : T, Fallback {};

	template<typename C, C> struct ChT;

	template<typename C> static char(&f(ChT<int Fallback::*, &C::pSelf>*))[1];
	template<typename C> static char(&f(...))[2];

	static bool const value = sizeof(f<Derived>(0)) == 2;
};