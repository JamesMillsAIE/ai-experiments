#include "Physics.h"

bool Physics::CheckCollisionLines(vec2 start1, vec2 end1, vec2 start2, vec2 end2, vec2* collisionPoint)
{
    bool collision = false;

    float div = (end2.y - start2.y) * (end1.x - start1.x) - (end2.x - start2.x) * (end1.y - start1.y);

    if (fabsf(div) >= FLT_EPSILON)
    {
        collision = true;

        float xi = ((start2.x - end2.x) * (start1.x * end1.y - start1.y * end1.x) - (start1.x - end1.x) * (start2.x * end2.y - start2.y * end2.x)) / div;
        float yi = ((start2.y - end2.y) * (start1.x * end1.y - start1.y * end1.x) - (start1.y - end1.y) * (start2.x * end2.y - start2.y * end2.x)) / div;

        if ((fabsf(start1.x - end1.x) > FLT_EPSILON && (xi < fminf(start1.x, end1.x) || xi > fmaxf(start1.x, end1.x))) ||
            (fabsf(start2.x - end2.x) > FLT_EPSILON && (xi < fminf(start2.x, end2.x) || xi > fmaxf(start2.x, end2.x))) ||
            (fabsf(start1.y - end1.y) > FLT_EPSILON && (yi < fminf(start1.y, end1.y) || yi > fmaxf(start1.y, end1.y))) ||
            (fabsf(start2.y - end2.y) > FLT_EPSILON && (yi < fminf(start2.y, end2.y) || yi > fmaxf(start2.y, end2.y))))
        {
	        collision = false;
        }

        if (collision && collisionPoint != nullptr)
        {
            collisionPoint->x = xi;
            collisionPoint->y = yi;
        }
    }

    return collision;
}

bool Physics::CheckCollisionPointTriangle(const vec2 point, const vec2 p1, const vec2 p2, const vec2 p3)
{
    bool collision = false;

    const float alpha = ((p2.y - p3.y) * (point.x - p3.x) + (p3.x - p2.x) * (point.y - p3.y)) /
        ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));

    const float beta = ((p3.y - p1.y) * (point.x - p3.x) + (p1.x - p3.x) * (point.y - p3.y)) /
        ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));

    const float gamma = 1.0f - alpha - beta;

    if (alpha > 0 && beta > 0 && gamma > 0)
    {
	    collision = true;
    }

    return collision;
}
