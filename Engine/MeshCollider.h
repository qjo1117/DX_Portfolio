#pragma once

#include "BaseCollider.h"

class Triangle
{
public:
    Vec3 pos0 = Vec3::Zero;
    Vec3 pos1 = Vec3::Zero;
    Vec3 pos2 = Vec3::Zero;
};

class MeshCollider :
    public BaseCollider
{
public:
    MeshCollider();
    virtual ~MeshCollider();

    /* ----- override function ------ */
    virtual void Awake() override;
    virtual void FinalUpdate() override;
    virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) override;
    virtual bool Collision(Ref<BaseCollider> collider) override;
    virtual void EditorUpdate() override;


private:
    PRIVATE_PROPERTY(vector<Triangle>, BoundMesh) = {};
    PRIVATE_PROPERTY(Vec3, Center) = Vec3::Zero;
    PRIVATE_PROPERTY(Vec3, Scale) = Vec3::Zero;
};

