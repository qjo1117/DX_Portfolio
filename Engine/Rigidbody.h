#pragma once
#include "Component.h"
class Rigidbody :
    public Component
{
public:
    Rigidbody();
    virtual ~Rigidbody();

    virtual void Awake() override;
    virtual void Update() override;
    virtual void FinalUpdate() override;

public:
    /* ------ Register Function ----- */
    void RigidbodyCollision(Ref<class BaseCollider> p_collider);

public:
    void AddForce(Vec3 p_force);

private:
    Ref<class BaseCollider> m_collider;
    Vec3 m_force = Vec3::Zero;
};

