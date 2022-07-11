#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "RenderTargetGroup.h"

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"

#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Resources.h"
#include "ColliderManager.h"


#define EDITOR_MANAGER

#ifdef EDITOR_MANAGER
#include "EditorManager.h"
#endif

/*-----------
	Engine 
------------*/
class Engine
{
public:
	/* ----- External Function ----- */
	void Init(const WindowInfo& info);
	void Render();
	void Update();
	void LateUpdate();
	void CollisionUpdate();
	void End();

	void ResizeWindow(int32 width, int32 height);

public:
	/* ----- Helper Function ----- */
	Ref<Device>					GetDevice() { return _device; }
	Ref<GraphicsCommandQueue>	GetGraphicsCmdQueue() { return _graphicsCmdQueue; }
	Ref<ComputeCommandQueue>	GetComputeCmdQueue() { return _computeCmdQueue; }
	Ref<SwapChain>				GetSwapChain() { return _swapChain; }
	Ref<RootSignature>			GetRootSignature() { return _rootSignature; }
	Ref<GraphicsDescriptorHeap>	GetGraphicsDescHeap() { return _graphicsDescHeap; }
	Ref<ComputeDescriptorHeap>	GetComputeDescHeap() { return _computeDescHeap; }
	Ref<ConstantBuffer>			GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }
	Ref<RenderTargetGroup>		GetRTGroup(RENDER_TARGET_GROUP_TYPE type) { return _rtGroups[static_cast<uint8>(type)]; }

	const WindowInfo& GetWindow() { return _winInfo; }

private:
	/* ----- Render Function ----- */
	void RenderBegin();
	void RenderEnd();

private:
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);
	void CreateRenderTargetGroups();

private:


private:
	/* ----- View Variable ----- */
	// 그려질 화면 크기 관련
	WindowInfo		_winInfo;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};

	/* ----- Graphics Variable ----- */
	Ref<Device>													_device;
	Ref<GraphicsCommandQueue>									_graphicsCmdQueue;
	Ref<ComputeCommandQueue>									_computeCmdQueue;
	Ref<SwapChain>												_swapChain;
	Ref<RootSignature>											_rootSignature;
	Ref<GraphicsDescriptorHeap>									_graphicsDescHeap;
	Ref<ComputeDescriptorHeap>									_computeDescHeap;

	array<Ref<RenderTargetGroup>, RENDER_TARGET_GROUP_COUNT>	_rtGroups;
	vector<Ref<ConstantBuffer>>									_constantBuffers;

	PRIVATE_PROPERTY(bool, isFrame) = false;

	thread		_collisionThread;
	bool		_isCollisionThread = false;
	thread		_renderThread;
	int32		_maxFiexdTime = 12;
	int32		_currentFiexdTime = 0;
};

