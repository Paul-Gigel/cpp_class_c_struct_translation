#include <iostream>
#include <concepts>
class ParameterPack {
	struct TypeInfo {
		bool isVoidType;
		bool notSameTypes;
	};
	//... parameter pack of Types and instances (by const ref)
public:
	template<typename T1, typename T2>
	constexpr static friend const bool operator==(T1 left, T2 right) noexcept {
		if constexpr (std::same_as<void, T2> || std::same_as<T1, void>) {
			return false;
		}
		else if constexpr (!std::same_as<T1, T2>) {
			return false;
		}
		else {
			return (left == right);
		}
	}
	template<typename T1>
	constexpr const bool operator==(T1 right) noexcept {};
	template<typename T>
	constexpr static const bool isVoidType(T t) noexcept {
		return (std::same_as<void, T>);
	}
	template<typename T>
	constexpr static const bool isClosureType(T t) noexcept {
		//not shure how to umplement, or if its possible at all
	}
	template<typename T1, typename T2>
	constexpr static const bool isSameType(const T1 left, const T2 right) noexcept {
		return std::same_as<T1, T2>;
	}
	template<typename T1, typename T2>
	constexpr static const bool isSameValue(const T1 left, const T2 right) noexcept {
		return (left == right);
	}
	template<const int nT, typename T1, typename ...Tn>
	constexpr const auto& unpack(const T1& t1, const Tn& ...tn) noexcept {
		if constexpr (nT == 0) return t1;
		else return unpack<nT - 1, Tn...>(tn...);
	}
};

int main() {

	ParameterPack parameterPack;
	std::cout << "hallo Welt\n";
	return 0;
}