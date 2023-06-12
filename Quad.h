#pragma once
#include "Direct3D.h"
#include <DirectXMath.h> //XMVECTOR ���g�����߂̂��
#include "Texture.h"

using namespace DirectX;

//�R���X�^���g�o�b�t�@�[
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
};

//���_���
struct VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
};

class Quad
{
protected:
	//�K�������o�ϐ������������悤
	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	Texture* pTexture_;

public:
	Quad();
	virtual ~Quad();

	virtual HRESULT InitializeIndex();
	HRESULT InitializeConstantBuffer();

	void Draw(XMMATRIX& worldMatrix);
	virtual void DrawIndex(XMMATRIX& worldMatrix);
	void Release();
};