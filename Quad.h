#pragma once
#include "Direct3D.h"
#include <DirectXMath.h> //XMVECTOR ���g�����߂̂��

using namespace DirectX;

//define�͐��l�̑���ł͂Ȃ��ā@�����̒u������ ��s�ŏ�����Ƃ�����
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

//�R���X�^���g�o�b�t�@�[
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
};

class Quad
{
	//�K�������o�ϐ������������悤
	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@

public:
	Quad();
	~Quad();
	HRESULT Initialize();
	void Draw();
	void Release();
};