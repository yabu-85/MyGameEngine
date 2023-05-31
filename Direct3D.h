#pragma once

#include <d3d11.h>

//このリンカやけどどこに書いてもいい/使うとこに書いたほうがわかりやすいけどね
//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//defineは数値の代入ではなくて　文字の置き換え 一行で書かんといけん
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Direct3D
{
	//外部にこれ作ってるよって宣言 extern えくすたーｎ
	extern ID3D11Device* pDevice; //デバイス
	extern ID3D11DeviceContext* pContext; //デバイスコンテキスト

	//初期化
	void Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
	void InitShader();

	//描画開始
	void BeginDraw();

	//描画終了
	void EndDraw();

	//解放
	void Release();

};