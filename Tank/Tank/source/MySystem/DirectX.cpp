//-----------------------------------
//	DirectX11管理クラス
//	DirectX.cpp
//-- author --
//	HatoriMasashi
//-- Log --
//	2021/06/14	作成開始
//-----------------------------------

//-- include --
#include "DirectX.h"
#include <stdio.h>
#pragma comment(lib, "d3d11")
#pragma comment(lib,"winmm")
using namespace DX;
#include "System\Texture.h"
#include "Application.hpp"

/*
	コンストラクタ
*/
DirectXManager::DirectXManager()
	: m_pDevice(nullptr), m_pDeviceContext(nullptr), m_pSwapChain(nullptr),
	m_pRenderTargetView(nullptr), m_pRasterize(), m_pDepthStencilTexture(nullptr),
	m_pDepthStencilView(nullptr), m_uSyncInterval(0), m_pBlendState(), m_pDSS(),
	m_hInstance(0), m_hWnd(0), m_CountFPS(0)
{
	lstrcpyW(m_CaptionName, L"Node");
}

/*
	ウィンドウ作成関数
*/
const HRESULT DirectXManager::MakeWindow(HINSTANCE hInst, int nCmdShow)
{
	//-- ウィンドウクラス登録 --
	WNDCLASSEX wcEx = {
		sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW,
		WindowProc,
		0, 0, hInst,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(WHITE_BRUSH),
		NULL,  CLASS_NAME, NULL
	};
	if (!RegisterClassEx(&wcEx)) return -1;

	//-- インスタンス所持 --
	m_hInstance = hInst;

	//-- COM初期化 --
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		MessageBox(NULL, _T("COMの初期化に失敗"), _T("error"), MB_OK);
		return -1;
	}

	//-- ウィンドウサイズ算出 --
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION
		| WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX;
	DWORD dwExStyle = 0;
	RECT rc = { 0, 0, WindowInfo::m_ScreenWidth + WindowInfo::m_GUIWindowWidth
		, WindowInfo::m_ScreenHeight + WindowInfo::m_GUIWindowHeight };
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);

	//-- ウィンドウ作成 --
	HWND hWnd = CreateWindowEx(dwExStyle,
		CLASS_NAME, (LPCWSTR)m_CaptionName,
		dwStyle,
		15, 15,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr, nullptr, hInst, nullptr);
	if (!hWnd) return -1;
	m_hWnd = hWnd;

	//-- ウィンドウ表示 --
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);


	return S_OK;
}

/*
	ブレンドステート設定
*/
void DirectXManager::SetBlendState(EBlendState nBlendState)
{
	if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_pDeviceContext->OMSetBlendState(m_pBlendState[nBlendState], blendFactor, 0xffffffff);
	}
}

/*
	Zバッファー使用設定
*/
void DirectXManager::SetZBuffer(bool state)
{
	m_pDeviceContext->OMSetDepthStencilState((state) ? nullptr : m_pDSS[1], 0);
}

/*
	Zバッファー書き込み設定
*/
void DirectXManager::SetZWrite(bool state)
{
	m_pDeviceContext->OMSetDepthStencilState((state) ? nullptr : m_pDSS[0], 0);
}

/*
	FPS書き込み設定
*/
void DirectXManager::SetFPS(float fps)
{
	char fpsText[30];
	sprintf_s(fpsText, "Tank / Fps:%.2f", fps);
	SetWindowTextA(m_hWnd, (LPCSTR)fpsText);
}

/*
	DirectX初期化
*/
const HRESULT DirectXManager::Init()
{
	HRESULT hr = S_OK;

	//-- デバイス/スワップチェーン作成 --
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = WindowInfo::m_ScreenWidth + WindowInfo::m_GUIWindowWidth;
	scd.BufferDesc.Height = WindowInfo::m_ScreenHeight + WindowInfo::m_GUIWindowHeight;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = m_hWnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = true;
	//scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &scd,
		&m_pSwapChain, &m_pDevice, nullptr, &m_pDeviceContext);
	if (FAILED(hr)) {
		return hr;
	}

	//-- バックバッファ作成 --
	//-- レンダーターゲットビュー生成 --
	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	pBackBuffer->Release();
	pBackBuffer = nullptr;

	// Zバッファ用テクスチャ生成
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = WindowInfo::m_ScreenWidth + WindowInfo::m_GUIWindowWidth;
	td.Height = WindowInfo::m_ScreenHeight + WindowInfo::m_GUIWindowHeight;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = m_pDevice->CreateTexture2D(&td, nullptr, &m_pDepthStencilTexture);
	if (FAILED(hr)) {
		return hr;
	}

	// Zバッファターゲットビュー生成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture,
		&dsvd, &m_pDepthStencilView);
	if (FAILED(hr)) {
		return hr;
	}

	// 各ターゲットビューをレンダーターゲットに設定
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


	//-- ビューポート設定 --
	D3D11_VIEWPORT vp;
	vp.Width = (float)WindowInfo::m_ScreenWidth;
	vp.Height = (float)WindowInfo::m_ScreenHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);

	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;	// カリング無し(両面描画)
	m_pDevice->CreateRasterizerState(&rd, &m_pRasterize[0]);
	rd.CullMode = D3D11_CULL_FRONT;	// 前面カリング(裏面描画)
	m_pDevice->CreateRasterizerState(&rd, &m_pRasterize[1]);
	rd.CullMode = D3D11_CULL_BACK;	// 背面カリング(表面描画)
	m_pDevice->CreateRasterizerState(&rd, &m_pRasterize[2]);
	m_pDeviceContext->RSSetState(m_pRasterize[2]);

	// ブレンド ステート生成
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[0]);
	// ブレンド ステート生成 (アルファ ブレンド用)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[1]);
	// ブレンド ステート生成 (加算合成用)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[2]);
	// ブレンド ステート生成 (減算合成用)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState[3]);
	SetBlendState(BS_ALPHABLEND);

	// 深度ステンシルステート生成
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;		//Zバッファ書き込みOFF
	m_pDevice->CreateDepthStencilState(&dsd, &m_pDSS[0]);
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;		//深度テストを行わない
	m_pDevice->CreateDepthStencilState(&dsd2, &m_pDSS[1]);

	//-- アプリケーション初期化 --
	tank::Application::Init();

	return hr;
}

/*
	DirectX更新
*/
void DirectXManager::Update()
{
	//-- ウィンドウがトップに無ければ動かさない --
	if (GetActiveWindow() != m_hWnd) return;
	//-- アプリケーション更新 --
	tank::Application::Update();
}
/*
	DirectX描画
*/
void DirectXManager::Draw()
{
	//-- バックバッファクリア --
	float ClearColor[4] = { 0.2f, 0.2f, 0.5f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//-- アプリケーション描画 --
	tank::Application::Draw();

	//-- バッファ入れ替え --
	m_pSwapChain->Present(m_uSyncInterval, 0);
}

/*
	DirectX終了処理
*/
void DirectXManager::Uninit()
{
	//-- アプリケーション終了 --
	tank::Application::Uninit();

	//-- オブジェクト破棄 --
	for (int i = 0; i < _countof(m_pDSS); i++)
	{
		if (m_pDSS[i])
			m_pDSS[i]->Release();
		m_pDSS[i] = nullptr;
	}
	for (int i = 0; i < _countof(m_pBlendState); i++)
	{
		if (m_pBlendState[i])
			m_pBlendState[i]->Release();
		m_pBlendState[i] = nullptr;
	}
	if (m_pDepthStencilTexture)
	{
		m_pDepthStencilTexture->Release();
		m_pDepthStencilTexture = nullptr;
	}
	if (m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
		m_pDepthStencilView = nullptr;
	}
	if (m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = nullptr;
	}
	for (int i = 0; i < _countof(m_pRasterize); i++)
	{
		if (m_pRasterize[i])
			m_pRasterize[i]->Release();
		m_pRasterize[i] = nullptr;
	}
	if (m_pSwapChain)
	{
		m_pSwapChain->Release();
		m_pSwapChain = nullptr;
	}
	if (m_pDeviceContext)
	{
		m_pDeviceContext->Release();
		m_pDeviceContext = nullptr;
	}
	if (m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}


	//-- タイマ設定を元に戻す --
	timeEndPeriod(1);

	//-- ウィンドウクラスの登録を解除 --
	UnregisterClass(CLASS_NAME, m_hInstance);
}

/*
	テクスチャ読み込みラッパー関数
	引数	ファイル名 const wchar_t*
			テクスチャ保存ポインタ ID3D11ShaderResoruceView*
	戻り値	読み込み成功か否か
*/
bool DX::LoadTexture(const char* filename, ID3D11ShaderResourceView** pTex)
{
	HRESULT hr;
	hr = CreateTextureFromFile(DX::DirectXManager::GetInstance().GetDevice(), filename, pTex);
	if (FAILED(hr))
	{
		wchar_t message[256];
		wcscat(message, L"Texture could not be loaded");
		//wcscat(message, filename);
		DX::MakeMessage(message);
		return false;
	}
	return true;
}


/*
	エラーメッセージ出力関数
	引数	メッセージ const wchar_t*
*/
void DX::MakeMessage(const wchar_t* message = _T("ERROR"))
{
	MessageBox(NULL, message, _T("error"), MB_OK);
}
