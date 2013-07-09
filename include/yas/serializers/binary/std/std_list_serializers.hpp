
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

#ifndef _yas__binary__std_list_serializer_hpp__included_
#define _yas__binary__std_list_serializer_hpp__included_

#include <yas/detail/type_traits/type_traits.hpp>
#include <yas/detail/type_traits/properties.hpp>
#include <yas/detail/type_traits/selector.hpp>

#include <list>

namespace yas {
namespace detail {

/***************************************************************************/

template<typename T>
struct serializer<
	type_prop::not_a_pod,
	ser_method::use_internal_serializer,
	archive_type::binary,
	direction::out,
	std::list<T>
> {
	template<typename Archive>
	static Archive& apply(Archive& ar, const std::list<T>& list) {
		const yas::uint32_t size = list.size();
		ar.write(reinterpret_cast<const char*>(&size), sizeof(size));
		if ( std::is_fundamental<T>::value ) {
			typename std::list<T>::const_iterator it = list.begin();
			for ( ; it != list.end(); ++it ) {
				ar.write(reinterpret_cast<const char*>(&(*it)), sizeof(T));
			}
		} else {
			typename std::list<T>::const_iterator it = list.begin();
			for ( ; it != list.end(); ++it ) {
				ar & (*it);
			}
		}
		return ar;
	}
};

template<typename T>
struct serializer<
	type_prop::not_a_pod,
	ser_method::use_internal_serializer,
	archive_type::binary,
	direction::in,
	std::list<T>
> {
	template<typename Archive>
	static Archive& apply(Archive& ar, std::list<T>& list) {
		yas::uint32_t size = 0;
		ar.read(reinterpret_cast<char*>(&size), sizeof(size));
		list.resize(size);
		if ( std::is_fundamental<T>::value ) {
			typename std::list<T>::iterator it = list.begin();
			for ( ; it != list.end(); ++it ) {
				ar.read(reinterpret_cast<char*>(&(*it)), sizeof(T));
			}
		} else {
			typename std::list<T>::iterator it = list.begin();
			for ( ; it != list.end(); ++it ) {
				ar & (*it);
			}
		}
		return ar;
	}
};

/***************************************************************************/

} // namespace detail
} // namespace yas

#endif // _yas__binary__std_list_serializer_hpp__included_
