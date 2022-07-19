#include "pch.h"
#include "ActionPlugin.h"

#include "../Engine/EditorManager.h"
#include "../Engine/PluginManager.h"
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Resources.h"
#include "../Engine/Scene.h"
#include "../Engine/EditorManager.h"
#include "../Engine/SoundManager.h"

#include "../Engine/GameObject.h"
#include "../Engine/MeshRenderer.h"
#include "../Engine/MeshCollider.h"
#include "../Engine/Transform.h"
#include "../Engine/Animator.h"
#include "../Engine/MeshData.h"
#include "../Engine/Light.h"



PLUGINDECL bool ActionPlugin::Awake()
{
	m_pManager->Log("Action Load");
	GET_SINGLE_DLL(EditorManager)->Log("Action Load");

	//m_light = m_pManager->GetResources()->Get<GameObject>(L"PointLight");
	//m_pManager->GetScene()->GetCurrentScene()->AddGameObject(m_light);
	//m_light->GetLight()->lightInfo.color.diffuse = Vec4{ 1.0f, 0.0f,0.0f ,1.0f };
	//m_light->GetLight()->lightInfo.range = 100.0f;

	//reinterpret_cast<SoundManager*>((*GSingle)["SoundManager"]);
	//if (GET_SINGLE_DLL(SoundManager)->Play("HotelArcs.mp3") == true) {
	//	m_pManager->GetEditor()->Log("HotelArcs.mp3");
	//}

	return true;
}

PLUGINDECL bool ActionPlugin::Init()
{
	m_pManager->Log("Action Init");



	return true;
}

PLUGINDECL bool ActionPlugin::Update()
{
	m_pManager->Log("Action Update");

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
