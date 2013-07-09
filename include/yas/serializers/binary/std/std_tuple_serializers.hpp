
// Copyright (c) 2010-2012 niXman (i dot nixman dog gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef _yas__binary__std_tuple_serializer_hpp__included_
#define _yas__binary__std_tuple_serializer_hpp__included_

#include <yas/detail/config/config.hpp>

#if defined(YAS_HAS_STD_TUPLE)

#include <stdexcept>

#include <yas/detail/type_traits/type_traits.hpp>
#include <yas/detail/type_traits/properties.hpp>
#include <yas/detail/type_traits/selector.hpp>
#include <yas/detail/preprocessor/preprocessor.hpp>

#include <stdint.h>
#include <cassert>
#include <tuple>

namespace yas {
namespace detail {

/***************************************************************************/

#define YAS__BINARY__WRITE_STD_TUPLE_ITEM(unused, idx, type) \
	if ( std::is_fundamental<YAS_PP_CAT(type, idx)>::value ) \
		ar.write(reinterpret_cast<const char*>(&std::get<idx>(tuple)), sizeof(YAS_PP_CAT(type, idx))); \
	else \
		ar & std::get<idx>(tuple);

#define YAS__BINARY__READ_STD_TUPLE_ITEM(unused, idx, type) \
	if ( std::is_fundamental<YAS_PP_CAT(type, idx)>::value ) \
		ar.read(reinterpret_cast<char*>(&std::get<idx>(tuple)), sizeof(YAS_PP_CAT(type, idx))); \
	else \
		ar & std::get<idx>(tuple);

#define YAS__BINARY__GENERATE_EMPTY_SAVE_SERIALIZE_STD_TUPLE_FUNCTION_VARIADIC() \
	template<> \
	struct serializer<type_prop::not_a_pod,ser_method::use_internal_serializer, \
		archive_type::binary,direction::out,std::tuple<> \
	> { \
		template<typename Archive> \
		static Archive& apply(Archive& ar, const std::tuple<>&) { return ar; } \
	};

#define YAS__BINARY__GENERATE_EMPTY_LOAD_SERIALIZE_STD_TUPLE_FUNCTION_VARIADIC() \
	template<> \
	struct serializer<type_prop::not_a_pod,ser_method::use_internal_serializer, \
		archive_type::binary,direction::in,std::tuple<> \
	> { \
		template<typename Archive> \
		static Archive& apply(Archive& ar, std::tuple<>&) { return ar; } \
	};

#define YAS__BINARY__GENERATE_SAVE_SERIALIZE_STD_TUPLE_FUNCTION_VARIADIC(unused, count, text) \
	template<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), typename T)> \
	struct serializer<type_prop::not_a_pod,ser_method::use_internal_serializer, \
		archive_type::binary,direction::out, \
		std::tuple<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), T)> \
	> { \
		template<typename Archive> \
		static Archive& apply(Archive& ar, const std::tuple<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), T)>& tuple) { \
			const yas::uint8_t size = YAS_PP_INC(count); \
			ar.write(reinterpret_cast<const char*>(&size), sizeof(size)); \
			YAS_PP_REPEAT( \
				YAS_PP_INC(count), \
				YAS__BINARY__WRITE_STD_TUPLE_ITEM, \
				T \
			) \
			return ar; \
		} \
	};

#define YAS__BINARY__GENERATE_SAVE_SERIALIZE_STD_TUPLE_FUNCTIONS_VARIADIC(count) \
	YAS__BINARY__GENERATE_EMPTY_SAVE_SERIALIZE_STD_TUPLE_FUNCTION_VARIADIC() \
	YAS_PP_REPEAT( \
		count, \
		YAS__BINARY__GENERATE_SAVE_SERIALIZE_STD_TUPLE_FUNCTION_VARIADIC, \
		~ \
	)

#define YAS__BINARY__GENERATE_LOAD_SERIALIZE_STD_TUPLE_FUNCTION_VARIADIC(unused, count, text) \
	template<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), typename T)> \
	struct serializer<type_prop::not_a_pod,ser_method::use_internal_serializer, \
		archive_type::binary,direction::in, \
		std::tuple<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), T)> \
	> { \
		template<typename Archive> \
		static Archive& apply(Archive& ar, std::tuple<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), T)>& tuple) { \
			yas::uint8_t size = 0; \
			ar.read(reinterpret_cast<char*>(&size), sizeof(size)); \
			if ( size != YAS_PP_INC(count) ) throw std::runtime_error("size error on deserialize std::tuple"); \
			YAS_PP_REPEAT( \
				YAS_PP_INC(count), \
				YAS__BINARY__READ_STD_TUPLE_ITEM, \
				T \
			) \
			return ar; \
		} \
	};

#define YAS__BINARY__GENERATE_LOAD_SERIALIZE_STD_TUPLE_FUNCTIONS_VARIADIC(count) \
	YAS__BINARY__GENERATE_EMPTY_LOAD_SERIALIZE_STD_TUPLE_FUNCTION_VARIADIC() \
	YAS_PP_REPEAT( \
		count, \
		YAS__BINARY__GENERATE_LOAD_SERIALIZE_STD_TUPLE_FUNCTION_VARIADIC, \
		~ \
	)

/***************************************************************************/

#if _MSC_VER >= 1700
YAS__BINARY__GENERATE_SAVE_SERIALIZE_STD_TUPLE_FUNCTIONS_VARIADIC(_VARIADIC_MAX)
YAS__BINARY__GENERATE_LOAD_SERIALIZE_STD_TUPLE_FUNCTIONS_VARIADIC(_VARIADIC_MAX)
#else
YAS__BINARY__GENERATE_SAVE_SERIALIZE_STD_TUPLE_FUNCTIONS_VARIADIC(10)
YAS__BINARY__GENERATE_LOAD_SERIALIZE_STD_TUPLE_FUNCTIONS_VARIADIC(10)
#endif

/***************************************************************************/

} // namespace detail
} // namespace yas

#endif // defined(YAS_HAS_STD_TUPLE)

#endif // _yas__binary__std_tuple_serializer_hpp__included_
