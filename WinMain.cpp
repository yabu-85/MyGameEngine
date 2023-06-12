//インクルード
#include <Windows.h>
#include "Direct3D.h"
#include "Quad.h"
#include "Dice.h"
#include "Camera.h"

//定数宣言
const char* WIN_CLASS_NAME = "SampleGame";
const char* WIN_CLASS_TITLEBAR = "サンプルゲーム";
const int WINDOW_WIDTH = 800;  //ウィンドウの幅
const int WINDOW_HEIGHT = 600; //ウィンドウの高さ

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

	//Direct3D初期化
	HRESULT hr;
	hr = Direct3D::Initialize(winW, winH, hWnd);
	if (FAILED(hr)) {
		PostQuitMessage(0);  //プログラム終了
	}

	Camera::Initialize();
	Camera::SetTarget(XMFLOAT3(0, 0, 0));

#if 0
	Quad* q = new Quad;
	
	hr = q->InitializeVertex();
	if (FAILED(hr)) PostQuitMessage(0); 

	hr = q->InitializeIndex();
	if (FAILED(hr)) PostQuitMessage(0);

	hr = q->InitializeConstantBuffer();
	if (FAILED(hr)) PostQuitMessage(0);

#else
	Dice* d = new Dice;

	hr = d->InitializeVertex();
	if (FAILED(hr)) PostQuitMessage(0);

	hr = d->InitializeIndex();
	if (FAILED(hr)) PostQuitMessage(0);

	hr = d->InitializeConstantBuffer();
	if (FAILED(hr)) PostQuitMessage(0);

#endif

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
			Camera::Update();

			//ゲームの処理
			Direct3D::BeginDraw();

#if 0
			XMMATRIX matT = XMMatrixTranslation(0, 0, 0);

			q->Draw(matT);
			q->DrawIndex(matT);

#else
			static float a = 0;
			a += 0.04f;

			XMMATRIX matR1 = XMMatrixRotationY(XMConvertToRadians(a)); //n軸に何度回転するラジアンの値を出す
			XMMATRIX matR2 = XMMatrixRotationX(XMConvertToRadians(a + a)); //n軸に何度回転するラジアンの値を出す
			XMMATRIX matT = XMMatrixTranslation(0, 0, 0);
			XMMATRIX matS = XMMatrixScaling(1, 1, 1);

			//R * Tだとまわり回転する
			XMMATRIX mat = matR1 * matR2 * matT * matS;

			d->Draw(mat); //参照で受け取ってるから計算式を渡せない？
			d->DrawIndex(mat);

#endif

			//描画処理
			Direct3D::EndDraw();

		}
	}

	Direct3D::Release();

#if 0
	SAFE_DELETE(q);
#else
	SAFE_DELETE(d);
#endif

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