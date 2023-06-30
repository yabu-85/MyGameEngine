#include "Input.h"

namespace Input
{
	//LP�`�`�`�`���ĂȂ��Ă�^��������A�|�C���^���Ǝv���Ă���
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr; //key���Ǘ�����|�C���^ key, maouse ���ꂼ��ɕK�v
	BYTE keyState[256] = { 0 };
	
	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);

		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard); //KeyBoard�������new ���Ă�
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}

	void Update()
	{
		pKeyDevice->Acquire(); //�A�N���C���[�K���������Ă�L�[�{�[�h���������Ă��܂��炵��
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState); //�������
	}

	bool IsKey(int keyCode)
	{
		//1byte�̒���1bit����flag���ǂ��Ȃ��Ă��邩�m�肽��
		//�Ώۂ�����1�ɂȂ�2�i���Ƃ�AND���Z������ΑΏۂ������ǂ��Ȃ��Ă��邩���킩��
		//KeyCode�Ŏw�肵���Ƃ��� 8bit�̈�ԍ��ɂȂ� ���� 128 �� ��ԍ������P�ɂȂ�2�i���̒l��AND���Z������
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