#pragma once
#include "BaseCollider.h"


class SphereCollider : public BaseCollider
{
public:
    SphereCollider();
    virtual ~SphereCollider();

    /* ----- override function ------ */
    virtual void FinalUpdate() override;
    virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) override;


    void SetRadius(float radius) { _radius = radius; }
    float GetRadius() { return _radius; }

private:
    float _radius = 1.0f;
    BoundingSphere _boundingSphere = {};
};

