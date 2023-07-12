//template<typename ...T> class Tuple {};

template <size_t N, typename... Ts>
struct NthType;

template <size_t N, typename T, typename... Ts>
struct NthType<N, T, Ts...> : NthType<N - 1, Ts...> {};

template <typename T, typename... Ts>
struct NthType<0, T, Ts...> {
	using type = T;
};

template <size_t N, typename... Ts>
using NthType_t = typename NthType<N, Ts...>::type;
