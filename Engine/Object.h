#pragma once

enum class OBJECT_TYPE : uint8
{
	NONE,
	GAMEOBJECT,		// PREFAB
	COMPONENT,
	MATERIAL,
	MESH,
	SHADER,
	TEXTURE,
	MESH_DATA,

	// TODO

	END
};

enum
{
	OBJECT_TYPE_COUNT = static_cast<uint8>(OBJECT_TYPE::END)
};

/*-----------
	Object
------------*/
class Object
{
public:
	Object(OBJECT_TYPE type);
	virtual ~Object();

	// TODO : Intantiate 복사본 역할

protected:
	friend class Resources;
	virtual void Load(const wstring& path) { }
	virtual void Save(const wstring& path) { }

protected:
	PROTECTED_PROPERTY(wstring, name);
	PROTECTED_PROPERTY(OBJECT_TYPE, objectType) = OBJECT_TYPE::NONE;
	PROTECTED_PROPERTY(uint32, id) = 0;
};

