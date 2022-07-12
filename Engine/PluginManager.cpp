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
    for (auto& info : m_vecPlugins) {
        info->pPlugin->Init();
    }

}

void PluginManager::Update()
{
    for (auto& info : m_vecPlugins) {
        info->pPlugin->Update();
    }
}

void PluginManager::Render()
{
    for (auto& info : m_vecPlugins) {
        info->pPlugin->Render();
    }
}

void PluginManager::End()
{
    for (auto& info : m_vecPlugins) {
        info->pPlugin->End();

        SAFEDELETE(info->pPlugin);
        ::FreeLibrary(info->hDll);

        info = nullptr;
    }
    m_vecPlugins.clear();

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

    Ref<PluginInfo> info = make_shared<PluginInfo>();
    info->pPlugin = pPlugin;
    info->hDll = hDll;

    m_vecPlugins.push_back(info);

    return true;
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

