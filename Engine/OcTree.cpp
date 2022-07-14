#include "pch.h"
#include "OcTree.h"
#include "BaseCollider.h"

int32 OcTree::TreeCount = 0;

void OcTree::Init(float width, float height, float depth, int32 capacity)
{
	_bounding = { Vec3::Zero, Vec3{width, height, depth} };
	_capacity = capacity;
	_divided = false;
	m_vecList.clear();
	_count = 0;
}


void OcTree::Init(Vec3 pos, Vec3 extents, int32 capacity)
{
	_bounding.Center = pos;
	_bounding.Extents = extents;

	_capacity = capacity;
	_divided = false;
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

	for (Ref<OcTree> collider : _childs) {
		if (collider) {
			collider->Clear();
		}
		collider = nullptr;
	}

	Init(_bounding.Center, _bounding.Extents);

	OcTree::TreeCount = 0;
}

bool OcTree::Insert(Ref<class BaseCollider> collider)
{
	if (_bounding.Intersects(collider->Bound) == false) {
		return false;
	}

	// ���� �����ϱ� ���� ������ �������� Push_back����
	if (_capacity > _count) {
		m_vecList.push_back(collider);
		_count += 1;
		OcTree::TreeCount += 1;
		return true;
	}
	else {
		if (_divided == false) {
			SubDivid();
			_divided = true;
		}

		for (Ref<OcTree>& octor : _childs) {
			if (octor->Insert(collider) == true) {
				return true;
			}
		}
	}

	return false;
}


void OcTree::QuarryRange(Ref<class BaseCollider> collider, vector<Ref<class BaseCollider>>& vec)
{
	if (CollisionBoxToPoint(_bounding, collider->Bound.Center) == false) {
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
			else if (_divided) {
				for (Ref<OcTree> octor : _childs) {
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

	// LEFT_UP_FORWARD		���� �� ��

	center = Vec3{	_bounding.Center.x - (_bounding.Extents.x / 2),
					_bounding.Center.y + (_bounding.Extents.y / 2),
					_bounding.Center.z + (_bounding.Extents.z / 2) };
	extents = Vec3(_bounding.Extents) / 2.0f;
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_UP_FORWARD)] = make_shared<OcTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_UP_FORWARD)]->Init(center, extents, _capacity);

	// RIGHT_UP_FORWARD		������ �� ��
	center = Vec3{	_bounding.Center.x + (_bounding.Extents.x / 2),
					_bounding.Center.y + (_bounding.Extents.y / 2),
					_bounding.Center.z + (_bounding.Extents.z / 2) };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_UP_FORWARD)] = make_shared<OcTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_UP_FORWARD)]->Init(center, extents, _capacity);

	// LEFT_DOWN_FORWARD	���� �Ʒ� ��
	center = Vec3{	_bounding.Center.x - (_bounding.Extents.x / 2),
					_bounding.Center.y - (_bounding.Extents.y / 2),
					_bounding.Center.z + (_bounding.Extents.z / 2) }; 
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_DOWN_FORWARD)] = make_shared<OcTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_DOWN_FORWARD)]->Init(center, extents, _capacity);

	// RIGHT_DOWN_FORWARD	������ �Ʒ� ��
	center = Vec3{	_bounding.Center.x + (_bounding.Extents.x / 2),
					_bounding.Center.y - (_bounding.Extents.y / 2),
					_bounding.Center.z + (_bounding.Extents.z / 2) };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_DOWN_FORWARD)] = make_shared<OcTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_DOWN_FORWARD)]->Init(center, extents, _capacity);


	// LEFT_UP_BACKWARD		���� �� ��
	center = Vec3{	_bounding.Center.x - (_bounding.Extents.x / 2),
					_bounding.Center.y + (_bounding.Extents.y / 2),
					_bounding.Center.z - (_bounding.Extents.z / 2) };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_UP_BACKWARD)] = make_shared<OcTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_UP_BACKWARD)]->Init(center, extents, _capacity);

	// RIGHT_UP_BACKWARD		������ �� ��
	center = Vec3{	_bounding.Center.x + (_bounding.Extents.x / 2),
					_bounding.Center.y + (_bounding.Extents.y / 2),
					_bounding.Center.z - (_bounding.Extents.z / 2) };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_UP_BACKWARD)] = make_shared<OcTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_UP_BACKWARD)]->Init(center, extents, _capacity);

	// LEFT_DOWN_BACKWARD	���� �Ʒ� ��
	center = Vec3{	_bounding.Center.x - (_bounding.Extents.x / 2),
					_bounding.Center.y - (_bounding.Extents.y / 2),
					_bounding.Center.z - (_bounding.Extents.z / 2) };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_DOWN_BACKWARD)] = make_shared<OcTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_DOWN_BACKWARD)]->Init(center, extents, _capacity);

	// RIGHT_DOWN_BACKWARD	������ �Ʒ� ��
	center = Vec3{	_bounding.Center.x + (_bounding.Extents.x / 2),
					_bounding.Center.y - (_bounding.Extents.y / 2),
					_bounding.Center.z - (_bounding.Extents.z / 2) };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_DOWN_BACKWARD)] = make_shared<OcTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_DOWN_BACKWARD)]->Init(center, extents, _capacity);
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