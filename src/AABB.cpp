#include "AABB.h"

using namespace DirectX;

/*
 * TODO
 */
AABB::AABB(DirectX::XMFLOAT2 min, DirectX::XMFLOAT2 max)
    : min(min), max(max)
{
    /* Nothing to do. */
}

/*
 * TODO
 */
AABB::AABB(DirectX::XMFLOAT2 center, float halfWidth, float halfHeight)
{
    min = XMFLOAT2{center.x - halfWidth, center.y - halfHeight};
    max = XMFLOAT2{center.x + halfWidth, center.y + halfHeight};
}

/*
 * TODO
 */
bool AABB::IntersectsWith(const AABB& rhs)
{
    if(max.x < rhs.min.x || min.x > rhs.max.x || max.y < rhs.min.y || min.y > rhs.max.y)
    {
        return false;
    }

    return true;
}

/*
 * TODO
 */
bool AABB::ContainsPoint(const DirectX::XMFLOAT2& point)
{
    if(point.x < min.x || point.x > max.x || point.y < min.y || point.y > max.y)
    {
        return false;
    }

    return true;
}
