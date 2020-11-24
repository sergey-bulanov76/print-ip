#include <cstddef> // std::size_t
#include <typeinfo> // typeid
#include <iostream>
#include <type_traits> // std::is_...
#include <vector>
#include <list>
#include <tuple>

#include "is_stl_container.h"

template <class Tp>
std::size_t tuple_size(Tp t) {
	return std::tuple_size<Tp>::value;
}

template <typename T1, typename... T2>
constexpr bool check_for_type(std::tuple<T2...>) {
	return std::disjunction_v<std::is_same<T1, T2>...>;
}

template <typename T> // integral types
auto print_ip(const T& someIntType)
-> std::enable_if_t<std::is_integral<T>::value>
{
	const uint8_t* val_lsb = reinterpret_cast<const uint8_t*>(&someIntType) + sizeof(someIntType) - 1; // pointer to the lsb
	for (auto it = 0; it < sizeof(someIntType); ++it) {
		if (it != 0) {
			std::cout << ".";
		}
		std::cout << +*val_lsb;
		--val_lsb;
	}
	std::cout << std::endl;
}

template <typename T> // vector, list
auto print_ip(const T& container)
-> std::enable_if_t<is_stl_container<T>::value>
{
	for (auto it = container.begin(); it != container.end(); ++it) {
		if (it != container.begin()) {
			std::cout << ".";
		}
		std::cout << +*it;
	}
	std::cout << std::endl;
}

auto print_ip(const std::string& s) { // string
	std::cout << s << std::endl;
}

template<size_t I = 0, typename... Tp> // tuple
void print_ip(std::tuple<Tp...>& t) {
	std::cout << std::get<I>(t);
	if (I < tuple_size(t) - 1) std::cout << ".";
	else std::cout << std::endl;
	if constexpr (I + 1 != sizeof...(Tp))
		print_ip<I + 1>(t);
}

template<size_t I = 0, typename... Tp> // tuple
void check_type(std::tuple<Tp...>& t) {
	std::cout << std::get<I>(t);
	if (I < tuple_size(t) - 1) std::cout << ".";
	else std::cout << std::endl;
	if constexpr (I + 1 != sizeof...(Tp))
		print_ip<I + 1>(t);
}

template <size_t I = 0, typename... Ts>
void printTuple(std::tuple<Ts...>& tup) {
	if constexpr (I == sizeof...(Ts)) {
		return;
	}
	else {
		std::cout << std::get<I>(tup);
		if (I < sizeof...(Ts) - 1) std::cout << ".";
		else std::cout << std::endl;
		printTuple<I + 1>(tup);
	}
}

template <size_t I = 0, typename... Ts>
constexpr bool checkType(std::tuple<Ts...>& tup) {
	if constexpr (I == (sizeof...(Ts) - 1)) {
		std::cout << I << std::endl;
		return true;
	}
	else {
		std::cout << I << std::endl;
		if (typeid(std::get<I>(tup)).name() ==
			typeid(std::get<I + 1>(tup)).name()) {
			checkType<I + 1>(tup);
		}
		return false;
	}
}

template <size_t I = 0, typename... Ts>
constexpr bool checkType1(std::tuple<Ts...>& tup) {
	if constexpr (I == (sizeof...(Ts) - 1)) {
		std::cout << I << std::endl;
		return true;
	}
	else {
		std::cout << I << std::endl;
		std::tuple_element<I, std::tuple<Ts...>>::type x;
		std::tuple_element<I + 1, std::tuple<Ts...>>::type y;
		if (typeid(x).name() ==
			typeid(y).name()) {
			checkType1<I + 1>(tup);
		}
		return false;
	}
}


int main() {
	std::vector<int> v{ 46, 45, 44, 43, 42 };
	std::tuple<int, int, int, int, int> t{ 48, 49, 50, 51, 52 };
	std::tuple<int, std::string, int, int> t1{ 48, "trololo", 50, 51 };
	std::tuple<int, int, int, int, int64_t> t2{ 48, 49, 50, 51, 52 };
	std::list<int> l = { 52, 53, 54, 55 };
	std::string s{ "trololo" };
	int16_t foo{ -1 };
	uint64_t foo1{ 0xFF'FF'FD'FD'FB'FA'FF'09 };
	uint32_t foo2{ 0xFF'FE'FD'FC };
	auto foo3 = char(-1);
	auto foo4 = short(0);
	auto foo5 = int(2130706433);
	auto foo6 = long(8875824491850138409);


	std::cout << foo << " as ";
	print_ip(foo);
	std::cout << foo1 << " as ";
	print_ip(foo1);
	std::cout << foo2 << " as ";
	print_ip(foo2);
	//std::cout << foo3 << " as ";
	//print_ip(foo3);

	std::cout << "vector as ";
	print_ip(v);
	std::cout << "list as ";
	print_ip(l);
	std::cout << "string as ";
	print_ip(s);
	std::cout << "tuple as ";
	// print_ip(t);
	std::cout << "tuple t" << std::endl;
	printTuple(t);
	std::cout << checkType1(t) << std::endl;
	std::cout << "tuple t1" << std::endl;
	printTuple(t1);
	std::cout << checkType1(t1) << std::endl;
	std::cout << "tuple t2" << std::endl;
	printTuple(t2);
	std::cout << checkType1(t2) << std::endl;

	// std::apply([](auto&&... args) {((std::cout << args << '.'), ...); }, t); print tuple
	// std::tuple_element<0, decltype(t)>::type first = std::get<0>(t);
	std::tuple_element<0, decltype(t)>::type x;
	std::cout << typeid(x).name() << std::endl;
}