//SFINEA!!!
template<typename T> struct HasPDerived {
	struct Fallback { int pDerived; };
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