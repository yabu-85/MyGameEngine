#include "Input.h"
#include <DirectXMath.h> //XMVECTOR ���g�����߂̂��
#include <string>

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
	XMFLOAT3  mousePosition;							//�}�E�X�J�[�\���̈ʒu

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
		if (keyState[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		if ( IsKey(keyCode) && !prevKeyState[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}
	bool IsKeyUp(int keyCode)
	{
		if (!IsKey(keyCode) && prevKeyState[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}
	int KeyButton()
	{
		for (int i = DIK_ESCAPE; i <= DIK_LWIN; i++) //���m����ŏ�����A�Ō�
		{
			if (IsKeyUp(i))
			{
				return i;
			}
		}

		return 0;
	}

	//�[�|�|�|�|�|�|�|�}�E�X�[�|�|�|�|�|�|�|�|�|�|�|

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

	XMFLOAT3 GetMousePosition()
	{
		return mousePosition;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition.x = x;
		mousePosition.y = y;
		std::string resStr = std::to_string(x) + "," + std::to_string(y) + "\n";
		OutputDebugString(resStr.c_str());
	}

	//���̃t���[���ł̃}�E�X�̈ړ��ʂ��擾
	XMFLOAT3 GetMouseMove()
	{
		XMFLOAT3 result = XMFLOAT3((float)mouseState.lX, (float)mouseState.lY, (float)mouseState.lZ);
		return result;
	}

}