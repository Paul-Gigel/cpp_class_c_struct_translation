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