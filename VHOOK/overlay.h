#pragma once

#define RETRY_COUNT 10
#define SLEEP_DURATION_MS 1000

class overlay
{
private:
	static overlay* instance;

	void load_layers(void* p_device);

public:
	overlay()
	{
		this->proc = process::get_instance();
	}
	

	bool directXInit();
	void createRenderTarget();
	void cleanupRenderTarget();
	void releaseDirectX();

	void updateWindowState();

	// create window to overlay
	bool create();

	void render();

	static overlay* get_instance();


#ifndef __NVidiaAMD
	ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	UINT					g_ResizeWidth = 0, g_ResizeHeight = 0;
	ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

	ID3D11ShaderResourceView* ChickenLogoTexture = nullptr;
	ID3D11ShaderResourceView* PedBoneTexture = nullptr;
	ImFont* medium;
	ImFont* semibold;
	ImFont* logo;

#else
	IDirect3D9Ex* p_object = NULL;
	IDirect3DDevice9Ex* p_device = NULL;
	D3DPRESENT_PARAMETERS p_params = {};
	LPDIRECT3DTEXTURE9 ChickenLogoTexture = nullptr;
	LPDIRECT3DTEXTURE9 PedBoneTexture = nullptr;
	bool amd;
	ImFont* medium;
	ImFont* semibold;
	ImFont* logo;
#endif

	HWND overlayWindow = nullptr;
	MSG msg{};

	process* proc;
};