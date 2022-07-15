#include "pch.h"
#include "OcTree.h"
#include "BaseCollider.h"

int32 OcTree::TreeCount = 0;

void OcTree::Init(float width, float height, float depth, int32 capacity)
{
	m_bounding = { Vec3::Zero, Vec3{width, height, depth} };
	_capacity = capacity;
	m_isDivided = false;
	m_vecList.clear();
	_count = 0;
}


void OcTree::Init(Vec3 pos, Vec3 extents, int32 capacity)
{
	m_bounding.Center = pos;
	m_bounding.Extents = extents;

	_capacity = capacity;
	m_isDivided = false;
	m_vecList.clear();
	_count = 0;
}

void OcTree::Render(HDC hdc)
{
}

void OcTree::Clear()
{
	for (auto& obj : m_vecList) {
		obj.reset();					// REF_RELEASE
	}
	m_vecList.clear();

	for (Ref<OcTree> collider : m_vecChilds) {
		if (collider) {
			collider->Clear();
		}
		collider = nullptr;
	}

	Init(m_bounding.Center, m_bounding.Extents);

	OcTree::TreeCount = 0;
}

bool OcTree::Insert(Ref<class BaseCollider> collider)
{
	if (m_bounding.Intersects(collider->Bound) == false) {
		return false;
	}

	// 값을 참조하기 위해 포인터 형식으로 Push_back진행
	if (_capacity > _count) {
		m_vecList.push_back(collider);
		_count += 1;
		OcTree::TreeCount += 1;
		return true;
	}
	else {
		if (m_isDivided == false) {
			SubDivid();
			m_isDivided = true;
		}

		for (Ref<OcTree>& octor : m_vecChilds) {
			if (octor->Insert(collider) == true) {
				return true;
			}
		}
	}

	return false;
}


void OcTree::QuarryRange(Ref<class BaseCollider> collider, vector<Ref<class BaseCollider>>& vec)
{
	if (m_bounding.Intersects(collider->Bound) == false) {
		return;
	}
	else {
		for (Ref<class BaseCollider>& obj : m_vecList) {
			if (collider == obj) {
				continue;
			}

			if (collider->Bound.Intersects(obj->Bound) == true) {
				vec.push_back(obj);
			}
			if (m_isDivided == true) {
				for (Ref<OcTree> octor : m_vecChilds) {
					octor->QuarryRange(collider, vec);
				}
			}
		}
	}
	
}

void OcTree::SetParent(Ref<OcTree> parent)
{
	m_parent = parent;
}

void OcTree::SubDivid()
{
	BoundingBox octer;
	Vec3 center = Vec3::Zero;
	Vec3 extents = Vec3::Zero;

	// LEFT_UP_FORWARD		왼쪽 위 앞

	center = Vec3{	m_bounding.Center.x - (m_bounding.Extents.x / 2),
					m_bounding.Center.y + (m_bounding.Extents.y / 2),
					m_bounding.Center.z + (m_bounding.Extents.z / 2) };
	extents = Vec3(m_bounding.Extents) / 2.0f;
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::LEFT_UP_FORWARD)] = make_shared<OcTree>();
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::LEFT_UP_FORWARD)]->Init(center, extents, _capacity);

	// RIGHT_UP_FORWARD		오른쪽 위 앞
	center = Vec3{	m_bounding.Center.x + (m_bounding.Extents.x / 2),
					m_bounding.Center.y + (m_bounding.Extents.y / 2),
					m_bounding.Center.z + (m_bounding.Extents.z / 2) };
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_UP_FORWARD)] = make_shared<OcTree>();
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_UP_FORWARD)]->Init(center, extents, _capacity);

	// LEFT_DOWN_FORWARD	왼쪽 아래 앞
	center = Vec3{	m_bounding.Center.x - (m_bounding.Extents.x / 2),
					m_bounding.Center.y - (m_bounding.Extents.y / 2),
					m_bounding.Center.z + (m_bounding.Extents.z / 2) }; 
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::LEFT_DOWN_FORWARD)] = make_shared<OcTree>();
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::LEFT_DOWN_FORWARD)]->Init(center, extents, _capacity);

	// RIGHT_DOWN_FORWARD	오른쪽 아래 앞
	center = Vec3{	m_bounding.Center.x + (m_bounding.Extents.x / 2),
					m_bounding.Center.y - (m_bounding.Extents.y / 2),
					m_bounding.Center.z + (m_bounding.Extents.z / 2) };
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_DOWN_FORWARD)] = make_shared<OcTree>();
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_DOWN_FORWARD)]->Init(center, extents, _capacity);


	// LEFT_UP_BACKWARD		왼쪽 위 뒤
	center = Vec3{	m_bounding.Center.x - (m_bounding.Extents.x / 2),
					m_bounding.Center.y + (m_bounding.Extents.y / 2),
					m_bounding.Center.z - (m_bounding.Extents.z / 2) };
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::LEFT_UP_BACKWARD)] = make_shared<OcTree>();
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::LEFT_UP_BACKWARD)]->Init(center, extents, _capacity);

	// RIGHT_UP_BACKWARD		오른쪽 위 뒤
	center = Vec3{	m_bounding.Center.x + (m_bounding.Extents.x / 2),
					m_bounding.Center.y + (m_bounding.Extents.y / 2),
					m_bounding.Center.z - (m_bounding.Extents.z / 2) };
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_UP_BACKWARD)] = make_shared<OcTree>();
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_UP_BACKWARD)]->Init(center, extents, _capacity);

	// LEFT_DOWN_BACKWARD	왼쪽 아래 뒤
	center = Vec3{	m_bounding.Center.x - (m_bounding.Extents.x / 2),
					m_bounding.Center.y - (m_bounding.Extents.y / 2),
					m_bounding.Center.z - (m_bounding.Extents.z / 2) };
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::LEFT_DOWN_BACKWARD)] = make_shared<OcTree>();
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::LEFT_DOWN_BACKWARD)]->Init(center, extents, _capacity);

	// RIGHT_DOWN_BACKWARD	오른쪽 아래 뒤
	center = Vec3{	m_bounding.Center.x + (m_bounding.Extents.x / 2),
					m_bounding.Center.y - (m_bounding.Extents.y / 2),
					m_bounding.Center.z - (m_bounding.Extents.z / 2) };
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_DOWN_BACKWARD)] = make_shared<OcTree>();
	m_vecChilds[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_DOWN_BACKWARD)]->Init(center, extents, _capacity);
}

int32 EnumToInt32(OCTOR_TREE_TYPE type)
{
	return static_cast<int32>(type);
}

bool CollisionBoxToPoint(int32 x, int32 y, int32 width, int32 height, Vec2& pos)
{
	if (x - width <= pos.x && pos.x <= x + width) {
		if (y - height <= pos.y && pos.y <= y + height) {
			return true;
		}
	}
	return false;
}