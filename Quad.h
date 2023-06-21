#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Camera.h"
#include "Texture.h"
#include "Direct3D.h"

using namespace DirectX;

//コンスタントバッファー
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
	XMMATRIX	matNormal; //matWからNormal用に改名
};

//頂点情報
struct VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
	XMVECTOR normal;
};

class Quad
{
protected:
	std::vector<int> index_; //インデックス情報
	int indexNum_;
	int vertexNum_;
	std::vector<VERTEX> vertices_;

	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	Texture* pTexture_;

public:
	Quad();
	virtual ~Quad();

	void Release();
	HRESULT Initialize();
	void Draw(XMMATRIX& worldMatrix);
	
private:
	//  Initializeから呼ばれる関数-----------------
	virtual void InitVertexData(); //頂点情報の準備
	HRESULT CreateVertexBuffer();  //頂点バッファを作成

	virtual void InitIndexData();  //インデックス情報を準備
	HRESULT CreateIndexBuffer();  //インデックスバッファを作成

	HRESULT CreateConstantBuffer(); //コンスタントバッファを作成

	HRESULT LoadTexture(); //テクスチャをロード


	// Draw関数から呼ばれる関数--------------------
	void PassDataToCB(DirectX::XMMATRIX& worldMatrix);  //コンス
	void SetBufferToPipeline();                         //各バッファ
};