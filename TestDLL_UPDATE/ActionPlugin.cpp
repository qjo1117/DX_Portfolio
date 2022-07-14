#include "pch.h"
#include "ActionPlugin.h"

#include "../Engine/EditorManager.h"
#include "../Engine/PluginManager.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Scene.h"
#include "../Engine/Timer.h"

#include "../Engine/GameObject.h"
#include "../Engine/Transform.h"
#include "../Engine/MeshRenderer.h"
#include "../Engine/Light.h"

PLUGINDECL bool ActionPlugin::Init()
{
	m_pManager->Log("Action Init");


	return true;
}

PLUGINDECL bool ActionPlugin::Update()
{
	m_pManager->Log("Action Update");
	if (m_player == nullptr) {
		m_player = m_pManager->GetScene()->GetCurrentScene()->FindGameObject(L"Player");
	}
	else {
		m_player->GetTransform()->localPosition.x += 1.0f;
	}

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
