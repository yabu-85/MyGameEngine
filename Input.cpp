#include "Input.h"

namespace Input
{
	//LP～～～～ってなってる型だったら、ポインタだと思っていい
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr; //keyを管理するポインタ key, maouse それぞれに必要
	BYTE keyState[256] = { 0 };
	
	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);

		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard); //KeyBoardだよってnew してる
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}

	void Update()
	{
		pKeyDevice->Acquire(); //アクワイヤー必ず入れろってやつキーボードを見失ってしまうらしい
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState); //これもね
	}

	bool IsKey(int keyCode)
	{
		//1byteの中の1bitだけflagがどうなっているか知りたい
		//対象だけが1になる2進数とでAND演算をすれば対象だけがどうなっているかがわかる
		//KeyCodeで指定したとこが 8bitの一番左になる から 128 の 一番左だけ１になる2進数の値でAND演算をする
		if (keyState[keyCode] & 128)
		{
			return true;
		}
		return false;
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
	}
}