#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include <DirectXMath.h> //XMVECTOR を使うためのやつ
#include <vector>
#include "Transform.h"

using namespace DirectX;

class Sprite
{
	//コンスタントバッファー
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matW;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};
	
protected:
	INT64 vertexNum_; //頂点数  今いらない
	UINT64 indexNum_; //インデックス数
	
	std::vector<int> index_; //インデックス情報
	std::vector<VERTEX> vertices_;  //頂点情報

	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_; //インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	Texture* pTexture_; //テクスチャ

public:
	Sprite();
	~Sprite();

	//初期化（ポリゴンを表示するための各種情報を準備
	//戻り値：成功/失敗
	HRESULT Load(string fileName);

	//描画
	//引数：transform	トランスフォームクラスオブジェクト
	void Draw(Transform& transform);

	//解放
	void Release();

	XMFLOAT3 GetTextureSize();

private:
	//  Initializeから呼ばれる関数-----------------
	virtual void InitVertexData(); //頂点情報の準備
	HRESULT CreateVertexBuffer();  //頂点バッファを作成

	virtual void InitIndexData();  //インデックス情報を準備
	HRESULT CreateIndexBuffer();  //インデックスバッファを作成

	HRESULT CreateConstantBuffer(); //コンスタントバッファを作成

	HRESULT LoadTexture(string fileName); //テクスチャをロード

	// Draw関数から呼ばれる関数--------------------
	void PassDataToCB(XMMATRIX worldMatrix);	//コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline();                         //各バッファ

};

