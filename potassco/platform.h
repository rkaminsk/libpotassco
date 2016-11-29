//
// Copyright (c) 2016, Benjamin Kaufmann
//
// This file is part of Potassco.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef POTASSCO_PLATFORM_H_INCLUDED
#define POTASSCO_PLATFORM_H_INCLUDED
#include <cstddef>
#include <cassert>
#include <cstdlib>

#if !defined(POTASSCO_HAS_STATIC_ASSERT)
#	if (defined(__cplusplus) && __cplusplus >= 201103L) || (defined(_MSC_VER) && _MSC_VER >= 1600) || (defined(static_assert) && !defined(_LIBCPP_VERSION))
#		define POTASSCO_HAS_STATIC_ASSERT 1
#	else
#		define POTASSCO_HAS_STATIC_ASSERT 0
#	endif
#endif

#if defined(_MSC_VER)
#define POTASSCO_ATTR_UNUSED
#define POTASSCO_STRING2(x) #x
#define POTASSCO_STRING(x) POTASSCO_STRING2(x)
#define POTASSCO_PRAGMA_TODO(X) __pragma(message(__FILE__ "(" POTASSCO_STRING(__LINE__) ") : TODO: " X))
#define POTASSCO_FUNC_NAME __FUNCTION__
#define POTASSCO_WARNING_BEGIN_RELAXED \
	__pragma(warning(push))\
	__pragma(warning (disable : 4200))

#define POTASSCO_WARNING_END_RELAXED \
	__pragma(warning(pop))

#elif defined(__GNUC__) || defined(__clang__)
#if !defined(__STDC_FORMAT_MACROS)
#define __STDC_FORMAT_MACROS
#endif
#if !defined(__STDC_LIMIT_MACROS)
#define __STDC_LIMIT_MACROS
#endif
#define POTASSCO_ATTR_UNUSED __attribute__((unused))
#define POTASSCO_FUNC_NAME __PRETTY_FUNCTION__
#define POTASSCO_APPLY_PRAGMA(x) _Pragma (#x)
#define POTASSCO_PRAGMA_TODO(x) POTASSCO_APPLY_PRAGMA(message ("TODO: " #x))
#	if defined(__clang__)
#		define POTASSCO_WARNING_BEGIN_RELAXED \
		_Pragma("clang diagnostic push") \
		_Pragma("clang diagnostic ignored \"-Wzero-length-array\"")
#		define POTASSCO_WARNING_END_RELAXED _Pragma("clang diagnostic pop")
#	else
#		define POTASSCO_WARNING_BEGIN_RELAXED \
		_Pragma("GCC diagnostic push")\
		_Pragma("GCC diagnostic ignored \"-Wpragmas\"")\
		_Pragma("GCC diagnostic ignored \"-Wpedantic\"")\
		_Pragma("GCC diagnostic ignored \"-pedantic\"")
#		define POTASSCO_WARNING_END_RELAXED _Pragma("GCC diagnostic pop")
#endif
#else
#define POTASSCO_ATTR_UNUSED
#define POTASSCO_FUNC_NAME __FILE__
#define POTASSCO_WARNING_BEGIN_RELAXED
#define POTASSCO_WARNING_END_RELAXED
#endif
#include <inttypes.h>

#if !defined(POTASSCO_ENABLE_PRAGMA_TODO) || POTASSCO_ENABLE_PRAGMA_TODO==0
#undef POTASSCO_PRAGMA_TODO
#define POTASSCO_PRAGMA_TODO(X)
#endif

#if POTASSCO_HAS_STATIC_ASSERT == 0
template <bool> struct static_assertion;
template <>     struct static_assertion<true> {};
#undef static_assert
#define static_assert(x, message) typedef bool clasp_static_assertion[sizeof(static_assertion< (x) >)] POTASSCO_ATTR_UNUSED
#endif

#if UINTPTR_MAX > UINT64_MAX
#error Unsupported platform!
#endif

namespace Potassco {
extern void fail(const char* function, unsigned line, int type, const char* fmt, ...);

} // namespace Potassco

#define POTASSCO_FAIL_IF(exp, ...) \
	(void)( (!(exp)) || (Potassco::fail(POTASSCO_FUNC_NAME, __LINE__, 0, __VA_ARGS__), std::abort(), 0))

#define POTASSCO_ASSERT_CONTRACT_MSG(exp, ...) \
	(void)( (!!(exp)) || (Potassco::fail(POTASSCO_FUNC_NAME, __LINE__, 1, __VA_ARGS__), std::abort(), 0))

#define POTASSCO_ASSERT_CONTRACT(exp) POTASSCO_ASSERT_CONTRACT_MSG(exp, #exp)

#if (defined(__cplusplus) && __cplusplus >= 201103L) || defined(_MSC_VER) || defined(_LIBCPP_VERSION)
#define POTASSCO_EXT_INCLUDE(x) <x>
#if !defined(_MSC_VER) || _MSC_VER > 1500
#define POTASSCO_EXT_NS std
#else
#define POTASSCO_EXT_NS std::tr1
#endif
#else
#define POTASSCO_EXT_INCLUDE(x) <tr1/x>
#define POTASSCO_EXT_NS std::tr1
#endif

#endif