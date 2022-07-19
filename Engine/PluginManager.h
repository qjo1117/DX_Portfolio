#pragma once

class IPlugin;

struct PluginInfo
{
	IPlugin* pPlugin = nullptr;
	HMODULE hDll = nullptr;
};

class PluginManager
{
	DECLARE_SINGLE(PluginManager)
public:
	void Init();
	void Update();
	void Render();
	void End();

	void LoadPlugins();  
	void UnLoadPlugins();

	bool LoadPlugin(const wstring& p_fileName);
	void UnLoadPlugin(const wstring& p_fileName);


	Ref<PluginInfo> FindPlugin(const wstring& name);


public:
	void Log(const string& log);
	
	template<typename T>
	T* GetSingle()
	{
		vector<string> splis = Utils::Split(typeid(T).name(), ' ');
		return reinterpret_cast<T*>((*m_pSingle)[splis[1]]);
	}
	

	map<wstring, Ref<PluginInfo>>& Plugins() { return m_mapPlugins; }

private:
	map<wstring, Ref<PluginInfo>> m_mapPlugins;

	wstring					m_strName = L"Plugin";
	unordered_map<string, void*>* m_pSingle;
};


