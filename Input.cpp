#include "Input.h"
#include <DirectXMath.h> //XMVECTOR を使うためのやつ

namespace Input
{
	//LP～～～～ってなってる型だったら、ポインタだと思っていい
	//DirectInputオブジェクト
	LPDIRECTINPUT8   pDInput = nullptr;

	//キーボード
	LPDIRECTINPUTDEVICE8 pKeyDevice;		//デバイスオブジェクト
	BYTE keyState[256]; 					//現在の各キーの状態
	BYTE prevKeyState[256]; 				//前フレームでの各キーの状態

	//マウス
	LPDIRECTINPUTDEVICE8 pMouseDevice;	    //デバイスオブジェクト
	DIMOUSESTATE mouseState;				//マウスの状態
	DIMOUSESTATE prevMouseState;			//前フレームのマウスの状態
	XMVECTOR  mousePosition;							//マウスカーソルの位置

	void Initialize(HWND hWnd)
	{
		//DirectInput本体
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);

		//キーボード
		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard); //KeyBoardだよってnew してる
		pKeyDevice->SetCooperativeLevel(NULL, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND); 

		//マウス	
		pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, nullptr);
		pMouseDevice->SetDataFormat(&c_dfDIMouse);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND); 

	}

	void Update()
	{
		memcpy(prevKeyState, keyState, 256); //メモリーコピー
		pKeyDevice->Acquire(); //アクワイヤー必ず入れろってやつキーボードを見失ってしまうらしい
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState); //ここでキーボードの状況を保存しているらしい
	
		//マウス
		memcpy(&prevMouseState, &mouseState, sizeof(mouseState));
		pMouseDevice->Acquire();
		pMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
	
	}

	void Release()
	{
		SAFE_RELEASE(pMouseDevice);
		SAFE_RELEASE(pKeyDevice);
		SAFE_RELEASE(pDInput);
	}

	//ー－－－－－－－－－－キーボードー－－－－－－－－－－－－－
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
		//押していない判定と、押している判定
		if (!prevKeyState[keyCode] & 0x80 && IsKey(keyCode))
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		if (prevKeyState[keyCode] & 0x80 && !IsKey(keyCode))
		{	
			return true;
		}
		return false;
	}

	//ー－－－－－－－マウスー－－－－－－－－－－－

		//マウスのボタンが押されているか調べる
	bool IsMouseButton(int buttonCode)
	{
		//押してる
		if (mouseState.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//マウスのボタンを今押したか調べる（押しっぱなしは無効）
	bool IsMouseButtonDown(int buttonCode)
	{
		//今は押してて、前回は押してない
		if (IsMouseButton(buttonCode) && !(prevMouseState.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//マウスのボタンを今放したか調べる
	bool IsMouseButtonUp(int buttonCode)
	{
		//今押してなくて、前回は押してる
		if (!IsMouseButton(buttonCode) && prevMouseState.rgbButtons[buttonCode] & 0x80)
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

	//そのフレームでのマウスの移動量を取得
	XMFLOAT3 GetMouseMove()
	{
		XMFLOAT3 result = XMFLOAT3((float)mouseState.lX, (float)mouseState.lY, (float)mouseState.lZ);
		return result;
	}

}