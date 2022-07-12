#include "pch.h"
#include "ActionPlugin.h"

#include "../Engine/EditorManager.h"
#include "../Engine/PluginManager.h"

PLUGINDECL bool ActionPlugin::Init()
{
	m_pManager->Log("Action");

	

	return true;
}

PLUGINDECL bool ActionPlugin::Update()
{


	return true;
}

PLUGINDECL bool ActionPlugin::Render()
{


	return true;
}

PLUGINDECL bool ActionPlugin::End()
{


	return true;
}
