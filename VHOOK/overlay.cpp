#include "includes.h"
#include "process.h"
#include "overlay.h"
#include "config.h"
#include "WindowBand.h"
#include "menu.h"
#include "ESP.h"
#include "imgui/byte_array.h"
#include "oppressor_logo.h"
#include "PedBone.h"
#include "byte_array2.h"
#include "stb_image.h"  

overlay* overlay::instance = nullptr;

#ifdef _DirectX9
bool LoadTextureFromMemory(LPDIRECT3DDEVICE9 device, const void* file_mapped, size_t size_of_file, LPDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	// Criar textura a partir dos dados da imagem em memória
	HRESULT hr = D3DXCreateTextureFromFileInMemory(device, file_mapped, size_of_file, out_texture);
	if (FAILED(hr))
	{
#ifdef __VERBOSE
		std::cerr << "Falha ao criar a textura." << std::endl;
#endif
		return false;
	}

	// Obter informações da textura criada
	D3DSURFACE_DESC desc;
	(*out_texture)->GetLevelDesc(0, &desc);

	if (out_width)
		*out_width = desc.Width;
	if (out_height)
		*out_height = desc.Height;

	return true;
}

#else

bool LoadTextureFromMemory(ID3D11Device* device, const void* file_mapped, size_t size_of_file, ID3D11ShaderResourceView** out_texture, int* out_width, int* out_height)
{
	int width, height, channels;
	unsigned char* image_data = stbi_load_from_memory((const unsigned char*)file_mapped, (int)size_of_file, &width, &height, &channels, STBI_rgb_alpha);
	if (!image_data)
	{
#ifdef __VERBOSE
		std::cerr << "Falha ao carregar a imagem da memória." << std::endl;
#endif
		return false;
	}

	if (out_width)
		*out_width = width;
	if (out_height)
		*out_height = height;

	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;

	// Pitch da imagem (tamanho da linha)
	UINT rowPitch = width * 4;  // 4 bytes por pixel (RGBA)

	// Dados para preencher a textura
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = image_data;
	initData.SysMemPitch = rowPitch;
	initData.SysMemSlicePitch = rowPitch * height;

	// Criar a textura no Direct3D 11
	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = device->CreateTexture2D(&texDesc, &initData, &texture);
	if (FAILED(hr))
	{
#ifdef __VERBOSE
		std::cerr << "Falha ao criar a textura." << std::endl;
#endif
		stbi_image_free(image_data);  // Liberar memória da imagem
		return false;
	}

	// Criar o Shader Resource View (SRV)
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	hr = device->CreateShaderResourceView(texture, &srvDesc, out_texture);
	texture->Release();  // Liberar a textura, pois o SRV já a mantém
	stbi_image_free(image_data);  // Liberar a memória da imagem carregada

	if (FAILED(hr))
	{
#ifdef __VERBOSE
		std::cerr << "Falha ao criar o Shader Resource View." << std::endl;
#endif
		return false;
	}

	return true;
}
#endif

overlay* overlay::get_instance()
{
	if (!overlay::instance)
		overlay::instance = new overlay();

	return overlay::instance;
}

void overlay::createRenderTarget()
{
#ifndef __NVidiaAMD
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
#else
	p_object->Release();
#endif
}

void overlay::cleanupRenderTarget()
{
#ifndef __NVidiaAMD
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
#else
	if (p_device)
	{
		p_device->Release();
		p_device = NULL;
	}
	if (p_object)
	{
		p_object->Release();
		p_object = NULL;
	}
#endif 
}

void overlay::releaseDirectX()
{
#ifndef __NVidiaAMD
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	this->cleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
#else
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	this->cleanupRenderTarget();
	if (p_object) { p_object->Release(); p_object = nullptr; }
	if (p_device) { p_device->Release(); p_device = nullptr; }
#endif
}

void overlay::updateWindowState()
{
	if (CFG.menu.opened)
	{
		auto dwExStyle = GetWindowLong(this->overlayWindow, GWL_STYLE);
		SetWindowLong(this->overlayWindow, GWL_EXSTYLE, dwExStyle | WS_EX_TOOLWINDOW);
	}
	else
	{
		SetWindowLong(this->overlayWindow, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
		//case WM_SIZE:
		//	if ( g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED )
		//	{
		//		g_d3dpp.BackBufferWidth = LOWORD( lParam );
		//		g_d3dpp.BackBufferHeight = HIWORD( lParam );
		//		ResetDevice( );
		//	}
		//	return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		//::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

bool overlay::create()
{
	WNDCLASSEXA WCEx = {
		sizeof(WNDCLASSEXA),
		0,
		WinProc,
		0,
		0,
		0,
		LoadIcon(0, IDI_APPLICATION),
		LoadCursor(0, IDC_ARROW),
		0,
		0,
		"Chrome_WidgetWin_1",
		LoadIcon(0, IDI_APPLICATION)
	};

	ATOM aRCE = RegisterClassExA(&WCEx);

	RECT defRect;

	GetWindowRect(GetDesktopWindow(), &defRect);

#ifdef WBAND
	this->overlayWindow = WindowBand::Create(0, aRCE, xor (L"Discord"), WS_POPUP, defRect.left,
		defRect.top, defRect.right, defRect.bottom, 0, 0, WCEx.hInstance, 0, WindowBand::ZBID_UIACCESS);
#endif
#ifdef __Medal
	this->overlayWindow = FindWindowA((xor ("MedalOverlayClass")), (xor ("MedalOverlay")));
#endif

#ifndef WBAND
#ifndef __Medal
	this->overlayWindow = CreateWindowExA(NULL, WCEx.lpszClassName, WCEx.lpszClassName, WS_POPUP,
		defRect.left, defRect.top, defRect.right, defRect.bottom, NULL, NULL, 0, NULL);
#endif
#endif
#ifdef __NVidiaAMD
	this->overlayWindow = FindWindowA(xor ("AMDDVROVERLAYWINDOWCLASS"), xor ("amd dvr overlay"));
	if (!this->overlayWindow)
		this->overlayWindow = FindWindowA(xor ("CEF-OSC-WIDGET"), xor ("NVIDIA GeForce Overlay"));
	else
		amd = true;
#endif

	if (!this->overlayWindow)
		return false;

#ifdef __NVidiaAMD
	SetWindowLongA(this->overlayWindow, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	SetWindowPos(this->overlayWindow, HWND_TOPMOST, 0, 0, 0, 0, 0x0002 | 0x0001);
	if (amd) MoveWindow(this->overlayWindow, defRect.left, defRect.top, defRect.right, defRect.bottom, false); // only when amd
#endif

#ifdef __Medal
	SetWindowPos(this->overlayWindow, HWND_TOPMOST, defRect.left, defRect.top, defRect.right, defRect.bottom, SWP_SHOWWINDOW);
#endif

	auto dwExStyle = GetWindowLong(this->overlayWindow, GWL_STYLE);

	SetWindowLong(this->overlayWindow, GWL_EXSTYLE, dwExStyle | WS_EX_TOOLWINDOW);

	SetLayeredWindowAttributes(this->overlayWindow, RGB(0, 0, 0), 255, LWA_ALPHA);

	MARGINS margin = { -1 };

	DwmExtendFrameIntoClientArea(this->overlayWindow, &margin);

	ShowWindow(this->overlayWindow, SW_SHOW);

	UpdateWindow(this->overlayWindow);

	this->updateWindowState();

	return this->directXInit();
}

void overlay::render() {

	if (GetAsyncKeyState(CFG.menu.openKey) & 1) {
		CFG.menu.opened = !CFG.menu.opened;
		ConfigManager::SaveConfigToFile();
		this->updateWindowState();
	}

#ifndef __NVidiaAMD
	ImGui_ImplDX11_NewFrame();
#else
	ImGui_ImplDX9_NewFrame();
#endif
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::GetIO().MouseDrawCursor = CFG.menu.opened;

#ifdef _DEBUG
	if (GetAsyncKeyState(VK_HOME) & 1) {
		CFG.debug.drawGNames = !CFG.debug.drawGNames;
	}
#endif

#ifndef ONLY_MENU
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::Begin("##main", nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowPos(ImVec2(proc->windowLeft, proc->windowTop), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(proc->windowWidth, proc->windowHeight), ImGuiCond_Always);

	{
#ifdef _DEBUG
		ImGui::GetBackgroundDrawList()->AddText({ 1, 1 }, ImColor(255, 255, 255), (std::format("Debug (HOME): {} ", CFG.debug.drawGNames)).c_str());
#endif
		if (!CFG.RageMP.antiscreen) {
			ImGui::GetBackgroundDrawList()->AddText({ (float)((proc->windowWidth / 2) - (proc->windowWidth / 4)), 1.f }, ImColor(255, 0, 0, 255), xor ("OVERLAY PROTECTION DISABLED"));
		}

		if (CFG.RageMP.b_AimbotPlayers && CFG.RageMP.b_AimbotFOV) {
			ImGui::GetBackgroundDrawList()->AddCircle({ (float)(proc->windowWidth / 2), (float)(proc->windowHeight / 2) }, CFG.RageMP.AimbotFOV, ImColor{ 255,255,255 }, 100);
		}


		skeletonMutex.lock();
		ESP::Run();
		skeletonMutex.unlock();

	}
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);

#endif

	if (CFG.menu.opened) {
		menu::get_instance()->draw();
	}

#ifdef __NVidiaAMD
	p_device->BeginScene();
#endif

	// Rendering
	ImGui::Render();

#ifndef __NVidiaAMD
	static const ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	g_pSwapChain->Present(0, 0);
#else
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	p_device->EndScene();
	p_device->Present(NULL, NULL, NULL, NULL);
	p_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
#endif
}

bool overlay::directXInit()
{

#ifndef __NVidiaAMD
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = this->overlayWindow;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK)
		return false;
#else
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_object)))
		exit(3);

	ZeroMemory(&p_params, sizeof(p_params));
	p_params.Windowed = TRUE;
	p_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_params.hDeviceWindow = this->overlayWindow;
	p_params.MultiSampleType = D3DMULTISAMPLE_NONE;
	p_params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_params.BackBufferWidth = 0;
	p_params.BackBufferHeight = 0;
	p_params.EnableAutoDepthStencil = TRUE;
	p_params.AutoDepthStencilFormat = D3DFMT_D16;
	p_params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->overlayWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_params, 0, &p_device)))
	{
		p_object->Release();
		return false;
	}
#endif 

#ifndef __NVidiaAMD
	this->createRenderTarget();
#endif 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

#ifndef __NVidiaAMD
	this->load_layers(this->g_pd3dDevice);
#else
	this->load_layers(this->p_device);
#endif 

	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureMouse = true;
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.IniFilename = NULL;
	io.LogFilename = NULL;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.09f, 0.10f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);

	style.WindowRounding = 3.0f;
	style.FrameRounding = 3.3f;

	// Load Fonts
	ImFontConfig font_config;
	font_config.PixelSnapH = false;
	font_config.OversampleH = 5;
	font_config.OversampleV = 5;
	font_config.RasterizerMultiply = 1.2f;

	ImGui_ImplWin32_Init(this->overlayWindow);

#ifndef __NVidiaAMD
	ImGui_ImplDX11_Init(this->g_pd3dDevice, this->g_pd3dDeviceContext);
#else
	ImGui_ImplDX9_Init(p_device);
#endif 
	return true;
}

void overlay::load_layers(void* p_device)
{
	auto menu = menu::get_instance();

#ifndef __NVidiaAMD
	auto device = reinterpret_cast<ID3D11Device*>(p_device);
#else
	auto device = reinterpret_cast<IDirect3DDevice9Ex*>(p_device);
#endif 

	// Load Fonts
	ImFontConfig font_config;
	font_config.PixelSnapH = false;
	font_config.OversampleH = 5;
	font_config.OversampleV = 5;
	font_config.RasterizerMultiply = 1.2f;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
		0x2DE0, 0x2DFF, // Cyrillic Extended-A
		0xA640, 0xA69F, // Cyrillic Extended-B
		0xE000, 0xE226, // icons
		0,
	};

	font_config.GlyphRanges = ranges;

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	menu->bold = io.Fonts->AddFontFromMemoryTTF(PTRootUIBold, sizeof(PTRootUIBold), 15.0f, &font_config, ranges);
	menu->tab_icons = io.Fonts->AddFontFromMemoryTTF(clarityfont, sizeof(clarityfont), 15.0f, &font_config, ranges);
	menu->logo = io.Fonts->AddFontFromMemoryTTF(clarityfont, sizeof(clarityfont), 21.0f, &font_config, ranges);
	menu->tab_title = io.Fonts->AddFontFromMemoryTTF(PTRootUIBold, sizeof(PTRootUIBold), 19.0f, &font_config, ranges);
	menu->tab_title_icon = io.Fonts->AddFontFromMemoryTTF(clarityfont, sizeof(clarityfont), 18.0f, &font_config, ranges);
	menu->combo_arrow = io.Fonts->AddFontFromMemoryTTF(combo, sizeof(combo), 9.0f, &font_config, ranges);

	this->medium = io.Fonts->AddFontFromMemoryTTF(InterMedium, sizeof(InterMedium), 15.0f, &font_config, ranges);
	this->semibold = io.Fonts->AddFontFromMemoryTTF(InterSemiBold, sizeof(InterSemiBold), 17.0f, &font_config, ranges);
	this->logo = io.Fonts->AddFontFromMemoryTTF(catrine_logo, sizeof(catrine_logo), 17.0f, &font_config, ranges);

	// Our state
	ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

	int oppressor_width = 0;
	int oppressor_height = 0;

	bool ret = LoadTextureFromMemory(device, ChickenLogoArray, sizeof(ChickenLogoArray), &ChickenLogoTexture, &oppressor_width, &oppressor_height);
	if (!ret)
	{
		printf(xor ("[-] failed to create ChickenLogoTexture...\n"));
		ExitProcess(0);
	}

	// Carregar a textura do PedBone
	int pedbone_width = 0, pedbone_height = 0;
	ret = LoadTextureFromMemory(device, PedBoneArray, sizeof(PedBoneArray), &PedBoneTexture, &pedbone_width, &pedbone_height);
	if (!ret)
	{
		printf(xor ("[-] failed to create Ped Image...\n"));
		ExitProcess(0);
	}

	ImGui::SetNextWindowSize({ 540, 370 });
}