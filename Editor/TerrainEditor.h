#pragma once

#include "IEditor.h"

enum class BRUSH_TYPE
{
	RAY,
	END
};

class TerrainEditor :
    public IEditor
{
public:
	TerrainEditor() {}
	virtual ~TerrainEditor() {}

	MENUITEM("Menu/Terrain", TerrainEditor, Register);
	static void Register()
	{
		if (m_pInstance == nullptr) {
			m_pInstance = EDITOR->GetWindow<TerrainEditor>();
		}
		
	}

	virtual void Init() override;
	virtual void Show() override;
	virtual void End() override;

public:
	TerrainEditor* GetI() { return m_pInstance; }
private:
	inline static TerrainEditor* m_pInstance = nullptr;

private:
	BRUSH_TYPE m_type = BRUSH_TYPE::END;
	float m_speed = 5.0f;
};

