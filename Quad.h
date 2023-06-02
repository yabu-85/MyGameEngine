#pragma once
#include "Direct3D.h"
#include <DirectXMath.h> //XMVECTOR を使うためのやつ

using namespace DirectX;

//defineは数値の代入ではなくて　文字の置き換え 一行で書かんといけん
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

//コンスタントバッファー
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
};

class Quad
{
	//必ずメンバ変数を初期化しよう
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

public:
	Quad();
	~Quad();
	HRESULT Initialize();
	void Draw();
	void Release();
};