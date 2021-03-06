#pragma once

struct OcBounding
{
	Vec3 center = Vec3::Zero;
	Vec3 extents = Vec3::One;
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


class OcTree
{
public:
	void Init(float width, float height, float depth, int32 capacity = 8);
	void Init(Vec3 pos, Vec3 extents, int32 capacity = 8);
	void Render(HDC hdc);
	void Clear();

public:
	bool Insert(Ref<class BaseCollider> collider);
	void QuarryRange(Ref<class BaseCollider> collider, vector<Ref<class BaseCollider>>& vec);
	void SetParent(Ref<OcTree> parent);

	const array<Ref<OcTree>, static_cast<int32>(OCTOR_TREE_TYPE::END)>& GetChilds() { return m_vecChilds; }

private:
	void SubDivid();
	bool CollisionBoxToPoint(BoundingBox box, Vec3 pos) {
		if (box.Center.x - box.Extents.x <= pos.x && pos.x <= box.Center.x + box.Extents.x) {
			if (box.Center.y - box.Extents.y <= pos.y && pos.y <= box.Center.y + box.Extents.y) {
				if (box.Center.z - box.Extents.z <= pos.z && pos.z <= box.Center.z + box.Extents.z) {
					return true;
				}
			}
		}
		return false;
	}

private:
	array<Ref<OcTree>, static_cast<int32>(OCTOR_TREE_TYPE::END)>  m_vecChilds;

	PRIVATE_PROPERTY(Ref<OcTree>, parent);
	PRIVATE_PROPERTY(vector<Ref<class BaseCollider>>, vecList);


	BoundingBox						m_bounding;
	int32							_capacity = 8;
	int32							_count = 0;


	bool							m_isDivided = false;
	static int32					TreeCount;
};


bool CollisionBoxToPoint(int32 x, int32 y, int32 width, int32 height, Vec2& pos);