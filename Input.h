#pragma once

#include <dInput.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib") //�o�[�W�����W�̗��R��input�Ɋւ��Ă�update���邱�Ƃ��Ȃ�����

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Input
{
	void Initialize(HWND hWnd);
	void Update();
	bool IsKey(int keyCode);
	void Release();
};