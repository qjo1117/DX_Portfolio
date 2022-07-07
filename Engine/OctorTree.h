#pragma once

struct Octor
{
	Vec3 center = Vec3::Zero;
	Vec3 extents = Vec3::One;

	bool CollisionBoxToPoint(Vec3 pos) {
		if (center.x - extents.x <= pos.x && pos.x <= center.x + extents.x) {
			if (center.y - extents.y <= pos.y && pos.y <= center.y + extents.y) {
				if (center.z - extents.z <= pos.z && pos.z <= center.z + extents.z) {
					return true;
				}
			}
		}
		return false;
	}
};

enum class OCTOR_TREE_TYPE
{
	RIGHT_UP_FORWARD,
	RIGHT_UP_BACKWARD,
	LEFT_UP_FORWARD,
	LEFT_UP_BACKWARD,
	RIGHT_DOWN_FORWARD,
	RIGHT_DOWN_BACKWARD,
	LEFT_DOWN_FORWARD,
	LEFT_DOWN_BACKWARD,
	END,
};

int32 EnumToInt32(OCTOR_TREE_TYPE type);


class OctorTree
{
public:
	void Init(float width, float height, float depth, int32 capacity = 8);
	void Init(Octor rect, int32 capacity = 8);
	void Render(HDC hdc);
	void Clear();

public:
	bool Insert(Ref<class BaseCollider> collider);
	void QuarryRange(Octor& range, vector<Ref<class BaseCollider>>& vec);
	void QuarryRange(Ref<class BaseCollider> collider, vector<Ref<class BaseCollider>>& vec);
	void SetParent(Ref<OctorTree> parent);

private:
	void SubDivid();

private:
	array<Ref<OctorTree>, static_cast<int32>(OCTOR_TREE_TYPE::END)>  _childs;

	Ref<OctorTree>					_parent;

	Octor							_boundary = {};

	vector<Ref<class BaseCollider>>	_vecList;
	int32							_capacity = 8;
	int32							_count = 0;


	bool							_divided = false;
	static int32					TreeCount;
};


bool CollisionBoxToPoint(int32 x, int32 y, int32 width, int32 height, Vec2& pos);