#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

//このリンカやけどどこに書いてもいい/使うとこに書いたほうがわかりやすいけどね
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//defineは数値の代入ではなくて　文字の置き換え 一行で書かんといけん
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

//これ enumクラスにして見えないようにするのが正解
enum SHADER_TYPE {
	SHADER_2D,
	SHADER_3D,
	SHADER_POINT,
	SHADER_MAX
};

namespace Direct3D
{
	//外部にこれ作ってるよって宣言 extern えくすたーｎ
	extern ID3D11Device* pDevice_; //デバイス
	extern ID3D11DeviceContext* pContext_; //デバイスコンテキスト
	extern int scrWidth, scrHeight;

	//初期化
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
	HRESULT InitShader();
	HRESULT InitShader3D();
	HRESULT InitShader2D();
	HRESULT InitShaderPoint();

	void SetShader(SHADER_TYPE type);

	//描画開始
	void BeginDraw();

	//描画終了
	void EndDraw();

	//解放
	void Release();

};