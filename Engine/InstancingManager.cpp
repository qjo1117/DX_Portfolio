#include "pch.h"
#include "InstancingManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Transform.h"
#include "Material.h"


void InstancingManager::Render(vector<Ref<GameObject>>& gameObjects)
{
	map<uint64,vector<Ref<GameObject>>> cache;

	/* ----- ºÐ·ù ----- */
	for (Ref<GameObject>& go : gameObjects) {
		const uint64 instanceId = go->GetMeshRenderer()->GetInstanceID();
		cache[instanceId].push_back(go);
	}

	/* ----- Rendering ----- */
	for (auto& pair : cache) {
		const vector<Ref<GameObject>>& vec = pair.second;


		if (vec.size() == 1) {
			/* ----- Not Instancing ----- */
			vec[0]->GetMeshRenderer()->Render();
		}
		else {
			/* ----- Instancing ----- */
			const uint64 instanceId = pair.first;

			for (const Ref<GameObject>& go : vec) {
				InstancingParams params;
				params.matWorld = go->GetTransform()->GetLocalToWorldMatrix();
				params.matWV = params.matWorld * Camera::S_MatView;
				params.matWVP = params.matWV * Camera::S_MatProjection;

				AddParam(instanceId, params);
			}
			Ref<InstancingBuffer>& buffer = _buffers[instanceId];
			vec[0]->GetMeshRenderer()->GetMaterial()->SetInt(3, 1);
			vec[0]->GetMeshRenderer()->Render(buffer);
		}
	}

}

void InstancingManager::ClearBuffer()
{
	for (auto& pair : _buffers) {
		Ref<InstancingBuffer>& buffer = pair.second;
		buffer->Clear();
	}
}

void InstancingManager::AddParam(uint64 instanceId, InstancingParams& data)
{
	if (_buffers.find(instanceId) == _buffers.end())
		_buffers[instanceId] = make_shared<InstancingBuffer>();

	_buffers[instanceId]->AddData(data);
}
