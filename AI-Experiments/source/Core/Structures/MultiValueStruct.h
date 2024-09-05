#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;

enum EStructValueType
{
	Float,
	Int,
	Char,
	Bool,
	Vec2,
	Vec3,
	Vec4,
	Pointer
};

struct MultiValueStruct
{
public:
	union
	{
		float fValue;
		int iValue;
		char cValue;
		bool bValue;
		vec2 v2Value;
		vec3 v3Value;
		vec4 v4Value;
		void* pValue;
	};

public:
	EStructValueType type;

};