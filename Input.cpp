#include "Input.h"
#include <DirectXMath.h> //XMVECTOR ���g�����߂̂��

namespace Input
{
	//LP�`�`�`�`���ĂȂ��Ă�^��������A�|�C���^���Ǝv���Ă���
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr; //key���Ǘ�����|�C���^ key, maouse ���ꂼ��ɕK�v
	BYTE keyState[256];        //���݂̊e�L�[�̏��
	BYTE prevKeyState[256];    //�O�t���[���ł̊e�L�[�̏��
	XMVECTOR mousePosition;

	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);

		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard); //KeyBoard�������new ���Ă�
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}

	void Update()
	{
		memcpy(prevKeyState, keyState, 256); //�������[�R�s�[

		//�����ŃL�[�{�[�h�̏󋵂�ۑ����Ă���炵��
		pKeyDevice->Acquire(); //�A�N���C���[�K���������Ă�L�[�{�[�h���������Ă��܂��炵��
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState); //�������
	}

	bool IsKey(int keyCode)
	{
		//1byte�̒���1bit����flag���ǂ��Ȃ��Ă��邩�m�肽��
		//�Ώۂ�����1�ɂȂ�2�i���Ƃ�AND���Z������ΑΏۂ������ǂ��Ȃ��Ă��邩���킩��
		//KeyCode�Ŏw�肵���Ƃ��� 8bit�̈�ԍ��ɂȂ� ���� 128 �� ��ԍ������P�ɂȂ�2�i���̒l��AND���Z������

		if (keyState[keyCode] & 0x80) //128��16�i����=0x80 || 1<<7���P������7��V�t�g����
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		//�����Ă��锻��ƁA�����Ă��Ȃ�����
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