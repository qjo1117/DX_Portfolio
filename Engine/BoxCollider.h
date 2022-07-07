#pragma once

#include "BaseCollider.h"


class BoxCollider :
    public BaseCollider
{
public:
    BoxCollider();
    virtual ~BoxCollider();

    /* ----- override function ------ */
    virtual void Awake() override;
    virtual void FinalUpdate() override;
    virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) override;
    virtual bool Collision(Ref<BaseCollider> collider) override;
    virtual void EditorUpdate() override;

    virtual bool RayCast(Vec3 rayOrin, Vec3 rayDir, OUT RayCastHit& hit, float distance = FLT_MAX);

private:
    PRIVATE_PROPERTY(BoundingOrientedBox, BoundBox) = {};
};

