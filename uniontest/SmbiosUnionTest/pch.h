//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <gtest\gtest.h>
#include <tchar.h>

#include <type_traits>
#include <functional>

template<typename T>
struct function_traits;

template<typename R, typename ...Args>
struct function_traits<std::function<R(Args...)>>
{
	static const size_t nargs = sizeof...(Args);

	typedef R result_type;

	template <size_t i>
	struct arg
	{
		typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
	};
};

template<class R, class C, typename ...Args>
R GetReturnType(R(C::*)(Args...));
