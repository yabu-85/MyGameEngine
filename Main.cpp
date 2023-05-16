//インクルード
#include <Windows.h>

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//エントリーポイント Handl Instance
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) //APIENTRY main にするよみたいな
{
	//ウィンドウクラス（設計図）を作成
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);             //この構造体のサイズ
	wc.hInstance = hInstance;                   //インスタンスハンドル
	wc.lpszClassName = "SampleGame";            //ウィンドウクラス名
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


	//ウィンドウを作成
	HWND hWnd = CreateWindow(
		"SampleGame",         //ウィンドウクラス名
		"サンプルゲーム",     //タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW, //スタイル（普通のウィンドウ）
		CW_USEDEFAULT,       //表示位置左（おまかせ）
		CW_USEDEFAULT,       //表示位置上（おまかせ）
		800,                 //ウィンドウ幅
		600,                 //ウィンドウ高さ
		NULL,                //親ウインドウ（なし）
		NULL,                //メニュー（なし）
		hInstance,           //インスタンス
		NULL                 //パラメータ（なし）
	);


	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow); //作ったウィンドウが見えるようにする


	//メッセージループ（何か起きるのを待つ）
	MSG msg; //MSG＝メッセージ入れるための型
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) //終了するやつが来るまで
	{
		//メッセージあり  メッセージが優先　メッセがなければゲームの処理
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//メッセージなし
		else
		{
			//ゲームの処理


		}
	}


	return 0;
}

//ウィンドウプロシージャ（何かあった時によばれる関数　左クリックとか
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY: //WM_DESTROY = ウィンドウが閉じられた  変数右クリ→宣言へ移動（全部見れるよ) 本当は全部のやつ書かないといけないけどめんどいから下return 
		PostQuitMessage(0);  //プログラム終了
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam); //なんのcase にもかからなかったら default だよ　ってやつ
}