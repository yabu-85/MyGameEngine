#include "Input.h"
#include <DirectXMath.h> //XMVECTOR を使うためのやつ

namespace Input
{
	//LP～～～～ってなってる型だったら、ポインタだと思っていい
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr; //keyを管理するポインタ key, maouse それぞれに必要
	BYTE keyState[256];        //現在の各キーの状態
	BYTE prevKeyState[256];    //前フレームでの各キーの状態
	XMVECTOR mousePosition;

	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);

		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard); //KeyBoardだよってnew してる
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}

	void Update()
	{
		memcpy(prevKeyState, keyState, 256); //メモリーコピー

		//ここでキーボードの状況を保存しているらしい
		pKeyDevice->Acquire(); //アクワイヤー必ず入れろってやつキーボードを見失ってしまうらしい
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState); //これもね
	}

	bool IsKey(int keyCode)
	{
		//1byteの中の1bitだけflagがどうなっているか知りたい
		//対象だけが1になる2進数とでAND演算をすれば対象だけがどうなっているかがわかる
		//KeyCodeで指定したとこが 8bitの一番左になる から 128 の 一番左だけ１になる2進数の値でAND演算をする

		if (keyState[keyCode] & 0x80) //128を16進数で=0x80 || 1<<7＝１を左に7回シフトした
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		//押している判定と、押していない判定
		if (keyState[keyCode] & 0x80 && !prevKeyState[keyCode] & 0x7F)
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		if (!keyState[keyCode] & 0x7F && prevKeyState[keyCode] & 0x80)
		{	
			return true;
		}
		return false;
	}

	XMVECTOR GetMousePosition()
	{
		return mousePosition;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition = XMVectorSet((float)x, (float)y, 0, 0);
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
	}
}