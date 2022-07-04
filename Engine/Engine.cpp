#include "pch.h"
#include "Engine.h"
#include "Material.h"
#include "Transform.h"
#include "Light.h"
#include "Resources.h"
#include "PathManager.h"
#include "InstancingManager.h"

void Engine::Init(const WindowInfo& info)
{
	// ���������� ���������� �ܼ��� ���� WM_QUIT �̺�Ʈ�� �߻��� ���ؼ�
	// �޸𸮰� ���������� ������ �ȵǴ� ������ Window�� �����Ű��.
#pragma region DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(992);
	// �޸� Leak�� �������� ����ϸ� ��

#ifdef _DEBUG
	AllocConsole();
#endif 


#pragma endregion

	/* ----- ���� ���� ----- */
	_winInfo = info;

	/* ----- �׷��� ȭ�� ũ�⸦ ���� ----- */
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	/* ----- ��ü ������ �ʱ�ȭ �κ� ----- */
#pragma region MakeShaded

	_device = make_shared<Device>();
	_graphicsCmdQueue = make_shared<GraphicsCommandQueue>();
	_computeCmdQueue = make_shared<ComputeCommandQueue>();
	_swapChain = make_shared<SwapChain>();
	_rootSignature = make_shared<RootSignature>();
	_graphicsDescHeap = make_shared<GraphicsDescriptorHeap>();
	_computeDescHeap = make_shared<ComputeDescriptorHeap>();

#pragma endregion

	_device->Init();
	_graphicsCmdQueue->Init(_device->GetDevice(), _swapChain);
	_computeCmdQueue->Init(_device->GetDevice());
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _graphicsCmdQueue->GetGraphicsCmdQueue());
	_rootSignature->Init();
	_graphicsDescHeap->Init(256);
	_computeDescHeap->Init();
#ifdef EDITOR_MANAGER
	GET_SINGLE(EditorManager)->Init();
#endif


	CreateRenderTargetGroups();
	
	GET_SINGLE(PathManager)->Init();
	GET_SINGLE(Input)->Init(_winInfo.hWnd);
	GET_SINGLE(Timer)->Init(_winInfo.hWnd);
	GET_SINGLE(Resources)->Init();

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

	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();
	GET_SINGLE(SceneManager)->Update();

	LateUpdate();

	Render();
}

void Engine::LateUpdate()
{
	// TODO
}

void Engine::End()
{
#ifdef EDITOR_MANAGER
	GET_SINGLE(EditorManager)->End();
#endif
	GET_SINGLE(PathManager)->Clear();
}

void Engine::RenderBegin()
{
	_graphicsCmdQueue->RenderBegin();
}

void Engine::RenderEnd()
{

	_graphicsCmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	_winInfo.width = width;
	_winInfo.height = height;

	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_winInfo.hWnd, 0, 100, 100, rect.right - rect.left, rect.bottom - rect.top, 0);

	//_engineGUI->ResizeWindow();
}

void Engine::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	uint8 typeInt = static_cast<uint8>(reg);
	assert(_constantBuffers.size() == typeInt);

	Ref<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize, count);
	_constantBuffers.push_back(buffer);
}

void Engine::CreateRenderTargetGroups()
{
	/* ----- DepthStencil ----- */
	Ref<Texture> dsTexture = GET_SINGLE(Resources)->CreateTexture(L"DepthStencil",
		DXGI_FORMAT_D32_FLOAT, _winInfo.width, _winInfo.height,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	/* ----- SwapChain Group ----- */
	{
		vector<RenderTarget> rtVec(SWAP_CHAIN_BUFFER_COUNT);
		for (uint32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
			wstring name = L"SwapChainTarget_" + std::to_wstring(i);

			ComPtr<ID3D12Resource> resources;
			_swapChain->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&resources));
			rtVec[i].target = GET_SINGLE(Resources)->CreateTextureFromResource(name, resources);
		}

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)]->Create(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN, rtVec, dsTexture);
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

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SHADOW)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SHADOW)]->Create(RENDER_TARGET_GROUP_TYPE::SHADOW, rtVec, shadowDepthTexture);
	}

	/* ----- PickTargetView Group ----- */
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_PICK_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"PickTargetView",
			DXGI_FORMAT_R32G32B32A32_FLOAT, _winInfo.width, _winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		Ref<Texture> pickDepthTexture = GET_SINGLE(Resources)->CreateTexture(L"PickDepthStencil",
			DXGI_FORMAT_D32_FLOAT, _winInfo.width, _winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::PICK)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::PICK)]->Create(RENDER_TARGET_GROUP_TYPE::PICK, rtVec, pickDepthTexture);
	}

	/* ----- Deferred Group ----- */
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"PositionTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, _winInfo.width, _winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture(L"NormalTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, _winInfo.width, _winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		rtVec[2].target = GET_SINGLE(Resources)->CreateTexture(L"DiffuseTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _winInfo.width, _winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)]->Create(RENDER_TARGET_GROUP_TYPE::G_BUFFER, rtVec, dsTexture);
	}

	/* ----- Light Group ----- */
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"DiffuseLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _winInfo.width, _winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture(L"SpecularLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _winInfo.width, _winInfo.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)]->Create(RENDER_TARGET_GROUP_TYPE::LIGHTING, rtVec, dsTexture);
	}
}