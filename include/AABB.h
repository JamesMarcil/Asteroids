#ifndef AABB_H
#define AABB_H

// DirectX
#include <DirectXMath.h>

struct AABB
{
public:
    DirectX::XMFLOAT2 min, max;

    /*
     * TODO
     */
    AABB(DirectX::XMFLOAT2 min, DirectX::XMFLOAT2 max);

    /*
     * TODO
     */
    AABB(DirectX::XMFLOAT2 center, float halfWidth, float halfHeight);

    // Deleted and default constructors.
    AABB(void) = delete;
    AABB(const AABB& rhs) = default;
    AABB& operator=(const AABB& rhs) = default;
    AABB(AABB&& rhs) = default;
    AABB& operator=(AABB&& rhs) = default;

    /*
     * TODO
     */
    bool IntersectsWith(const AABB& rhs);

    /*
     * TODO
     */
    bool ContainsPoint(const DirectX::XMFLOAT2& point);
};

#endif
