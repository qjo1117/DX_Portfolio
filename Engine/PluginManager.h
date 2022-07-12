#pragma once

class IPlugin;

struct PluginInfo
{
	IPlugin* pPlugin = nullptr;
	HMODULE hDll = nullptr;
};

class PluginManager
{
	DECLARE_SINGLE(PluginManager);
public:
	void Init(class EditorManager& p_Editor, class Engine& p_Engine, class SceneManager& p_Scene);
	void Update();
	void Render();
	void End();

	void LoadPlugins();  
	bool LoadPlugin(const wstring& p_fileName);

	Ref<PluginInfo> FindPlugin(const wstring& name);

public:
	void Log(const string& log);
	class EditorManager* GetEditor();
	class Engine* GetEngine();
	class SceneManager* GetScene();

	map<wstring, Ref<PluginInfo>>& Plugins() { return m_mapPlugins; }

private:
	map<wstring, Ref<PluginInfo>> m_mapPlugins;

	// TODO : È®ÀÎ¿ë
	wstring m_strName = L"Plugin";
	class EditorManager* m_pEditor = nullptr;
	class Engine* m_pEngine = nullptr;
	class SceneManager* m_pScene = nullptr;
};


