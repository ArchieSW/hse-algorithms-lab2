#pragma once
#include <type_traits>

#ifndef __has_feature       // Optional of course.
#define __has_feature(x) 0  // Compatibility with non-clang compilers.
#endif
#ifndef __has_extension
#define __has_extension __has_feature  // Compatibility with pre-3.0 compilers.
#endif

#if __has_extension(nullability)
template <typename T, typename std::enable_if_t<std::is_pointer_v<T>>* = nullptr>
using Nonnull = T _Nonnull;  // NOLINT
#else
template <typename T, typename std::enable_if_t<std::is_pointer_v<T>>* = nullptr>
using Nonnull = T;  // NOLINT
#endif
