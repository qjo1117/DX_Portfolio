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
    
    PRIVATE_PROPERTY(Vec3, force) = Vec3::Zero;
    PRIVATE_PROPERTY(Vec3, velocity) = Vec3::Zero;
    PRIVATE_PROPERTY(float, friction) = 0.1f;
    PRIVATE_PROPERTY(float, mass) = 1.0f;
    PRIVATE_PROPERTY(float, e) = 0.8f;
};

