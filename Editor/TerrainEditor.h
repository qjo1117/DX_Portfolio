#pragma once
#include "IEditor.h"
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
};

