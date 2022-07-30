#include "pch.h"
#include "Engine.h"
#include "Material.h"
#include "Transform.h"
#include "Light.h"
#include "PathManager.h"
#include "InstancingManager.h"
#include "PluginManager.h"
#include "DirectoryManager.h"
#include "SoundManager.h"

void Engine::Init(const WindowInfo& info)
{
	// 지속적으로 걱정하지만 콘솔을 끄면 WM_QUIT 이벤트가 발생을 안해서
	// 메모리가 정상적으로 해제가 안되니 꼭꼭꼭 Window를 종료시키자.
#pragma region DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(157424);
	//_CrtSetBreakAlloc(51430);
	// 메모리 Leak이 있을때만 사용하면 됨

#ifdef _DEBUG
	AllocConsole();
#endif 


#pragma endregion

	/* ----- 정보 저장 ----- */
	m_winInfo = info;

	/* ----- 그려질 화면 크기를 설정 ----- */
	m_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	m_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	/* ----- 객체 생성및 초기화 부분 ----- */
#pragma region MakeShaded

	m_device = make_shared<Device>();
	m_graphicsCmdQueue = make_shared<GraphicsCommandQueue>();
	m_computeCmdQueue = make_shared<ComputeCommandQueue>();
	m_swapChain = make_shared<SwapChain>();
	m_rootSignature = make_shared<RootSignature>();
	m_graphicsDescHeap = make_shared<GraphicsDescriptorHeap>();
	m_computeDescHeap = make_shared<ComputeDescriptorHeap>();

#pragma endregion

	m_device->Init();
	m_graphicsCmdQueue->Init(m_device->GetDevice(), m_swapChain);
	m_computeCmdQueue->Init(m_device->GetDevice());
	m_swapChain->Init(info, m_device->GetDevice(), m_device->GetDXGI(), m_graphicsCmdQueue->GetGraphicsCmdQueue());
	m_rootSignature->Init();
	m_graphicsDescHeap->Init(256);
	m_computeDescHeap->Init();

	CreateRenderTargetGroups();
	
	GET_SINGLE(PathManager)->Init();
	GET_SINGLE(Input)->Init(m_winInfo.hWnd);
	GET_SINGLE(Timer)->Init(m_winInfo.hWnd);
	GET_SINGLE(DirectoryManager)->Init();
	GET_SINGLE(SoundManager)->Init();

	GET_SINGLE(Resources)->Init();
	GET_SINGLE(ColliderManager)->Init();


#ifdef EDITOR_MANAGER
	GET_SINGLE(EditorManager)->Init();
#endif

	GET_SINGLE(PluginManager)->Init();

	ResizeWindow(info.width, info.height);

	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(LightParams), 1);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(TransformParams), 256);
	CreateConstantBuffer(CBV_REGISTER::b2, sizeof(MaterialParam), 256);
}

void Engine::Render()
{
	RenderBegin();

	GET_SINGLE(SceneManager)->Render();

#ifdef EDITOR_MANAGER
	GET_SINGLE(EditorManager)->Render();
#endif

	RenderEnd();
}

void Engine::Update()
{
	GET_SINGLE(InstancingManager)->ClearBuffer();

	if (m_isCollisionThread == false) {
		m_isCollisionThread = true;
		m_collisionThread = thread([]() {
			while (GEngine->GetWindow().loop) {
				GEngine->CollisionUpdate();
			}
			GET_SINGLE(ColliderManager)->End();
		});
	}

	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();

	GET_SINGLE(SceneManager)->Update();

	LateUpdate();

	Render();

	GET_SINGLE(PluginManager)->Update();
	GET_SINGLE(DirectoryManager)->Update();
}

void Engine::LateUpdate()
{
	// TODO
}

void Engine::CollisionUpdate()
{
	m_currentFiexdTime += 1;
	if (m_maxFiexdTime > m_currentFiexdTime) {
		return;
	}

	GET_SINGLE(ColliderManager)->Update();
	m_currentFiexdTime = 0;
}

void Engine::End()
{

	m_winInfo.loop = false;

#ifdef EDITOR_MANAGER
	GET_SINGLE(EditorManager)->End();
#endif

	GET_SINGLE(SoundManager)->End();

	GET_SINGLE(SceneManager)->End();

	GET_SINGLE(PathManager)->Clear();
	GET_SINGLE(PluginManager)->End();
	GET_SINGLE(DirectoryManager)->End();

	if (m_collisionThread.joinable() == true) {
		m_collisionThread.join();
	}
}

void Engine::RenderBegin()
{
	m_graphicsCmdQueue->RenderBegin();
}

void Engine::RenderEnd()
{

	m_graphicsCmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	m_winInfo.width = width;
	m_winInfo.height = height;

	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(m_winInfo.hWnd, 0, 100, 100, rect.right - rect.left, rect.bottom - rect.top, 0);

	//_engineGUI->ResizeWindow();
}

void Engine::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	uint8 typeInt = static_cast<uint8>(reg);
	assert(m_constantBuffers.size() == typeInt);

	Ref<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize, count);
	m_constantBuffers.push_back(buffer);
}

void Engine::CreateRenderTargetGroups()
{
	/* ----- DepthStencil ----- */
	Ref<Texture> dsTexture = GET_SINGLE(Resources)->CreateTexture(L"DepthStencil",
		DXGI_FORMAT_D32_FLOAT, m_winInfo.width, m_winInfo.height,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	/* ----- SwapChain Group ----- */
	{
		vector<RenderTarget> rtVec(SWAP_CHAIN_BUFFER_COUNT);
		for (uint32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
			wstring name = L"SwapChainTarget_" + std::to_wstring(i);

			ComPtr<ID3D12Resource> resources;
			m_swapChain->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&resources));
			rtVec[i].target = GET_SINGLE(Resources)->CreateTextureFromResource(name, resources);
		}

		m_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)] = make_shared<RenderTargetGroup>();
		m_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)]->Create(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN, rtVec, dsTexture);
	}

	/* ----- Shadow Group ----- */
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_SHADOW_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"ShadowTarget",
			DXGI_FORMAT_R32_FLOAT, 4096, 4096,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		Ref<Texture> shadowDepthTexture = GET_SINGLE(Resources)->CreateTexture(L"ShadowDepthStencil",
			DXGI_FORMAT_D32_FLOAT, 4096, 4096,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		m_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SHADOW)] = make_shared<RenderTargetGroup>();
		m_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SHADOW)]->Create(RENDER_TARGET_GROUP_TYPE::SHADOW, rtVec, shadowDepthTexture);
	}

	/* ----- PickTargetView Group ----- */
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_PICK_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"PickTargetView",
			DXGI_FORMAT_R32G32B32A32_FLOAT, m_winInfo.width, m_winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		Ref<Texture> pickDepthTexture = GET_SINGLE(Resources)->CreateTexture(L"PickDepthStencil",
			DXGI_FORMAT_D32_FLOAT, m_winInfo.width, m_winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		m_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::PICK)] = make_shared<RenderTargetGroup>();
		m_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::PICK)]->Create(RENDER_TARGET_GROUP_TYPE::PICK, rtVec, pickDepthTexture);
	}

	/* ----- Deferred Group ----- */
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"PositionTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, m_winInfo.width, m_winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture(L"NormalTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, m_winInfo.width, m_winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		rtVec[2].target = GET_SINGLE(Resources)->CreateTexture(L"DiffuseTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, m_winInfo.width, m_winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		m_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)] = make_shared<RenderTargetGroup>();
		m_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)]->Create(RENDER_TARGET_GROUP_TYPE::G_BUFFER, rtVec, dsTexture);
	}

	/* ----- Light Group ----- */
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"DiffuseLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, m_winInfo.width, m_winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture(L"SpecularLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, m_winInfo.width, m_winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		m_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)] = make_shared<RenderTargetGroup>();
		m_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)]->Create(RENDER_TARGET_GROUP_TYPE::LIGHTING, rtVec, dsTexture);
	}
}
