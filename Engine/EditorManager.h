#pragma once

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx12.h>
#include "IEditor.h"

struct
{
	string str;
	int32 count = 0;
};

/*---------------
	EngineGUI
----------------*/
class EditorManager
{
public:
	DECLARE_SINGLE(EditorManager)

public:
	/* ----- External Function ----- */
	void Init();
	void Render();
	void End();

	void ResizeWindow();


public:
	/* ----- MenuBar ------ */
	void				MainMenuBar();

	void				RegisterMenuBar(const string& p_title, function<void(void)> p_func);
	Ref<MenuInfo>		Ref_FindMenuBar(Ref<MenuInfo>& p_info, const vector<string>& title, int32 index = 0);
	void				Ref_MenuPresent(Ref<MenuInfo>& p_info);


	/* ----- Log ------ */
	void				ConsoleEditor();
	void				Log(const string& log, uint8 state = LOG_STATE::LOG);
	void				Clear(uint8 state = LOG_STATE::LOG | LOG_STATE::ERROR_ | LOG_STATE::WARNING);

	void				PushEditor(IEditor* editor);

	template<typename T>
	T* GetWindow();

private:
	/* ----- Draw Variable ----- */
	PRIVATE_PROPERTY(ComPtr<ID3D12DescriptorHeap>, srvHeap);

	PRIVATE_PROPERTY(Ref<class Object>, PickObject);
	PRIVATE_PROPERTY(Ref<class Texture>, DefaultImage);

	vector<IEditor*>				m_vecEditor;

	/* ----- Log ------ */
	uint8							m_eState = LOG_STATE::LOG | LOG_STATE::WARNING | LOG_STATE::ERROR_;
	array<unordered_map<string, int32>, 3>		m_arrLogs;

	/* ------ Menu Tree ------ */
	Ref<MenuInfo>					m_pMenuBar = make_shared<MenuInfo>();
};

class Texture;

namespace ImGui
{
	// 이유 너무 쓰기 힘들어서 간단하게 가져오기 위함
	void Image(Ref<Texture> texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
	void ImageButton(Ref<Texture> texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

	static uint32 GImageCount;
}

template<typename T>
T* EditorManager::GetWindow()
{
	if (std::is_same_v<T, IEditor>) {
		ASSERT_CRASH("안됨");
		return nullptr;
	}

	T* window = new T;
	window->Init();

	PushEditor(window);
	return window;
}