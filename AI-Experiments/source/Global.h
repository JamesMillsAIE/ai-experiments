#pragma once

#include <glm/vec2.hpp>

#include <aie/bootstrap/Vector2.h>

using glm::vec2;

using aie::Vector2;

extern vec2 AieToGlm(Vector2 vec);
extern Vector2 GlmToAie(vec2 vec);