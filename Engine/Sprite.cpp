#include "Sprite.h"

Sprite::Sprite():vertices_(0), index_(0), pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr), pTexture_(nullptr)
{
}

Sprite::~Sprite()
{
	Sprite::Release();
}

HRESULT Sprite::Load(string fileName)
{
	HRESULT hr;
	hr = CreateVertexBuffer(); //頂点情報の準備
	if (FAILED(hr)) {
		MessageBox(nullptr, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	hr = CreateIndexBuffer();  //インデックスバッファを作成
	if (FAILED(hr)) {
		MessageBox(nullptr, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	hr = CreateConstantBuffer(); //コンスタントバッファを作成
	if (FAILED(hr)) {
		MessageBox(nullptr, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	hr = LoadTexture(fileName); //テクスチャのロード
	if (FAILED(hr)) {
		MessageBox(nullptr, "テクスチャのロードに失敗しました", "エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

void Sprite::Draw(Transform& transform)
{
	Direct3D::SetShader(Direct3D::SHADER_2D);


	transform.Calclation();//トランスフォームを計算

	//コンスタントバッファに情報を渡す
	PassDataToCB(transform.GetWorldMatrix());

	//頂点バッファ、インデックスバッファ、コンスタントバッファをパイプラインにセット
	SetBufferToPipeline();

	//描画
	Direct3D::pContext_->DrawIndexed(indexNum_, 0, 0);
}


void Sprite::Release()
{

	SAFE_RELEASE(pTexture_);
	SAFE_RELEASE(pVertexBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pConstantBuffer_);
}

XMFLOAT3 Sprite::GetTextureSize()
{
	return pTexture_->GetSize();
}


//private

void Sprite::InitVertexData()
{
	// 頂点情報
	vertices_ =
	{
		{ XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f) },   // 四角形の頂点（左上）0
		{ XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) },   // 四角形の頂点（右上）1
		{ XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f) },   // 四角形の頂点（右下）2
		{ XMVectorSet(-1.0f,-1.0f, 0.0f, 0.0f), XMVectorSet(0.0f,  1.0f, 0.0f, 0.0f) },   // 四角形の頂点（左下）3
	};

	vertexNum_ = vertices_.size();	//全データのサイズ　÷　1頂点分のサイズ　＝　頂点数
}

HRESULT Sprite::CreateVertexBuffer()
{
	InitVertexData();

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertices_.size();
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data();

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr)) {
		MessageBox(nullptr, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

}

void Sprite::InitIndexData()
{
	//インデックス情報
	index_ = { 0,2,3, 0,1,2 };

	indexNum_ = index_.size();
}

HRESULT Sprite::CreateIndexBuffer()
{
	InitIndexData();

	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum_;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;

	InitData.pSysMem = index_.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr)) {
		MessageBox(nullptr, "インデックスバッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Sprite::CreateConstantBuffer()
{
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	HRESULT hr;
	// コンスタントバッファの作成
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr)) {
		MessageBox(nullptr, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK); //
		return hr;
	}

	return S_OK;
}

HRESULT Sprite::LoadTexture(string fileName)
{
	pTexture_ = new Texture;
	HRESULT hr;
	hr = pTexture_->Load(fileName);
	if (FAILED(hr)) {
		MessageBox(nullptr, "テクスチャのロードに失敗しました", "エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

void Sprite::PassDataToCB(XMMATRIX worldMatrix)
{
	//コンスタントバッファに渡す情報
	CONSTANT_BUFFER cb;
	D3D11_MAPPED_SUBRESOURCE pdata;

	///transpose = マトリックス座標の縦横を入れ替えるやつ
	cb.matW = XMMatrixTranspose(worldMatrix);

	Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//再開
}

void Sprite::SetBufferToPipeline()
{
	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

}
