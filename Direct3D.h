#pragma once
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

	//������
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//�V�F�[�_�[����
	HRESULT InitShader();

	//�`��J�n
	void BeginDraw();

	//�`��I��
	void EndDraw();

	//���
	void Release();

};