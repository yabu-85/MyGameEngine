#include "Input.h"
#include <DirectXMath.h> //XMVECTOR ���g�����߂̂��

namespace Input
{
	//LP�`�`�`�`���ĂȂ��Ă�^��������A�|�C���^���Ǝv���Ă���
	//DirectInput�I�u�W�F�N�g
	LPDIRECTINPUT8   pDInput = nullptr;

	//�L�[�{�[�h
	LPDIRECTINPUTDEVICE8 pKeyDevice;		//�f�o�C�X�I�u�W�F�N�g
	BYTE keyState[256]; 					//���݂̊e�L�[�̏��
	BYTE prevKeyState[256]; 				//�O�t���[���ł̊e�L�[�̏��

	//�}�E�X
	LPDIRECTINPUTDEVICE8 pMouseDevice;	    //�f�o�C�X�I�u�W�F�N�g
	DIMOUSESTATE mouseState;				//�}�E�X�̏��
	DIMOUSESTATE prevMouseState;			//�O�t���[���̃}�E�X�̏��
	XMVECTOR  mousePosition;							//�}�E�X�J�[�\���̈ʒu

	void Initialize(HWND hWnd)
	{
		//DirectInput�{��
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);

		//�L�[�{�[�h
		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard); //KeyBoard�������new ���Ă�
		pKeyDevice->SetCooperativeLevel(NULL, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND); 

		//�}�E�X	
		pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, nullptr);
		pMouseDevice->SetDataFormat(&c_dfDIMouse);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND); 

	}

	void Update()
	{
		memcpy(prevKeyState, keyState, 256); //�������[�R�s�[
		pKeyDevice->Acquire(); //�A�N���C���[�K���������Ă�L�[�{�[�h���������Ă��܂��炵��
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState); //�����ŃL�[�{�[�h�̏󋵂�ۑ����Ă���炵��
	
		//�}�E�X
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

	//�[�|�|�|�|�|�|�|�|�|�|�L�[�{�[�h�[�|�|�|�|�|�|�|�|�|�|�|�|�|
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
		//�����Ă��Ȃ�����ƁA�����Ă��锻��
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

	//�[�|�|�|�|�|�|�|�}�E�X�[�|�|�|�|�|�|�|�|�|�|�|

		//�}�E�X�̃{�^����������Ă��邩���ׂ�
	bool IsMouseButton(int buttonCode)
	{
		//�����Ă�
		if (mouseState.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//�}�E�X�̃{�^�����������������ׂ�i�������ςȂ��͖����j
	bool IsMouseButtonDown(int buttonCode)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if (IsMouseButton(buttonCode) && !(prevMouseState.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//�}�E�X�̃{�^�����������������ׂ�
	bool IsMouseButtonUp(int buttonCode)
	{
		//�������ĂȂ��āA�O��͉����Ă�
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

	//���̃t���[���ł̃}�E�X�̈ړ��ʂ��擾
	XMFLOAT3 GetMouseMove()
	{
		XMFLOAT3 result = XMFLOAT3((float)mouseState.lX, (float)mouseState.lY, (float)mouseState.lZ);
		return result;
	}

}