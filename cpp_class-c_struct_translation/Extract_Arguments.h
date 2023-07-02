template<int nT, typename T1, typename ...Tn> constexpr NthType<nT, T1, Tn...> unpack(T1 t1, Tn ...tn) noexcept {
	if (nT == 0) {
		return t1;
	}
	else {
		nT--;
		return unpack<nT, Tn...>(tn...);
	};
}	//compiletime function

template<typename RetType, const int nT, typename T1, typename ...Tn> 
constexpr RetType unpack1(T1 t1, Tn ...tn) noexcept {
	if constexpr (nT == 0) {
		return t1;
	}
	else {
		return unpack1<RetType, nT-1, Tn...>(tn...);
	};
}	//compiletime function