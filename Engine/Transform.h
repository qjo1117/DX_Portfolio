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
	const Vec3&			GetLocalPosition() { return _localPosition; }
	const Vec3&			GetLocalRotation() { return _localRotation; }
	const Vec3&			GetLocalScale() { return _localScale; }

	// TEMP
	float				GetBoundingSphereRadius() { return max(max(_localScale.x, _localScale.y), _localScale.z); }

	const Matrix&		GetLocalToWorldMatrix() { return _matWorld; }
	Vec3				GetWorldPosition() { return _matWorld.Translation(); }

	Vec3				GetRight() { return _matWorld.Right(); }
	Vec3				GetUp() { return _matWorld.Up(); }
	Vec3				GetForward() { return _matWorld.Backward(); }

	void				SetLocalPosition(const Vec3& position) { _localPosition = position; IsTransformChange(); }
	void				SetLocalRotation(const Vec3& rotation) { _localRotation = rotation; IsTransformChange(); }
	void				SetLocalScale(const Vec3& scale) { _localScale = scale; IsTransformChange(); }
	bool				IsChange() { return _change; }

	void				LookAt(const Vec3& dir);
	static bool			CloseEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon());
	static Vec3			DecomposeRotationMatrix(const Matrix& rotation);

	static bool			AABB(Ref<Transform>& a, Ref<Transform>& b);


private:
	void IsTransformChange() {
		if (_change == true) {
			return;
		}
		_change = true;
		for (const weak_ptr<Transform>& child : _childs) {
			child.lock()->IsTransformChange();
		}
	}
	
public:
	/* ----- Parent Helper Function ----- */
	void				SetParent(Ref<Transform> parent);
	weak_ptr<Transform> GetParent() { return _parent; }
	vector<weak_ptr<Transform>>& GetChilds() { return _childs; }

private:
	/* ----- Local Transform Variable ----- */
	Vec3				_localPosition = {};
	Vec3				_localRotation = {};
	Vec3				_localScale = { 1.0f, 1.0f ,1.0f };
	bool				_change = false;

	/* ----- World, Parent Variable ----- */
	Matrix				_matLocal = {};
	Matrix				_matWorld = {};

	/* ----- Parent Mapping Variable ----- */
	weak_ptr<Transform>				_parent;
	vector<weak_ptr<Transform>>		_childs;
};

