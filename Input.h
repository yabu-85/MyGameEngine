#pragma once

#include <dInput.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib") //バージョン８の理由はinputに関してはupdateすることがないから

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Input
{
	void Initialize(HWND hWnd);
	void Update();
	void Release();

	bool IsKey(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsKeyUp(int keyCode);
	
	bool IsMouseButton(int buttonCode);
	bool IsMouseButtonDown(int buttonCode);
	bool IsMouseButtonUp(int buttonCode);

	XMVECTOR GetMousePosition();
	void SetMousePosition(int x, int y);
	XMFLOAT3 GetMouseMove();
};