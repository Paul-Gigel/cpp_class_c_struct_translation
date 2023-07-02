#include <iostream>
#include "Check_If_Has_Member.h"

template<typename Self, typename Derived>
struct ForwardByOffset {
	Self* pSelf;
	Derived* pDerived;
};
template<typename Self>
struct ForwardByAddress {
	Self* pSelf;
};
ForwardByAddress<int> forwardByAddress;
ForwardByOffset<int, char> forwardByOffset;
int main() {
	std::cout<<HasPDerived<ForwardByAddress<int>>::value<<"\n";
	std::cout<<HasPSelf<ForwardByAddress<int>>::value<<"\n";
	return 0;
}