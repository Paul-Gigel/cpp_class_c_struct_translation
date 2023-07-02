#include <iostream>
#include <variant>
//#include "Extract_Types.h"
#include "Extract_Arguments.h"

template<typename ...Tn> void test1(Tn ...tn) {
	std::cout << unpack<0, Tn...>(tn...);
	std::cout << unpack<1, Tn...>(tn...);
	std::cout << unpack<2, Tn...>(tn...);
	std::cout << unpack<3, Tn...>(tn...).value;
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