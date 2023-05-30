#pragma once
#include "Direct3D.h"
#include <DirectXMath.h> //XMVECTOR を使うためのやつ

using namespace DirectX;

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
	void Initialize();
	void Draw();
	void Release();
};