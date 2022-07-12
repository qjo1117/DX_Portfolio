#include "pch.h"
#include "PluginManager.h"
#include "DirectoryManager.h"
#include "IPlugin.h"
#include "Engine.h"
#include "SceneManager.h"



void PluginManager::Init(EditorManager& p_Editor, Engine& p_Engine, SceneManager& p_Scene)
{
    m_strName = L"Plugin Manager";


    m_pEditor = &p_Editor;
    m_pEngine = &p_Engine;
    m_pScene = &p_Scene;

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

    string rootPath = GET_SINGLE(DirectoryManager)->GetFileInfo()->PathInfo.string();
    for (Ref<FileInfo>& fileInfo : vecFileInfos) {
        wstring fullPath = Utils::Str2Wstr(rootPath + "Output\\" + fileInfo->Name);
        LoadPlugin(fullPath);
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

    IPlugin* pPlugin = pFunc(*GetI());
    if (pPlugin == nullptr) {
        return false;
    }

    if (FindPlugin(pPlugin->GetName()) != nullptr) {
        delete pPlugin;
        pPlugin = nullptr;
        return true;
    }

    Ref<PluginInfo> info = make_shared<PluginInfo>();
    info->pPlugin = pPlugin;
    info->hDll = hDll;

    m_mapPlugins[pPlugin->GetName()] = info;

    return true;
}

Ref<PluginInfo> PluginManager::FindPlugin(const wstring& name)
{
    if (m_mapPlugins[name] == nullptr) {
        return nullptr;
    }
    return m_mapPlugins[name];
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

