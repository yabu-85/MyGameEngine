#pragma once

#include <d3d11.h>

//このリンカやけどどこに書いてもいい/使うとこに書いたほうがわかりやすいけどね
//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace Direct3D
{
	//外部にこれ作ってるよって宣言 extern
	extern ID3D11Device* pDevice; //デバイス

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