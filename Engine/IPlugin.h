#ifndef IPLUGIN_H_
#define IPLUGIN_H_

#define PLUGIN_EXPORTS

#ifdef PLUGIN_EXPORTS
#define PLUGINDECL __declspec(dllexport)
#else
#define PLUGINDECL __declspec(dllimport)
#endif


#include <string>
#include "PluginManager.h"
#include "EditorManager.h"
using namespace std;


class PluginManager;

class IPlugin
{
public:
	IPlugin(PluginManager& manager) : m_pManager(&manager) { GImGui = GET_SINGLE_DLL(EditorManager)->imgui; }
	virtual ~IPlugin() { }

	virtual PLUGINDECL bool Awake() { return false; }
	virtual PLUGINDECL bool Init() { return false; }
	virtual PLUGINDECL bool Start() { return false; }
	virtual PLUGINDECL bool Update() { return false; }
	virtual PLUGINDECL bool Render() { return false; }
	virtual PLUGINDECL bool End() { return false; }
	virtual PLUGINDECL const std::wstring& GetName() { return m_Title; }

protected:
	PluginManager* m_pManager = nullptr;

	wstring m_Title;
};

typedef IPlugin* (*CREATEPLUGIN)(PluginManager& manager);

#define CREATE_PLUGIN(Plugin)											\
extern "C" PLUGINDECL IPlugin* CreatePlugin(PluginManager& manager)		\
{																		\
	return new Plugin(manager);											\
}												  


#endif