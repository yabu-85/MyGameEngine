#pragma once
#include "Direct3D.h"
#include <DirectXMath.h> //XMVECTOR ���g�����߂̂��

using namespace DirectX;

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
	void Initialize();
	void Draw();
	void Release();
};