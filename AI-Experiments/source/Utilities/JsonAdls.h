#pragma once

#include <aie/bootstrap/Color.h>

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

using aie::Color;

namespace nlohmann
{
	template<>
	struct adl_serializer<Color>
	{
		static void to_json(Json& j, const Color& color)
		{
			j["r"] = color.r;
			j["g"] = color.g;
			j["b"] = color.b;
			j["a"] = color.a;
		}

		static void from_json(const Json& j, aie::Color& color)
		{
			if (j.is_null())
			{
				color = { 255, 255, 255, 255 };
			}
			else
			{
				color.r = j["r"];
				color.g = j["g"];
				color.b = j["b"];
				color.a = j["a"];
			}
		}
	};
}