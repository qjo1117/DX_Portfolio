#pragma once

#include "BaseCollider.h"


class BoxCollider :
    public BaseCollider
{
public:
    BoxCollider();
    virtual ~BoxCollider();

    /* ----- override function ------ */
    virtual void FinalUpdate() override;
    virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) override;
    virtual bool Collision(Ref<BaseCollider> collider) override;
    virtual void EditorUpdate() override;

private:
    PRIVATE_PROPERTY(BoundingOrientedBox, BoundBox) = {};
    PRIVATE_PROPERTY(Ref<GameObject>, WireFrame) = nullptr;
 
};

