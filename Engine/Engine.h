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
	Ref<Device>					GetDevice() { return m_device; }
	Ref<GraphicsCommandQueue>	GetGraphicsCmdQueue() { return m_graphicsCmdQueue; }
	Ref<ComputeCommandQueue>	GetComputeCmdQueue() { return m_computeCmdQueue; }
	Ref<SwapChain>				GetSwapChain() { return m_swapChain; }
	Ref<RootSignature>			GetRootSignature() { return m_rootSignature; }
	Ref<GraphicsDescriptorHeap>	GetGraphicsDescHeap() { return m_graphicsDescHeap; }
	Ref<ComputeDescriptorHeap>	GetComputeDescHeap() { return m_computeDescHeap; }
	Ref<ConstantBuffer>			GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return m_constantBuffers[static_cast<uint8>(type)]; }
	Ref<RenderTargetGroup>		GetRTGroup(RENDER_TARGET_GROUP_TYPE type) { return m_rtGroups[static_cast<uint8>(type)]; }

	const WindowInfo& GetWindow() { return m_winInfo; }

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
	WindowInfo		m_winInfo;
	D3D12_VIEWPORT	m_viewport = {};
	D3D12_RECT		m_scissorRect = {};

	/* ----- Graphics Variable ----- */
	Ref<Device>													m_device;
	Ref<GraphicsCommandQueue>									m_graphicsCmdQueue;
	Ref<ComputeCommandQueue>									m_computeCmdQueue;
	Ref<SwapChain>												m_swapChain;
	Ref<RootSignature>											m_rootSignature;
	Ref<GraphicsDescriptorHeap>									m_graphicsDescHeap;
	Ref<ComputeDescriptorHeap>									m_computeDescHeap;

	array<Ref<RenderTargetGroup>, RENDER_TARGET_GROUP_COUNT>	m_rtGroups;
	vector<Ref<ConstantBuffer>>									m_constantBuffers;

	PRIVATE_PROPERTY(bool, isFrame) = false;

	thread		m_collisionThread;
	bool		m_isCollisionThread = false;
	thread		m_renderThread;
	int32		m_maxFiexdTime = 12;
	int32		m_currentFiexdTime = 0;
};

