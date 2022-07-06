#include "pch.h"
#include "OctorTree.h"
#include "BaseCollider.h"

int32 OctorTree::TreeCount = 0;

void OctorTree::Init(float width, float height, float depth, int32 capacity)
{
	_boundary = { Vec3::Zero, Vec3{width, height, depth} };
	_capacity = capacity;
	_divided = false;
	_vecList.clear();
	_count = 0;
}

void OctorTree::Init(Octor rect, int32 capacity)
{
	_boundary = rect;
	_capacity = capacity;
	_divided = false;
	_vecList.clear();
	_count = 0;
}

void OctorTree::Render(HDC hdc)
{
}

void OctorTree::Clear()
{
	for (auto& obj : _vecList) {
		obj.reset();					// REF_RELEASE
	}
	_vecList.clear();

	for (Ref<OctorTree> collider : _childs) {
		if (collider) {
			collider->Clear();
		}
		collider = nullptr;
	}

	Init(_boundary, _capacity);
}

void OctorTree::Insert(Ref<class BaseCollider> collider)
{
	if (!_boundary.CollisionBoxToPoint(collider->Bound.Center)) {
		return;
	}

	// 값을 참조하기 위해 포인터 형식으로 Push_back진행
	if (_capacity >= _count) {
		_vecList.push_back(collider);
		_count += 1;
		OctorTree::TreeCount += 1;
	}
	else {
		if (_divided == false) {
			SubDivid();
			_divided = true;
		}

		for (Ref<OctorTree>& octor : _childs) {
			octor->Insert(collider);
		}
	}
}

void OctorTree::QuarryRange(Octor& range, vector<Ref<class BaseCollider>>& vec)
{
	if (!PMath::CollisionAABB(_boundary.center, _boundary.extents, range.center, range.extents)) {
		return;
	}
	else {
		for (Ref<class BaseCollider>& obj : _vecList) {
			if (PMath::CollisionAABB(range.center, range.extents, obj->Bound.Center, obj->Bound.Extents)) {
				vec.push_back(obj);
			}

			if (_divided) {
				for (Ref<OctorTree> octor : _childs) {
					octor->QuarryRange(range, vec);
				}
			}
		}
	}
}

void OctorTree::SetParent(Ref<OctorTree> parent)
{
	_parent = parent;
}

void OctorTree::SubDivid()
{
	Octor octer;

	// LEFT_UP_FORWARD		왼쪽 위 앞
	octer = {	Vec3{_boundary.center.x - (_boundary.extents.x / 2), _boundary.center.y + (_boundary.extents.y / 2), _boundary.center.z + (_boundary.extents.z / 2)},
				Vec3{_boundary.extents.x / 2,_boundary.extents.y / 2, _boundary.extents.z / 2} };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_UP_FORWARD)] = make_shared<OctorTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_UP_FORWARD)]->Init(octer, _capacity);

	// RIGHT_UP_FORWARD		오른쪽 위 앞
	octer = {	Vec3{_boundary.center.x + (_boundary.extents.x / 2), _boundary.center.y + (_boundary.extents.y / 2), _boundary.center.z + (_boundary.extents.z / 2)},
				Vec3{_boundary.extents.x / 2,_boundary.extents.y / 2, _boundary.extents.z / 2} };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_UP_FORWARD)] = make_shared<OctorTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_UP_FORWARD)]->Init(octer, _capacity);

	// LEFT_DOWN_FORWARD	왼쪽 아래 앞
	octer = {	Vec3{_boundary.center.x + (_boundary.extents.x / 2), _boundary.center.y - (_boundary.extents.y / 2), _boundary.center.z + (_boundary.extents.z / 2)},
				Vec3{_boundary.extents.x / 2,_boundary.extents.y / 2, _boundary.extents.z / 2} };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_DOWN_FORWARD)] = make_shared<OctorTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_DOWN_FORWARD)]->Init(octer, _capacity);

	// RIGHT_DOWN_FORWARD	오른쪽 아래 앞
	octer = {	Vec3{_boundary.center.x - (_boundary.extents.x / 2), _boundary.center.y - (_boundary.extents.y / 2), _boundary.center.z + (_boundary.extents.z / 2)},
				Vec3{_boundary.extents.x / 2,_boundary.extents.y / 2, _boundary.extents.z / 2} };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_DOWN_FORWARD)] = make_shared<OctorTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_DOWN_FORWARD)]->Init(octer, _capacity);


	// LEFT_UP_BACKWARD		왼쪽 위 뒤
	octer = { Vec3{_boundary.center.x - (_boundary.extents.x / 2), _boundary.center.y + (_boundary.extents.y / 2), _boundary.center.z + (_boundary.extents.z / 2)},
				Vec3{_boundary.extents.x / 2,_boundary.extents.y / 2, _boundary.extents.z / 2} };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_UP_BACKWARD)] = make_shared<OctorTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_UP_BACKWARD)]->Init(octer, _capacity);

	// RIGHT_UP_BACKWARD		오른쪽 위 뒤
	octer = { Vec3{_boundary.center.x + (_boundary.extents.x / 2), _boundary.center.y + (_boundary.extents.y / 2), _boundary.center.z + (_boundary.extents.z / 2)},
				Vec3{_boundary.extents.x / 2,_boundary.extents.y / 2, _boundary.extents.z / 2} };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_UP_BACKWARD)] = make_shared<OctorTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_UP_BACKWARD)]->Init(octer, _capacity);

	// LEFT_DOWN_BACKWARD	왼쪽 아래 뒤
	octer = { Vec3{_boundary.center.x + (_boundary.extents.x / 2), _boundary.center.y - (_boundary.extents.y / 2), _boundary.center.z + (_boundary.extents.z / 2)},
				Vec3{_boundary.extents.x / 2,_boundary.extents.y / 2, _boundary.extents.z / 2} };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_DOWN_BACKWARD)] = make_shared<OctorTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::LEFT_DOWN_BACKWARD)]->Init(octer, _capacity);

	// RIGHT_DOWN_BACKWARD	오른쪽 아래 뒤
	octer = { Vec3{_boundary.center.x - (_boundary.extents.x / 2), _boundary.center.y - (_boundary.extents.y / 2), _boundary.center.z + (_boundary.extents.z / 2)},
				Vec3{_boundary.extents.x / 2,_boundary.extents.y / 2, _boundary.extents.z / 2} };
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_DOWN_BACKWARD)] = make_shared<OctorTree>();
	_childs[EnumToInt32(OCTOR_TREE_TYPE::RIGHT_DOWN_BACKWARD)]->Init(octer, _capacity);
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

    return false;
}