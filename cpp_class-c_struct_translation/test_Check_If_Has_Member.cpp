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
int main() {
	std::cout << HaspDerived<ForwardByOffset<int, char>>::value << "\n";
	std::cout << HaspSelf<ForwardByOffset<int, char>>::value << "\n";

	std::cout << HaspDerived<ForwardByAddress<int>>::value << "\n";
	std::cout << HaspSelf<ForwardByAddress<int>>::value << "\n";
	return 0;
}