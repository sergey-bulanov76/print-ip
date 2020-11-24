#pragma once

#include <list>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

//specialize a type for all of the STL containers.
namespace is_stl_container_impl {
	template <typename T>       struct is_stl_container :std::false_type {};
	//template <typename T, std::size_t N> struct is_stl_container<std::array    <T, N>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::vector            <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::list              <Args...>> :std::true_type {};
	// template <typename... Args> struct is_stl_container<std::tuple     <Args...>> :std::true_type {};
	// template <> struct is_stl_container<std::string> :std::true_type {};
}

//type trait to utilize the implementation type traits as well as decay the type
template <typename T> struct is_stl_container {
	static constexpr bool const value = is_stl_container_impl::is_stl_container<std::decay_t<T>>::value;
};