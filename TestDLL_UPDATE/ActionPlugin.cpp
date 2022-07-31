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

#include "../Engine/GameObject.h"
#include "../Library/Include/ImGui/imgui.h"


#include "TestMonobehaviour.h"

PLUGINDECL bool ActionPlugin::Awake()
{



	return true;
}

PLUGINDECL bool ActionPlugin::Init()
{
	m_pManager->Log("Action Init");



	return true;
}

PLUGINDECL bool ActionPlugin::Start()
{
	GET_SINGLE_DLL(EditorManager)->Log("Action Load");

	{
		Ref<GameObject> test = make_shared<GameObject>();
		test->AddComponent(make_shared<Transform>());
		test->AddComponent(make_shared<TestMonobehaviour>());
		{
			Ref<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			meshRenderer->mesh = GET_SINGLE_DLL(Resources)->LoadCubeMesh();
			meshRenderer->SetMaterial(GET_SINGLE_DLL(Resources)->Get<Material>(L"Defualt"));
			test->AddComponent(meshRenderer);
		}
		test->GetTransform()->localPosition = Vec3::Backward * 300.0f;
		test->GetTransform()->localScale = Vec3::One * 300.0f;
		GET_SINGLE_DLL(SceneManager)->GetCurrentScene()->AddGameObject(test);
	}

	return true;
}

PLUGINDECL bool ActionPlugin::Update()
{
	GET_SINGLE_DLL(EditorManager)->Log("Action Update");

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
