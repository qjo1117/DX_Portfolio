#include "pch.h"
#include "TerrainEditor.h"
#include "Terrain.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "Terrain.h"

void TerrainEditor::Init()
{

}

void TerrainEditor::Show()
{
	if (ImGui::Button("Brush")) {
		m_type = BRUSH_TYPE::END;
	}

	if (ImGui::DragFloat("Speed", &m_speed, 0.1f, 0.1f, 100.0f)) {

	}

	if (INPUT->GetButton(KEY_TYPE::LBUTTON) == false) {
		return;
	}

	Ref<Camera> camera = GET_SINGLE(SceneManager)->GetCurrentScene()->GetMainCamera();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	Matrix matProj = camera->matProjection;

	// ViewSpace에서 Picking 진행
	float viewX = (+2.0f * INPUT->GetMousePos().x / width - 1.0f) / matProj(0, 0);
	float viewY = (-2.0f * INPUT->GetMousePos().y / height + 1.0f) / matProj(1, 1);

	Matrix matView = camera->matView;
	Matrix matViewInv = matView.Invert();

	Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

	rayOrigin = XMVector3TransformCoord(rayOrigin, matViewInv);
	rayDir = XMVector3TransformNormal(rayDir, matViewInv);
	rayDir.Normalize();

	float minDistance = FLT_MAX;
	Ref<GameObject> picked;

	RayCastHitInfo hit;
	if (GET_SINGLE(ColliderManager)->RayCast(rayOrigin, rayDir, OUT hit, LAYER_TYPE::GROUND, 10000.0f)) {
		picked = hit.collider->GetGameObject();

		Vec3 origin = hit.point - picked->GetTransform()->localPosition;

		vector<Vertex> vertexs = picked->GetMeshRenderer()->mesh->GetVertices();

		int32 x = origin.x / picked->GetTransform()->localScale.x;
		int32 z = origin.z / picked->GetTransform()->localScale.z;

		vertexs[x + (z * 16)].pos.y += m_speed * DELTATIME;

		picked->GetMeshRenderer()->mesh->Modify(vertexs);
	}


}

void TerrainEditor::End()
{
}
