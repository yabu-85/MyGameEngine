//インクルード
#include <Windows.h>
#include <d3d11.h>

//リンカ
#pragma comment(lib, "d3d11.lib")

//定数宣言
const char* WIN_CLASS_NAME = "SampleGame";
const char* WIN_CLASS_TITLEBAR = "サンプルゲーム";
const int WINDOW_WIDTH = 800;  //ウィンドウの幅
const int WINDOW_HEIGHT = 600; //ウィンドウの高さ

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ID3D11Device* pDevice;		//デバイス
ID3D11DeviceContext* pContext;		//デバイスコンテキスト
IDXGISwapChain* pSwapChain;		//スワップチェイン
ID3D11RenderTargetView* pRenderTargetView;	//レンダーターゲットビュー

//エントリーポイント Handl Instance
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) //APIENTRY main にするよみたいな
{
	//ウィンドウクラス（設計図）を作成
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);             //この構造体のサイズ
	wc.hInstance = hInstance;                   //インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;            //ウィンドウクラス名
	wc.lpfnWndProc = WndProc;                   //ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;         //スタイル（デフォルト）
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //アイコン
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //小さいアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //マウスカーソル
	wc.lpszMenuName = NULL;                     //メニュー（なし）
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景（白）

	RegisterClassEx(&wc);  //クラスを登録 設計図を作ってレジに入れる


	//ウィンドウサイズの計算
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE); //ここのfalse はメニューがありなしか決める
	int winW = winRect.right - winRect.left;     //ウィンドウ幅
	int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ


	//ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //ウィンドウクラス名
		WIN_CLASS_TITLEBAR,     //タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW, //スタイル（普通のウィンドウ）
		CW_USEDEFAULT,       //表示位置左（おまかせ）
		CW_USEDEFAULT,       //表示位置上（おまかせ）
		WINDOW_WIDTH,                 //ウィンドウ幅
		WINDOW_HEIGHT,                 //ウィンドウ高さ
		NULL,                //親ウインドウ（なし）
		NULL,                //メニュー（なし）
		hInstance,           //インスタンス
		NULL                 //パラメータ（なし）
	);
	// | WS_VISIBLE  <--これつけるのでも表示できる サンプルゲームの下
	//WS_OVERLAPPEDWINDOW | WS_VISIBLE,

	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow); //作ったウィンドウが見えるようにする


	///////////////////////////いろいろ準備するための設定///////////////////////////////
	//いろいろな設定項目をまとめた構造体
	DXGI_SWAP_CHAIN_DESC scDesc;

	//とりあえず全部0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//描画先のフォーマット
	scDesc.BufferDesc.Width = WINDOW_WIDTH;		//画面幅
	scDesc.BufferDesc.Height = WINDOW_HEIGHT;	//画面高さ
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
	D3D11CreateDeviceAndSwapChain(
		nullptr,				// どのビデオアダプタを使用するか？既定ならばnullptrで
		D3D_DRIVER_TYPE_HARDWARE,		// ドライバのタイプを渡す。ふつうはHARDWARE
		nullptr,				// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しないかぎりnullptr
		0,					// 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
		nullptr,				// デバイス、コンテキストのレベルを設定。nullptrにしとけばOK
		0,					// 上の引数でレベルを何個指定したか
		D3D11_SDK_VERSION,			// SDKのバージョン。必ずこの値
		&scDesc,				// 上でいろいろ設定した構造体
		&pSwapChain,				// 無事完成したSwapChainのアドレスが返ってくる
		&pDevice,				// 無事完成したDeviceアドレスが返ってくる
		&level,					// 無事完成したDevice、Contextのレベルが返ってくる
		&pContext);				// 無事完成したContextのアドレスが返ってくる

	///////////////////////////レンダーターゲットビュー作成///////////////////////////////
	//スワップチェーンからバックバッファを取得（バックバッファ ＝ レンダーターゲット）
	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	//レンダーターゲットビューを作成
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	//一時的にバックバッファを取得しただけなので解放
	pBackBuffer->Release();



	///////////////////////////ビューポート（描画範囲）設定///////////////////////////////
	//レンダリング結果を表示する範囲
	D3D11_VIEWPORT vp;
	vp.Width = (float)WINDOW_WIDTH;	//幅
	vp.Height = (float)WINDOW_HEIGHT;//高さ
	vp.MinDepth = 0.0f;	//手前
	vp.MaxDepth = 1.0f;	//奥
	vp.TopLeftX = 0;	//左
	vp.TopLeftY = 0;	//上

	//データを画面に描画するための一通りの設定（パイプライン）
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // データの入力種類を指定
	pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);            // 描画先を設定
	pContext->RSSetViewports(1, &vp);


	//メッセージループ（何か起きるのを待つ）
	MSG msg; //MSG＝メッセージ入れるための型
	ZeroMemory(&msg, sizeof(msg)); //配列すべてに０を入れるやつ
	while (msg.message != WM_QUIT) //終了するやつが来るまで
	{
		//メッセージあり  メッセージが優先　メッセがなければゲームの処理
		//下のPeekMessage 関数もdefine されてる 
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) //PeekMessage ここにメッセがたまっていく
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//メッセージなし
		else
		{
			//ゲームの処理

			//背景の色
			float clearColor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };//R,G,B,A

			//画面をクリア
			pContext->ClearRenderTargetView(pRenderTargetView, clearColor);


			//描画処理

			//書く用と描画する用二つ用意して書き終わったら　書く用から描画へコピーして表示している
			//スワップ（バックバッファを表に表示する）
			pSwapChain->Present(0, 0);

		}
	}


	//create ＝ Relese
	//解放処理
	pRenderTargetView->Release();
	pSwapChain->Release();
	pContext->Release();
	pDevice->Release();
	return 0;
}

//条件が満たされたら自動的に呼ばれる関数＝コールバック                       自分の識別番号
//クリックと化されたら開いてるすべてのウィンドウでウィンドウプロシージャを読んで hWnd を渡す
//UINT にカーソル合わせるとわかるけど unsigned int の略で typedef を使うと命名できる
//W, L PARAM paramerter 詳細情報や追加情報などが入っている

//ウィンドウプロシージャ（何かあった時によばれる関数　左クリックとか
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY: //WM_DESTROY = ウィンドウが閉じられた  変数右クリ→宣言へ移動（全部見れるよ) 本当は全部のやつ書かないといけないけどめんどいから下defaultへ
		PostQuitMessage(0);  //プログラム終了
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam); //なんのcase にもかからなかったら Windowsのdefaultの機能をするようにしている
}