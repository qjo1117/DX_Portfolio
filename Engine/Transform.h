#pragma once
#include "Component.h"

/*--------------
	Transform
---------------*/
class Transform : public Component
{
public:
	/* ----- Constructor Function ----- */
	Transform();
	virtual ~Transform();

	/* ----- External Function ----- */
	virtual void		FinalUpdate() override;
	virtual void		EditorUpdate() override;

	void				PushData();

public:
	/* ----- Serialize Function ----- */
	virtual void		Load(const wstring& path) override;
	virtual void		Save(const wstring& path) override;

	/* ----- Serializer Function ------ */
	virtual void		Serializer(class Json::Value& object);
	virtual void		DeSerializer(class Json::Value& object);
	
public:
	/* ----- Helper Function ----- */
	// Parent Bases
	const Vec3&			GetLocalPosition() { return m_localPosition; }
	const Vec3&			GetLocalRotation() { return m_localRotation; }
	const Vec3&			GetLocalScale() { return m_localScale; }

	// TEMP
	float				GetBoundingSphereRadius() { return max(max(m_localScale.x, m_localScale.y), m_localScale.z); }

	const Matrix&		GetLocalToWorldMatrix() { return m_matWorld; }
	Vec3				GetWorldPosition() { return m_matWorld.Translation(); }

	Vec3				GetRight() { return m_matWorld.Right(); }
	Vec3				GetUp() { return m_matWorld.Up(); }
	Vec3				GetForward() { return m_matWorld.Backward(); }

	void				SetLocalPosition(const Vec3& position) { m_localPosition = position; IsTransformChange(); }
	void				SetLocalRotation(const Vec3& rotation) { m_localRotation = rotation; IsTransformChange(); }
	void				SetLocalScale(const Vec3& scale) { m_localScale = scale; IsTransformChange(); }
	bool				IsChange() { return m_change; }

	void				LookAt(const Vec3& dir);
	static bool			CloseEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon());
	static Vec3			DecomposeRotationMatrix(const Matrix& rotation);

	static bool			AABB(Ref<Transform>& a, Ref<Transform>& b);


private:
	void IsTransformChange() {
		if (m_change == true) {
			return;
		}
		m_change = true;
		for (const weak_ptr<Transform>& child : m_childs) {
			child.lock()->IsTransformChange();
		}
	}
	
public:
	/* ----- Parent Helper Function ----- */
	void				SetParent(Ref<Transform> parent);
	weak_ptr<Transform> GetParent() { return m_parent; }
	vector<weak_ptr<Transform>>& GetChilds() { return m_childs; }

private:
	/* ----- Local Transform Variable ----- */
	PRIVATE_SET_EVENT_PROPERTY(Vec3, localPosition, IsTransformChange) = Vec3::Zero;
	PRIVATE_SET_EVENT_PROPERTY(Vec3, localRotation, IsTransformChange) = Vec3::Zero;
	PRIVATE_SET_EVENT_PROPERTY(Vec3, localScale, IsTransformChange) = Vec3::One;
	bool				m_change = false;

	/* ----- World, Parent Variable ----- */
	PRIVATE_PROPERTY(Matrix, matLocal) = Matrix::Identity;
	PRIVATE_PROPERTY(Matrix, matWorld) = Matrix::Identity;

	/* ----- Parent Mapping Variable ----- */
	weak_ptr<Transform>				m_parent;
	vector<weak_ptr<Transform>>		m_childs;
};

