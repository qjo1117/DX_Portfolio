#include "pch.h"
#include "EditorManager.h"
#include "Engine.h"
#include <ImGui/imgui_internal.h>
#include "Texture.h"


void EditorManager::Init()
{
	/* ----- GUI를 생성할 Heap 생성 및, 초기화 작업 ----- */
	D3D12_DESCRIPTOR_HEAP_DESC descSrv = {};
	descSrv.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descSrv.NumDescriptors = RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT + RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT + SRV_REGISTER_COUNT;
	descSrv.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&descSrv, IID_PPV_ARGS(&m_srvHeap));


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplWin32_Init(GEngine->GetWindow().hWnd);
	ImGui_ImplDX12_Init(DEVICE.Get(), descSrv.NumDescriptors,
		DXGI_FORMAT_R8G8B8A8_UNORM, m_srvHeap.Get(),
		m_srvHeap->GetCPUDescriptorHandleForHeapStart(),
		m_srvHeap->GetGPUDescriptorHandleForHeapStart());

	ImGuiContext& g = *GImGui;
	ImGuiViewportP* viewport = g.Viewports[0];
	g.Viewports[0]->Size.x = GEngine->GetWindow().width;
	g.Viewports[0]->Size.y = GEngine->GetWindow().height;

    m_DefaultImage = GET_SINGLE(Resources)->Get<Texture>(L"Default");
}

void EditorManager::Render()
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_srvHeap.Get() };
	GRAPHICS_CMD_LIST->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	ImGui::GImageCount = 1;

	// Start the Dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

    {
        MainMenuBar();
        ConsoleEditor();
    }
    for (IEditor* editor : m_vecEditor) {
        editor->ShowEditor();
    }

	ImGui::EndFrame();

	ImGui::Render();

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GRAPHICS_CMD_LIST.Get());
}

void EditorManager::End()
{
    for (IEditor* editor : m_vecEditor) {
        if (editor != nullptr) {
            editor->End();
            SAFEDELETE(editor);
        }
    }
    m_vecEditor.clear();

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void EditorManager::ResizeWindow()
{
	ImGuiContext& g = *GImGui;
	ImGuiViewportP* viewport = g.Viewports[0];
	g.Viewports[0]->Size.x = GEngine->GetWindow().width;
	g.Viewports[0]->Size.y = GEngine->GetWindow().height;
}

void EditorManager::MainMenuBar()
{
    if (ImGui::BeginMainMenuBar()) {
        Ref_MenuPresent(m_pMenuBar);
        ImGui::EndMainMenuBar();
    }
}


void EditorManager::Ref_MenuPresent(Ref<MenuInfo>& p_info)
{
    for (Ref<MenuInfo> info : p_info->vecChildMenu) {
        if (info->IsMenuBar()) {
            if (ImGui::BeginMenu(info->strTitle.c_str())) {
                Ref_MenuPresent(info);
                ImGui::EndMenu();
            }
        }
        else {
            if (ImGui::MenuItem(info->strTitle.c_str())) {
                // TODO : GetWindow를 구현하자
                info->FuncOn();
            }
        }
    }
}

void EditorManager::RegisterMenuBar(const string& p_title, function<void(void)> p_func)
{
    // 경로를 vector로 받아둔다.
    vector<string> titleToken = Utils::Split(p_title, '/');

    Ref<MenuInfo> ret = Ref_FindMenuBar(m_pMenuBar, titleToken);
    ret->FuncOn = p_func;
}

Ref<MenuInfo> EditorManager::Ref_FindMenuBar(Ref<MenuInfo>& p_info, const vector<string>& title, int32 index)
{
    // 찾은경우
    if (title.size() == index) {
        return p_info;
    }
    else if (p_info == nullptr) {
        // 없는 경우
        Ref<MenuInfo> ret = make_shared<MenuInfo>();
        ret->strTitle = title[index];
        p_info = ret;
        return Ref_FindMenuBar(ret, title, ++index);
    }

    // 순회하는 경우
    for (auto& info : p_info->vecChildMenu) {
        if (title[index] == info->strTitle) {
            return Ref_FindMenuBar(info, title, ++index);
        }
    }

    // 없는 경우
    Ref<MenuInfo> ret = make_shared<MenuInfo>();
    ret->strTitle = title[index];
    p_info->vecChildMenu.push_back(ret);
    return Ref_FindMenuBar(ret, title, ++index);
}



//void EditorManager::AddEditor(const string& title, Ref<IEditor> p_editor)
//{
//    m_vecEditor.push_back(p_editor);
//}

void EditorManager::ConsoleEditor()
{
    ImGui::Begin("Console");

    if (LOG_STATE::LOG & m_eState) {
        for (auto& log : m_arrLogs[0]) {
            string text = "| Log | " + log.first + " / Count : " + std::to_string(log.second);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), text.c_str());
        }
        ImGui::Separator();
    }
    if (LOG_STATE::WARNING & m_eState) {
        for (auto& log : m_arrLogs[1]) {
            string text = "| Warning | " + log.first + " / Count : " + std::to_string(log.second);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), text.c_str());
        }
        ImGui::Separator();
    }
    if (LOG_STATE::ERROR_ & m_eState) {
        for (auto& log : m_arrLogs[2]) {
            string text = "| Error | " + log.first + " / Count : " + std::to_string(log.second);
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), text.c_str());
        }
        ImGui::Separator();
    }

    ImGui::End();
}

void EditorManager::Log(const string& log, uint8 state)
{
    if (LOG_STATE::LOG & state) {
        m_arrLogs[0][log] += 1;
    }
    if (LOG_STATE::WARNING & state) {
        m_arrLogs[1][log] += 1;
    }
    if (LOG_STATE::ERROR_ & state) {
        m_arrLogs[2][log] += 1;
    }
}

void EditorManager::Clear(uint8 state)
{
    if (LOG_STATE::LOG & m_eState) {
        m_arrLogs[0].clear();
    }
    if (LOG_STATE::WARNING & m_eState) {
        m_arrLogs[1].clear();
    }
    if (LOG_STATE::ERROR_ & m_eState) {
        m_arrLogs[2].clear();
    }
}

void EditorManager::PushEditor(IEditor* editor)
{
    m_vecEditor.push_back(editor);
    editor->Init();
}


void ImGui::Image(Ref<Texture> texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
{
	if (texture == nullptr) {
		return;
	}

	uint32 increase = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	uint32 destSize = 1;
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(EDITOR->srvHeap->GetCPUDescriptorHandleForHeapStart(), (increase * GImageCount));

	uint32 srcSize = 1;
	ComPtr<ID3D12DescriptorHeap> srcRtvHeapBegin = texture->GetSRV();
	D3D12_CPU_DESCRIPTOR_HANDLE srcHandle = srcRtvHeapBegin->GetCPUDescriptorHandleForHeapStart();

	DEVICE->CopyDescriptors(1, &destHandle, &destSize, 1, &srcHandle, &srcSize, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_GPU_DESCRIPTOR_HANDLE guiGPU = EDITOR->srvHeap->GetGPUDescriptorHandleForHeapStart();
	ImGui::Image((ImTextureID)(guiGPU.ptr + (increase * GImageCount)), size, uv0, uv1, tint_col, border_col);

	GImageCount += 1;
}


//uint32 increase = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//D3D12_GPU_DESCRIPTOR_HANDLE my_texture_srv_gpu_handle = GET_SINGLE(Resources)->Get<Texture>(L"DiffuseLightTarget")->GetSRV()->GetGPUDescriptorHandleForHeapStart();
////my_texture_srv_gpu_handle.ptr += DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//Ref<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"A", L"..\\Resources\\Texture\\Picture0.png");
////io.Fonts->TexID = (ImTextureID)my_texture_srv_gpu_handle.ptr;
//
//uint32 destSize = 1;
//D3D12_CPU_DESCRIPTOR_HANDLE destHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GEngine->GetGUI()->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart(), increase);
//
//uint32 srcSize = 1;
//ComPtr<ID3D12DescriptorHeap> srcRtvHeapBegin = texture->GetSRV();
//D3D12_CPU_DESCRIPTOR_HANDLE srcHandle = srcRtvHeapBegin->GetCPUDescriptorHandleForHeapStart();
//
//DEVICE->CopyDescriptors(1, &destHandle, &destSize, 1, &srcHandle, &srcSize, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//
//D3D12_GPU_DESCRIPTOR_HANDLE guiGPU = GEngine->GetGUI()->GetSRVHeap().Get()->GetGPUDescriptorHandleForHeapStart();
//ImGui::Text("GPU handle = %p", (ImTextureID)my_texture_srv_gpu_handle.ptr);
//ImGui::Image((ImTextureID)(guiGPU.ptr + increase), ImVec2(100.0f, 100.0f));


void ImGui::ImageButton(Ref<Texture> texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
{
	uint32 increase = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	uint32 destSize = 1;
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(EDITOR->srvHeap->GetCPUDescriptorHandleForHeapStart(), increase * GImageCount);

	uint32 srcSize = 1;
	ComPtr<ID3D12DescriptorHeap> srcRtvHeapBegin = texture->GetSRV();
	D3D12_CPU_DESCRIPTOR_HANDLE srcHandle = srcRtvHeapBegin->GetCPUDescriptorHandleForHeapStart();

	DEVICE->CopyDescriptors(1, &destHandle, &destSize, 1, &srcHandle, &srcSize, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_GPU_DESCRIPTOR_HANDLE guiGPU = EDITOR->srvHeap->GetGPUDescriptorHandleForHeapStart();
	ImGui::ImageButton((ImTextureID)(guiGPU.ptr + (increase * GImageCount)), size, uv0, uv1, frame_padding, bg_col, tint_col);
}

