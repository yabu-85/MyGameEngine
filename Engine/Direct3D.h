#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

//���̃����J�₯�ǂǂ��ɏ����Ă�����/�g���Ƃ��ɏ������ق����킩��₷�����ǂ�
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//define�͐��l�̑���ł͂Ȃ��ā@�����̒u������ ��s�ŏ�����Ƃ�����
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Direct3D
{
	//�O���ɂ������Ă����Đ錾 extern ���������[��
	extern ID3D11Device* pDevice_; //�f�o�C�X
	extern ID3D11DeviceContext* pContext_; //�f�o�C�X�R���e�L�X�g
	extern int scrWidth, scrHeight;

	//������
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//�V�F�[�_�[����
	HRESULT InitShader();
	HRESULT InitShader3D();
	HRESULT InitShader2D();
	HRESULT InitShaderPoint();

	enum SHADER_TYPE {
		SHADER_2D,
		SHADER_3D,
		SHADER_POINT,
		SHADER_MAX
	};

	enum BLEND_MODE
	{
		BLEND_DEFAULT,
		BLEND_ADD,
		BLEND_MAX
	};

	void SetShader(SHADER_TYPE type);
	void SetBlendMode(BLEND_MODE blendMode);

	//�`��J�n
	void BeginDraw();

	//�`��I��
	void EndDraw();

	//���
	void Release();

	//Z�o�b�t�@�ւ̏�������ON/OFF
	//�����FisWrite	  true=��������ON�^false=��������OFF
	void SetDepthBafferWriteEnable(bool isWrite);

};