#include "Dice.h"
#include "Camera.h"

Dice::Dice()
{
	pVertexBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pConstantBuffer_ = nullptr;
	pTexture_ = nullptr;
}

Dice::~Dice()
{
	Quad::Release();
}



HRESULT Dice::InitializeVertex()
{

	// 頂点情報
	VERTEX vertices[] =
	{
		//1面（前
		{ XMVectorSet(-1.0f,  1.0f, -1.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) , XMVectorSet(-1.0f, 1.0f, -9.0f, 0.0f) },  //左上 0
		{ XMVectorSet(1.0f,  1.0f, -1.0f, 0.0f), XMVectorSet(0.25f, 0.0f, 0.0f, 0.0f) , XMVectorSet(1.0f, 1.0f, -9.0f, 0.0f) },  //右上 1
		{ XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f), XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f) , XMVectorSet(1.0f, -1.0f, -9.0f, 0.0f) },  //右下 2
		{ XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f), XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f) , XMVectorSet(-1.0f, -1.0f, -9.0f, 0.0f) },  //左下 3

		//2面（右
		{ XMVectorSet(1.0f,  1.0f, -1.0f, 0.0f), XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f) , XMVectorSet(9.0f, 1.0f, -1.0f, 0.0f) },   //左上 4
		{ XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f) ,  XMVectorSet(9.0f, 1.0f, 1.0f, 0.0f) },    //右上 5
		{ XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f), XMVectorSet(0.25f, 1.0f, 0.0f, 0.0f) , XMVectorSet(9.0f, -1.0f, 1.0f, 0.0f) },   //右下 6
		{ XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) , XMVectorSet(9.0f, -1.0f, -1.0f, 0.0f) },   //左下 7

		//3面（後
		{ XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f) ,  XMVectorSet(1.0f, 1.0f, 9.0f, 0.0f) },   //左上 8
		{ XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f) ,  XMVectorSet(-1.0f, 1.0f, 9.0f, 0.0f) },   //右上 9
		{ XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f), XMVectorSet(0.5f, 1.0f, 0.0f, 0.0f) , XMVectorSet(-1.0f, -1.0f, 9.0f, 0.0f) },   //右下 10
		{ XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f), XMVectorSet(0.25f, 1.0f, 0.0f, 0.0f) , XMVectorSet(1.0f, -1.0f, 9.0f, 0.0f) },   //左下 11

		//4面（左
		{ XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(0.25f, 0.0f, 0.0f, 0.0f) ,  XMVectorSet(-9.0f, 1.0f, 1.0f, 0.0f) },  //左上 12
		{ XMVectorSet(-1.0f,  1.0f, -1.0f, 0.0f), XMVectorSet(0.5f, 0.0f, 0.0f, 0.0f) , XMVectorSet(-9.0f, 1.0f, -1.0f, 0.0f) },  //右上 13
		{ XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f), XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f) , XMVectorSet(-9.0f, -1.0f, -1.0f, 0.0f) },  //右下 14
		{ XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f), XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f) , XMVectorSet(-9.0f, -1.0f, 1.0f, 0.0f) },  //左下 15

		//5面（上）
		{ XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(0.75f, 0.0f, 0.0f, 0.0f) ,   XMVectorSet(-1.0f, 9.0f, 1.0f, 0.0f) }, //左上 16
		{ XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) ,     XMVectorSet(1.0f, 9.0f, 1.0f, 0.0f) }, //右上 17
		{ XMVectorSet(1.0f,  1.0f, -1.0f, 0.0f), XMVectorSet(1.0f, 0.5f, 0.0f, 0.0f) ,   XMVectorSet(1.0f, 9.0f, -1.0f, 0.0f) }, //右下 18
		{ XMVectorSet(-1.0f,  1.0f, -1.0f, 0.0f), XMVectorSet(0.75f, 0.5f, 0.0f, 0.0f) , XMVectorSet(-1.0f, 9.0f, -1.0f, 0.0f) },  //左下 19

		//6面（下）
		{ XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f), XMVectorSet(0.5f, 0.0f, 0.0f, 0.0f) , XMVectorSet(-1.0f, -9.0f, -1.0f, 0.0f) }, //左上 20
		{ XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f), XMVectorSet(0.75f, 0.0f, 0.0f, 0.0f) , XMVectorSet(1.0f, -9.0f, -1.0f, 0.0f) }, //右上 21
		{ XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f), XMVectorSet(0.75f, 0.5f, 0.0f, 0.0f) ,  XMVectorSet(1.0f, -9.0f, 1.0f, 0.0f) }, //右下 22
		{ XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f), XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f) ,  XMVectorSet(-1.0f, -9.0f, 1.0f, 0.0f) }, //左下 23

	};

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	///ヘッダーしか見てないから見つけられない 対処 Direct3Dヘッダーで書いてるよ
	if (FAILED(hr)) {

		//エラー処理
		MessageBox(nullptr, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
}

HRESULT Dice::InitializeIndex()
{
	//インデックス情報 前・右・奥・左・上・下
	int index[] = { 0,1,2, 0,2,3, 4,5,6, 4,6,7, 8,9,10, 8,10,11, 12,13,14, 12,14,15, 16,17,18, 16,18,19, 20,21,22, 20,22,23 }; ///時計回りなら230,012 でもおｋ

	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "インデックスバッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
}

void Dice::DrawIndex(XMMATRIX& worldMatrix)
{
	Direct3D::pContext_->DrawIndexed(36, 0, 0); //Dice initialize のインデックス情報でいれた数

}
