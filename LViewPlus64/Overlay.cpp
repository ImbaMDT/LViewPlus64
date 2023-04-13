#include "Overlay.h"
#include "Utils.h"
#include "Structs.h"
#include "LeagueMemoryReader.h"
#include "MemSnapshot.h"
#include "PyGame.h"


#include <string>
#include <list>

#define HCheck(x, m) if(x != S_OK) { throw std::runtime_error(Character::Format("DirectX: Failed at %s. Error code: %d\n", m, MAKE_HRESULT(1, _FACDXGI, X))); }

bool isWOpen = true;
bool p_open = true;

ID3D11Device* Overlay::dxDevice = NULL;
ID3D11DeviceContext* Overlay::dxDeviceContext = NULL;
IDXGISwapChain1* Overlay::dxSwapChain = NULL;
ID3D11RenderTargetView* Overlay::dxRenderTarget = NULL;

Overlay::Overlay() : configs(*(ConfigSet::Get())) {
}

void Overlay::Init() {

	// Create transparent window
	std::string windowClassName = Character::RandomString(10);
	std::string windowName = Character::RandomString(10);
	SetConsoleTitleA(windowName.c_str());

	// Create window with random name & class name
	WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, windowClassName.c_str(), NULL };
	RegisterClassExA(&wc);
	hWindow = CreateWindowExA(
		WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_LAYERED,
		windowClassName.c_str(), windowName.c_str(),
		WS_POPUP,
		1, 1, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		nullptr, nullptr, GetModuleHandle(0), nullptr);

	if (hWindow == NULL) {
		throw WinApiException("Failed to create overlay window");
	}

	ShowWindow(hWindow, SW_SHOW);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hWindow))
	{
		CleanupDeviceD3D();
		throw std::runtime_error("Failed to create D3D device");
	}

	// Setup imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	D3DDEVICE_CREATION_PARAMETERS d3dcp{ 0 };
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.DeltaTime = 1.0f / 60.0f;
	// io.ConfigWindowsMoveFromTitleBarOnly = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsLight();
	ImGuiStyle& style = ImGui::GetStyle();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWindow);
	ImGui_ImplDX11_Init(dxDevice, dxDeviceContext);

	ImGui::GetStyle().FrameRounding = 4.0f;
	ImGui::GetStyle().GrabRounding = 4.0f;
	style.Alpha = 1.0f;                                // Global alpha applies to everything in ImGui
	style.WindowPadding = ImVec2(10, 10);                      // Padding within a window
	style.WindowMinSize = ImVec2(100, 100);                    // Minimum window size
	style.WindowRounding = 0.0f;                                // Radius of window corners rounding. Set to 0.0f to have rectangular windows
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);                  // Alignment for title bar text
	style.ChildRounding = 0.0f;                                // Radius of child window corners rounding. Set to 0.0f to have rectangular child windows
	style.FramePadding = ImVec2(5, 5);                        // Padding within a framed rectangle (used by most widgets)
	style.FrameRounding = 0.0f;                                // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
	style.ItemSpacing = ImVec2(5, 5);                        // Horizontal and vertical spacing between widgets/lines
	style.ItemInnerSpacing = ImVec2(4, 4);                        // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
	style.TouchExtraPadding = ImVec2(0, 0);                        // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
	style.IndentSpacing = 21.0f;                               // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
	style.ColumnsMinSpacing = 6.0f;                                // Minimum horizontal spacing between two columns
	style.ScrollbarSize = 16.0f;                               // Width of the vertical scrollbar, Height of the horizontal scrollbar
	style.ScrollbarRounding = 9.0f;                                // Radius of grab corners rounding for scrollbar
	style.GrabMinSize = 10.0f;                               // Minimum width/height of a grab box for slider/scrollbar
	style.GrabRounding = 0.0f;                                // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);                
	style.DisplayWindowPadding = ImVec2(22, 22);                     
	style.DisplaySafeAreaPadding = ImVec2(4, 4);                       
	style.AntiAliasedLines = true;                               
	style.AntiAliasedFill = true;                               
	style.CurveTessellationTol = 1.25f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.5f);			// 1.00f
	colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	//colors[ImGuiCol_TitleText] = ImVec4(0.80f, 0.80f, 1.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.60f, 0.90f, 0.50f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 0.8f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
	colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
	colors[ImGuiCol_Button] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void Overlay::GameStart(MemSnapshot& memSnapshot)
{
	scriptManager.LoadAll(configs.GetStr("scriptsFolder", "."), memSnapshot.player->name);
}

void Overlay::StartFrame()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Overlay::Update(MemSnapshot& memSnapshot) {

	// Simple check to see if game ended
//	if (memSnapshot.champions.size() == 0 || !isWindowVisible)
//		return;

	PyGame state = PyGame::ConstructFromMemSnapshot(memSnapshot);

	DrawOverlayWindows(state);
	ExecScripts(state);
	if (isWOpen)
		DrawUI(state, memSnapshot);
}

void Overlay::RenderFrame()
{
	static ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);

	ImGui::EndFrame();
	ImGui::Render();
	dxDeviceContext->OMSetRenderTargets(1, &dxRenderTarget, NULL);
	dxDeviceContext->ClearRenderTargetView(dxRenderTarget, (float*)&clear_color);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	dxSwapChain->Present(1, 0);
}

void Overlay::ExecScripts(PyGame& state)
{
	for (auto& script : scriptManager.activeScripts) {
		if (script->enabled && script->loadError.empty() && script->execError.empty())
			script->ExecUpdate(state, imguiInterface);
	}
}

void Overlay::DrawUI(PyGame& state, MemSnapshot& memSnapshot) {

	ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 800, 0 }, ImGuiCond_Once);
	if (!ImGui::Begin("WS+ BY BCKD00R", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::End();
		return;
	}

	DrawScriptSettings(state, memSnapshot);

	ImGui::End();
}

void Overlay::DrawOverlayWindows(PyGame& state)
{
	// Draw game overlay (used for primitive rendering)
	auto io = ImGui::GetIO();
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::SetNextWindowPos(ImVec2{ 800, 0 });
	ImGui::Begin("##Overlay", &isWOpen,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoCollapse |
		ImGuiTabBarFlags_Reorderable | 
		ImGuiTabBarFlags_FittingPolicyScroll | 
		ImGuiTabBarFlags_NoTooltip
	);
	state.overlay = ImGui::GetWindowDrawList();
	ImGui::End();
}

void Overlay::DrawScriptSettings(PyGame& state, MemSnapshot& memSnapshot)
{
	if (ImGui::Button("Save all addons")) {
		scriptManager.SaveAllScriptsConfigs();
		configs.SaveToFile();
	}

	ImGui::SameLine();
	if (ImGui::Button("Reload all addons")) {
		GameStart(memSnapshot);
	}
/*
	if (ImGui::CollapsingHeader("Zoom settings")) {
		int amount = 10;
		ImGui::SliderInt("Max zoom", &amount, 1000, 10000);
	}
*/

	int idNode = 10000;
	for (std::shared_ptr<Script>& script : scriptManager.activeScripts) {
		idNode++;

		// If we got any load/execution script error we should print it in bright red
		if (!script->loadError.empty() || !script->execError.empty()) {
			DrawScriptError(script);
		}
		else {

			bool skippedExecution = false;
			if (!script->enabled) {
				ImGui::PushStyleColor(ImGuiCol_Header, Colors::GRAY);
				skippedExecution = true;
			}
			if (ImGui::CollapsingHeader(script->name.c_str())) {
				ImGui::Button(script->name.c_str(), ImVec2(300, 25));
				DrawScriptCommonSettings(script, idNode);
				script->ExecDrawSettings(state, imguiInterface);
				/*if (ImGui::Begin("Manager")) {
					ImGui::Button(script->name.c_str(), ImVec2(300, 25));
					DrawScriptCommonSettings(script, idNode);
					script->ExecDrawSettings(state, imguiInterface);
				}
				ImGui::End();*/
			}

			if (skippedExecution)
				ImGui::PopStyleColor();
		}

	}
}

void Overlay::DrawScriptError(std::shared_ptr<Script>& script)
{
	ImGui::PushStyleColor(ImGuiCol_Header, Colors::RED);
	if (ImGui::CollapsingHeader(script->name.c_str())) {
		if (ImGui::Button("Reload addon"))
			scriptManager.ReloadScript(script);

		ImGui::TextColored(Colors::RED, script->loadError.c_str());
		ImGui::TextColored(Colors::RED, script->execError.c_str());
	}
	ImGui::PopStyleColor();
}

void Overlay::DrawScriptCommonSettings(std::shared_ptr<Script>& script, int id)
{
	
	if (ImGui::Button("Reload addon"))
		scriptManager.ReloadScript(script);
	ImGui::SameLine();
	if (ImGui::Button("Save addon")) {
		scriptManager.SaveScriptConfigs(script);
		configs.SaveToFile();
	}
	ImGui::Checkbox("Enabled", &script->enabled);
	ImGui::Separator();
}

bool Overlay::IsVisible()
{
	return isWindowVisible;
}

void Overlay::Hide()
{
	ShowWindow(hWindow, SW_HIDE);
	isWindowVisible = false;
}

void Overlay::Show()
{
	ShowWindow(hWindow, SW_SHOW);
	isWindowVisible = true;
}

void Overlay::ToggleTransparent()
{
	isWOpen = !isWOpen;
	LONG ex_style = GetWindowLong(hWindow, GWL_EXSTYLE);
	ex_style = (ex_style & WS_EX_TRANSPARENT) ? (ex_style & ~WS_EX_TRANSPARENT) : (ex_style | WS_EX_TRANSPARENT);
	SetWindowLong(hWindow, GWL_EXSTYLE, ex_style);
}

ID3D11Device* Overlay::GetDxDevice()
{
	return dxDevice;
}

bool Overlay::CreateDeviceD3D(HWND hWnd)
{
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HCheck(D3D11CreateDevice(
		nullptr,    // Adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,    // Module
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		nullptr, 0, // Highest available feature level
		D3D11_SDK_VERSION,
		&dxDevice,
		nullptr,    // Actual feature level
		&dxDeviceContext), "Creating device");

	IDXGIDevice* dxgiDevice;
	HCheck(dxDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice), "Query DXGI Device");

	IDXGIAdapter* dxgiAdapter = 0;
	HCheck(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter), "Get DXGI Adapter");

	IDXGIFactory2* dxgiFactory = 0;
	HCheck(dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory), "Get DXGI Factory");

	// Create swap chain with alpha mode premultiplied
	DXGI_SWAP_CHAIN_DESC1 description = {};
	description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	description.BufferCount = 2;
	description.SampleDesc.Count = 1;
	description.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
	description.Scaling = DXGI_SCALING_STRETCH;
	RECT rect = {};
	GetClientRect(hWnd, &rect);
	description.Width = rect.right - rect.left;
	description.Height = rect.bottom - rect.top;

	HCheck(dxgiFactory->CreateSwapChainForComposition(dxgiDevice,
		&description,
		nullptr,
		&dxSwapChain), "Create swap chain");

	// Create Direct Composition layer
	IDCompositionDevice* dcompDevice;
	DCompositionCreateDevice(
		dxgiDevice,
		__uuidof(dcompDevice),
		(void**)&dcompDevice);

	IDCompositionTarget* target;
	dcompDevice->CreateTargetForHwnd(hWnd,
		true,
		&target);

	IDCompositionVisual* visual;
	dcompDevice->CreateVisual(&visual);
	visual->SetContent(dxSwapChain);
	target->SetRoot(visual);
	dcompDevice->Commit();

	CreateRenderTarget();
	return true;
}

void Overlay::CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (dxSwapChain) { dxSwapChain->Release(); dxSwapChain = NULL; }
	if (dxDeviceContext) { dxDeviceContext->Release(); dxDeviceContext = NULL; }
	if (dxDevice) { dxDevice->Release(); dxDevice = NULL; }
}

void Overlay::CreateRenderTarget()
{
	ID3D11Resource* pBackBuffer;
	if (S_OK != dxSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))
		throw std::runtime_error("Failed to retrieve DX11 swap chain buffer");
	if (S_OK != dxDevice->CreateRenderTargetView(pBackBuffer, NULL, &dxRenderTarget))
		throw std::runtime_error("Failed to create DX11 render target");
	pBackBuffer->Release();
}

void Overlay::CleanupRenderTarget()
{
	if (dxRenderTarget) { dxRenderTarget->Release(); dxRenderTarget = NULL; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI Overlay::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (dxDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			dxSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}