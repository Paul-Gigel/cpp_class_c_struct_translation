#pragma once
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