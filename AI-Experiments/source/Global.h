// ReSharper disable CppExplicitSpecializationInNonNamespaceScope
// ReSharper disable CppClangTidyBugproneMacroParentheses
#pragma once

#include <glm/vec2.hpp>

#include <aie/bootstrap/Vector2.h>

using glm::vec2;

using aie::Vector2;

extern vec2 AieToGlm(Vector2 vec);
extern Vector2 GlmToAie(vec2 vec);

extern "C++"
{
    template <size_t S>
    struct EnumFlagIntegerForSize;

    template <>
    struct EnumFlagIntegerForSize<1>
    {
        typedef signed char type;
    };

    template <>
    struct EnumFlagIntegerForSize<2>
    {
        typedef signed short type;
    };

    template <>
    struct EnumFlagIntegerForSize<4>
    {
        typedef signed int type;
    };

    template <>
    struct EnumFlagIntegerForSize<8>
    {
        typedef signed long type;
    };

    // used as an approximation of std::underlying_type<T>
    template <class T>
    struct EnumFlagSizedInteger
    {
        typedef typename EnumFlagIntegerForSize<sizeof(T)>::type type;
    };

}

#define ENUM_FLAG_OPERATORS(TYPE) \
extern "C++" \
{ \
	inline constexpr TYPE operator | (TYPE a, TYPE b) noexcept { return TYPE(((EnumFlagSizedInteger<TYPE>::type)a) | ((EnumFlagSizedInteger<TYPE>::type)b)); } \
	inline TYPE &operator |= (TYPE &a, TYPE b) noexcept { return (TYPE &)(((EnumFlagSizedInteger<TYPE>::type &)a) |= ((EnumFlagSizedInteger<TYPE>::type)b)); } \
	inline constexpr TYPE operator & (TYPE a, TYPE b) noexcept { return TYPE(((EnumFlagSizedInteger<TYPE>::type)a) & ((EnumFlagSizedInteger<TYPE>::type)b)); } \
	inline TYPE &operator &= (TYPE &a, TYPE b) noexcept { return (TYPE &)(((EnumFlagSizedInteger<TYPE>::type &)a) &= ((EnumFlagSizedInteger<TYPE>::type)b)); } \
	inline constexpr TYPE operator ~ (TYPE a) noexcept { return TYPE(~((EnumFlagSizedInteger<TYPE>::type)a)); } \
	inline constexpr TYPE operator ^ (TYPE a, TYPE b) noexcept { return TYPE(((EnumFlagSizedInteger<TYPE>::type)a) ^ ((EnumFlagSizedInteger<TYPE>::type)b)); } \
	inline TYPE &operator ^= (TYPE &a, TYPE b) noexcept { return (TYPE &)(((EnumFlagSizedInteger<TYPE>::type &)a) ^= ((EnumFlagSizedInteger<TYPE>::type)b)); } \
}