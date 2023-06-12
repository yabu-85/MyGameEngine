#pragma once
#include "Direct3D.h"
#include <DirectXMath.h> //XMVECTOR を使うためのやつ
#include "Texture.h"

using namespace DirectX;

//コンスタントバッファー
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
};

//頂点情報
struct VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
};

class Quad
{
protected:
	//必ずメンバ変数を初期化しよう
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	Texture* pTexture_;

public:
	Quad();
	virtual ~Quad();

	virtual HRESULT InitializeIndex();
	HRESULT InitializeConstantBuffer();

	void Draw(XMMATRIX& worldMatrix);
	virtual void DrawIndex(XMMATRIX& worldMatrix);
	void Release();
};