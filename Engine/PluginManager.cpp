#include "pch.h"
#include "PluginManager.h"
#include "DirectoryManager.h"
#include "IPlugin.h"
#include "Engine.h"
#include "SceneManager.h"
#include "PathManager.h"


void PluginManager::Init(EditorManager& p_Editor, Engine& p_Engine, SceneManager& p_Scene, Input& p_input, Resources& p_resource)
{
    m_strName = L"Plugin Manager";


    m_pEditor = &p_Editor;
    m_pEngine = &p_Engine;
    m_pScene = &p_Scene;
    m_pInput = &p_input;
    m_pResource = &p_resource;

    LoadPlugins();
    for (auto& item : m_mapPlugins) {
        item.second->pPlugin->Init();
    }

}

void PluginManager::Update()
{
    for (auto& item : m_mapPlugins) {
        item.second->pPlugin->Update();
    }
}

void PluginManager::Render()
{
    for (auto& item : m_mapPlugins) {
        item.second->pPlugin->Render();
    }
}

void PluginManager::End()
{
    for (auto& item : m_mapPlugins) {
        item.second->pPlugin->End();

        SAFEDELETE(item.second->pPlugin);
        ::FreeLibrary(item.second->hDll);

        item.second = nullptr;
    }
    m_mapPlugins.clear();

}

void PluginManager::LoadPlugins()
{
	vector<Ref<FileInfo>> vecFileInfos;
	GET_SINGLE(DirectoryManager)->FindFileInfo(GET_SINGLE(DirectoryManager)->GetFileInfo(), vecFileInfos, ".dll");

    for (Ref<FileInfo>& fileInfo : vecFileInfos) {
        LoadPlugin(fileInfo->PathInfo.wstring());
    }
}

void PluginManager::UnLoadPlugins()
{
    int32 size = m_mapPlugins.size();
    int32 count = 0;
    for (auto& item : m_mapPlugins) {
        UnLoadPlugin(item.first);
        count += 1;
        if (size >= count) {
            break;
        }
    }
}

bool PluginManager::LoadPlugin(const wstring& p_fileName)
{
    HMODULE hDll = ::LoadLibraryW(p_fileName.c_str());
    if (hDll == NULL) {
        LPVOID lpMsgBuf;
        ::FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR)&lpMsgBuf,
            0,
            NULL
        );

        ::OutputDebugStringA((char*)lpMsgBuf);

        LocalFree(lpMsgBuf);
        return false;
    }

    CREATEPLUGIN pFunc = (CREATEPLUGIN)::GetProcAddress(hDll, "CreatePlugin");
    if (pFunc == nullptr) {
        return false;
    }

    // 함수를 호출하고 실패했을 경우 종료
    IPlugin* pPlugin = pFunc(*GetI());
    if (pPlugin == nullptr) {
        return false;
    }

    // 현재 Plugin이 존재할 경우 종료
    if (FindPlugin(pPlugin->GetName()) != nullptr) {
        delete pPlugin;
        pPlugin = nullptr;
        return true;
    }

    // 만약 없을 경우 추가
    Ref<PluginInfo> info = make_shared<PluginInfo>();
    info->pPlugin = pPlugin;
    info->hDll = hDll;

    m_mapPlugins[pPlugin->GetName()] = info;

    m_mapPlugins[pPlugin->GetName()]->pPlugin->Awake();

    return true;
}

Ref<PluginInfo> PluginManager::FindPlugin(const wstring& name)
{
    if (m_mapPlugins[name] == nullptr) {
        return nullptr;
    }
    return m_mapPlugins[name];
}

void PluginManager::UnLoadPlugin(const wstring& p_fileName)
{
    if (m_mapPlugins[p_fileName]->pPlugin) {
        delete m_mapPlugins[p_fileName]->pPlugin;
    }
    m_mapPlugins[p_fileName]->pPlugin = nullptr;

    FreeModule(m_mapPlugins[p_fileName]->hDll);

    m_mapPlugins.erase(p_fileName);

}

void PluginManager::Log(const string& log)
{
    m_pEditor->Log(log);
}

EditorManager* PluginManager::GetEditor()
{
    return m_pEditor;
}

Engine* PluginManager::GetEngine()
{
    return m_pEngine;
}

SceneManager* PluginManager::GetScene()
{
    return m_pScene;
}

Input* PluginManager::GetInput()
{
    return m_pInput;
}

Resources* PluginManager::GetResources()
{
    return m_pResource;
}

