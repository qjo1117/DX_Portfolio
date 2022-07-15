#pragma once
#include "BaseCollider.h"


class SphereCollider : public BaseCollider
{
public:
    SphereCollider();
    virtual ~SphereCollider();

    /* ----- override function ------ */
    virtual void Awake() override;
    virtual void FinalUpdate() override;
    virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) override;
    virtual bool Collision(Ref<BaseCollider> collider) override;
    virtual void EditorUpdate() override;

    void SetRadius(float radius) { _radius = radius; }
    float GetRadius() { return _radius; }

private:
    float _radius = 1.0f;
    PRIVATE_PROPERTY(BoundingSphere, BoundSphere) = {};
};

