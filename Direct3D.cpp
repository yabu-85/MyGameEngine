#include <d3dcompiler.h> //誰かが作ったやつ <>
#include "Direct3D.h"	//自分で作ったやつ ""
#include <assert.h>
#include <vector>

//変数
namespace Direct3D
{
	//このクラスでしか使わない・見えないほうが安全だからcpp で作ってる

	ID3D11Device* pDevice_;		//デバイス
	ID3D11DeviceContext* pContext_;		//デバイスコンテキスト
	IDXGISwapChain* pSwapChain_;		//スワップチェイン
	ID3D11RenderTargetView* pRenderTargetView_;	//レンダーターゲットビュー

	ID3D11VertexShader* pVertexShader = nullptr;	//頂点シェーダー
	ID3D11PixelShader* pPixelShader_ = nullptr;		//ピクセルシェーダー
	ID3D11InputLayout* pVertexLayout_ = nullptr;	//頂点インプットレイアウト
	ID3D11RasterizerState* pRasterizerState_ = nullptr;	//ラスタライザー
}



//初期化
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{
	///////////////////////////いろいろ準備するための設定///////////////////////////////
	//いろいろな設定項目をまとめた構造体
	DXGI_SWAP_CHAIN_DESC scDesc;

	//とりあえず全部0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//描画先のフォーマット
	scDesc.BufferDesc.Width = winW;		//画面幅
	scDesc.BufferDesc.Height = winH;	//画面高さ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 何色使えるか

	//FPS（1/60秒に1回）
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//その他
	scDesc.Windowed = TRUE;			//ウィンドウモードかフルスクリーンか
	scDesc.OutputWindow = hWnd;		//ウィンドウハンドル
	scDesc.BufferCount = 1;			//バックバッファの枚数
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バックバッファの使い道＝画面に描画するために
	scDesc.SampleDesc.Count = 1;		//MSAA（アンチエイリアス）の設定
	scDesc.SampleDesc.Quality = 0;		//　〃


	////////////////上記設定をもとにデバイス、コンテキスト、スワップチェインを作成////////////////////////
	D3D_FEATURE_LEVEL level;

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,				// どのビデオアダプタを使用するか？既定ならばnullptrで
		D3D_DRIVER_TYPE_HARDWARE,		// ドライバのタイプを渡す。ふつうはHARDWARE
		nullptr,				// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しないかぎりnullptr
		0,					// 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
		nullptr,				// デバイス、コンテキストのレベルを設定。nullptrにしとけばOK
		0,					// 上の引数でレベルを何個指定したか
		D3D11_SDK_VERSION,			// SDKのバージョン。必ずこの値
		&scDesc,				// 上でいろいろ設定した構造体
		&pSwapChain_,				// 無事完成したSwapChainのアドレスが返ってくる
		&pDevice_,				// 無事完成したDeviceアドレスが返ってくる
		&level,					// 無事完成したDevice、Contextのレベルが返ってくる
		&pContext_);				// 無事完成したContextのアドレスが返ってくる

	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "スワップチェインの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	///////////////////////////レンダーターゲットビュー作成///////////////////////////////
	//スワップチェーンからバックバッファを取得（バックバッファ ＝ レンダーターゲット）
	ID3D11Texture2D* pBackBuffer;
	hr = pSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "スワップチェインからバックバッファの取得/失敗しました", "エラー", MB_OK);
		return hr;
	}

	//レンダーターゲットビューを作成
	hr = pDevice_->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView_);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "レンダーターゲットビューの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	//一時的にバックバッファを取得しただけなので解放
	SAFE_RELEASE(pBackBuffer);

	///////////////////////////ビューポート（描画範囲）設定///////////////////////////////
	//レンダリング結果を表示する範囲
	D3D11_VIEWPORT vp;
	vp.Width = (float)winW;	//幅
	vp.Height = (float)winH;//高さ
	vp.MinDepth = 0.0f;	//手前
	vp.MaxDepth = 1.0f;	//奥
	vp.TopLeftX = 0;	//左
	vp.TopLeftY = 0;	//上

	//データを画面に描画するための一通りの設定（パイプライン）
	
	//TOPOLOGY物の配置のこと TRIANGLELIST トライアングルリスト か ストリップ
	pContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // データの入力種類を指定
	pContext_->OMSetRenderTargets(1, &pRenderTargetView_, nullptr);            // 描画先を設定
	pContext_->RSSetViewports(1, &vp);

	//シェーダー準備
	hr = InitShader();
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "シェーダーの準備に失敗しました", "エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

//シェーダー準備
HRESULT Direct3D::InitShader()
{
	HRESULT hr;

	//頂点シェーダの作成（コンパイル）
	/*
		ID3DBlob* pCompileVS = nullptr;							//バージョンの指定 ５_０
	hr = D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "頂点シェーダーのコンパイルに失敗しました", "エラー", MB_OK);
		return hr;
	}
	assert(pCompileVS != nullptr); ///////////

	hr = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "頂点シェーダーの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	*/

	//頂点インプットレイアウト

	/*
	 D3D11_INPUT_ELEMENT_DESC layout[] = {
		//位置データだけ
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 }, //UV座標
		{ "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(DirectX::XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },//法線
	};
	hr = pDevice_->CreateInputLayout(layout, 3, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout_);

	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "頂点インプットレイアウトに失敗しました", "エラー", MB_OK);
		return hr;
	}

	SAFE_RELEASE(pCompileVS)


	*/

	// ピクセルシェーダの作成（コンパイル）

	/*
		ID3DBlob* pCompilePS = nullptr;
	hr = D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "ピクセルシェーダーのコンパイルに失敗しました", "エラー", MB_OK);
		return hr;
	}

	assert(pCompilePS != nullptr);//////////

	hr = pDevice_->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader_);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "ピクセルシェーダーの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	SAFE_RELEASE(pCompilePS);


	*/

	//ラスタライザ作成
	/*
		D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK; //CULL
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE; //時計回りだけ表示される
	hr = pDevice_->CreateRasterizerState(&rdc, &pRasterizerState_); //クリエイト！
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "ラスタライザの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	*/
	


	//ここから２D

	//頂点シェーダの作成（コンパイル）
		ID3DBlob* pCompileVS = nullptr;							//バージョンの指定 ５_０
	hr = D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "頂点シェーダーのコンパイルに失敗しました", "エラー", MB_OK);
		return hr;
	}
	assert(pCompileVS != nullptr); ///////////

	hr = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "頂点シェーダーの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	//頂点インプットレイアウト
	 std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		//位置データだけ
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 }, //UV座標
	};
	hr = pDevice_->CreateInputLayout(layout.data(), layout.size(), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout_);
//	hr = pDevice_->CreateInputLayout(layout, 2, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout_); vector使った

	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "頂点インプットレイアウトに失敗しました", "エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS)

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	hr = D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "ピクセルシェーダーのコンパイルに失敗しました", "エラー", MB_OK);
		return hr;
	}

	assert(pCompilePS != nullptr);//////////

	hr = pDevice_->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader_);
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "ピクセルシェーダーの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	SAFE_RELEASE(pCompilePS);


	//ラスタライザ作成
		D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK; //CULL
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE; //時計回りだけ表示される
	hr = pDevice_->CreateRasterizerState(&rdc, &pRasterizerState_); //クリエイト！
	if (FAILED(hr)) {
		//エラー処理
		MessageBox(nullptr, "ラスタライザの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}






	//それぞれをデバイスコンテキストにセット
	pContext_->VSSetShader(pVertexShader, NULL, 0);	//頂点シェーダー
	pContext_->PSSetShader(pPixelShader_, NULL, 0);	//ピクセルシェーダー
	pContext_->IASetInputLayout(pVertexLayout_);	//頂点インプットレイアウト
	pContext_->RSSetState(pRasterizerState_);		//ラスタライザー

	return S_OK;
}

//描画開始
void Direct3D::BeginDraw()

{
	//背景の色
	float clearColor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };//R,G,B,A

	//画面をクリア
	pContext_->ClearRenderTargetView(pRenderTargetView_, clearColor);

}

//描画終了
void Direct3D::EndDraw()

{
	//書く用と描画する用二つ用意して書き終わったら　書く用から描画へコピーして表示している
	//スワップ（バックバッファを表に表示する）
	pSwapChain_->Present(0, 0);
}

//解放処理
void Direct3D::Release()
{
	///create ＝ Relese
	///解放処理 基本的に作った順にdeleteする　今回はいつでもおｋ

	SAFE_RELEASE(pRenderTargetView_);
	SAFE_RELEASE(pSwapChain_);
	SAFE_RELEASE(pContext_);
	SAFE_RELEASE(pDevice_);

	SAFE_RELEASE(pRasterizerState_);
	SAFE_RELEASE(pVertexLayout_);
	SAFE_RELEASE(pPixelShader_);
	SAFE_RELEASE(pVertexShader);
}