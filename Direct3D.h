#pragma once

#include <d3d11.h>

//���̃����J�₯�ǂǂ��ɏ����Ă�����/�g���Ƃ��ɏ������ق����킩��₷�����ǂ�
//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace Direct3D
{
	//�O���ɂ������Ă����Đ錾 extern
	extern ID3D11Device* pDevice; //�f�o�C�X

	//������
	void Initialize(int winW, int winH, HWND hWnd);

	//�V�F�[�_�[����
	void InitShader();

	//�`��J�n
	void BeginDraw();

	//�`��I��
	void EndDraw();

	//���
	void Release();

};