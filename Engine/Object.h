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

	OBJECT_TYPE GetType() { return _objectType; }

	void SetName(const string& name) { _guiName = name; _name = Utils::Str2Wstr(name); }
	void SetName(const wstring& name) { _name = name; _guiName = Utils::Wstr2Str(name); }

	const wstring& GetName() { return _name; }
	const string& GetGUIName() { return _guiName; }

	uint32 GetID() { return _id; }

	// TODO : Intantiate 복사본 역할

protected:
	friend class Resources;
	virtual void Load(const wstring& path) { }
	virtual void Save(const wstring& path) { }

protected:
	OBJECT_TYPE		_objectType = OBJECT_TYPE::NONE;
	wstring			_name;
	string			_guiName;

protected:
	uint32			_id = 0;
};

