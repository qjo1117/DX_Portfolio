#pragma once

#include "InstancingBuffer.h"

class GameObject;

class InstancingManager
{
private:
	DECLARE_SINGLE(InstancingManager);

public:
	void Render(vector<Ref<GameObject>>& gameObjects);

	void ClearBuffer();
	void Clear() { _buffers.clear(); }

private:
	void AddParam(uint64 instanceId, InstancingParams& data);

private:
	map<uint64/*id*/, Ref<InstancingBuffer>> _buffers;
};

