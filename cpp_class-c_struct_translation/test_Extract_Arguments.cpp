#include <iostream>
#include <variant>
//#include "Extract_Types.h"
//#include "Extract_Arguments.h"

template<const int nT, typename T1, typename ...Tn>
constexpr auto unpack1(T1 t1, Tn ...tn) noexcept {
	if constexpr (nT == 0) {
		return t1;
	}
	else {
		return unpack1<nT - 1, Tn...>(tn...);
	};
}	//compiletime function
template<typename ...Tn> void test1(Tn ...tn) {
	std::cout << unpack1<0, Tn...>(tn...);
	std::cout << unpack1<1, Tn...>(tn...);
	std::cout << unpack1<2, Tn...>(tn...);
	std::cout << unpack1<3, Tn...>(tn...).value;
	std::cout << "\n";
}
struct Testclass {
	int value = 5678;
};
int main() {
	Testclass testclass = Testclass();
	test1("C", "A", 1234, testclass);
	return 0;
}