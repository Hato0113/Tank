//--------------------------
//	DirectX.h
//	DirectX11管理マネージャ
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "main.h"
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <tchar.h>

//-- define --
#define CLASS_NAME _T("DirectXAppClass")
#define CAPTION_NAME ("DirectXGame")

//-- 列挙体 --
enum EBlendState {
	BS_NONE = 0,							// 半透明合成無し
	BS_ALPHABLEND,							// 半透明合成
	BS_ADDITIVE,							// 加算合成
	BS_SUBTRACTION,							// 減算合成

	MAX_BLENDSTATE
};

//-- 名前空間 --
namespace DX
{
	class DirectXManager;

	/*
		テクスチャ読み込みラッパー関数
		引数	ファイル名 const char*
				テクスチャ保存ポインタ ID3D11ShaderResoruceView*
		戻り値	読み込み成功か否か
	*/
	bool LoadTexture(const char* filename, ID3D11ShaderResourceView** pTex);

	/*
		エラーメッセージ出力関数
		引数	メッセージ const wchar_t*
	*/
	void MakeMessage(const wchar_t*);

	/*
	
	*/
	inline float wrapPI(float theta)
	{
		theta += DirectX::XM_PI;
		theta -= floorf(theta * (1.0f / DirectX::XM_PI)) * DirectX::XM_PI * 2.0f;
		theta -= DirectX::XM_PI;
		return theta;
	}
}

//-- クラス定義 --
class DX::DirectXManager
{
private:
	DirectXManager();
	~DirectXManager() = default;
public:
	DirectXManager(const DirectXManager&) = delete;
	DirectXManager& operator=(const DirectXManager&) = delete;
	DirectXManager(DirectXManager&&) = delete;
	DirectXManager& operator=(DirectXManager&&) = delete;

	static DirectXManager& GetInstance()
	{
		static DirectXManager Inst;
		return Inst;
	}

	//-- メンバ関数 --
	const HRESULT MakeWindow(HINSTANCE hInst, int nCmdShow);
	const HRESULT Init();		//DirectX初期化
	void Update();
	void Draw();
	void Uninit();

	//-- Getter --
	HWND GetWndHandle() { return m_hWnd; }
	HINSTANCE GetInstanceHandle() { return m_hInstance; }
	ID3D11Device* GetDevice() { return m_pDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext; }

	//-- Setter --
	void SetBlendState(EBlendState nBlendState);
	void SetZBuffer(bool state);	//2Dポリゴン描画はfalse
	void SetZWrite(bool state);		//透過する時にfalse
	void SetFPS(float fps);

	//-- DirectX情報 --
private:
	ID3D11Device* m_pDevice;						//デバイス
	ID3D11DeviceContext* m_pDeviceContext;			//デバイスコンテキスト
	IDXGISwapChain* m_pSwapChain;					//スワップチェイン
	ID3D11RenderTargetView* m_pRenderTargetView;	//レンダーターゲット
	ID3D11RasterizerState* m_pRasterize[3];			//ラスタライザ
	ID3D11Texture2D* m_pDepthStencilTexture;		// Zバッファ用メモリ
	ID3D11DepthStencilView* m_pDepthStencilView;	// Zバッファ
	UINT				m_uSyncInterval;			// 垂直同期 (0:無, 1:有)
	ID3D11BlendState* m_pBlendState[4];				//ブレンドステート
	ID3D11DepthStencilState* m_pDSS[2];				//Z/ステンシルステート

	//-- ウィンドウ情報 --
private:
	HINSTANCE m_hInstance;							//インスタンスハンドル
	HWND m_hWnd;									//メインウィンドウハンドル
	wchar_t m_CaptionName[30];						//キャプション名

public:
	int m_CountFPS;		//FPSカウント
};

