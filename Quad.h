#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Camera.h"
#include "Texture.h"
#include "Direct3D.h"

using namespace DirectX;

//�R���X�^���g�o�b�t�@�[
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
	XMMATRIX	matNormal; //matW����Normal�p�ɉ���
};

//���_���
struct VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
	XMVECTOR normal;
};

class Quad
{
protected:
	std::vector<int> index_; //�C���f�b�N�X���
	int indexNum_;
	int vertexNum_;
	std::vector<VERTEX> vertices_;

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	Texture* pTexture_;

public:
	Quad();
	virtual ~Quad();

	void Release();
	HRESULT Initialize();
	void Draw(XMMATRIX& worldMatrix);
	
private:
	//  Initialize����Ă΂��֐�-----------------
	virtual void InitVertexData(); //���_���̏���
	HRESULT CreateVertexBuffer();  //���_�o�b�t�@���쐬

	virtual void InitIndexData();  //�C���f�b�N�X��������
	HRESULT CreateIndexBuffer();  //�C���f�b�N�X�o�b�t�@���쐬

	HRESULT CreateConstantBuffer(); //�R���X�^���g�o�b�t�@���쐬

	HRESULT LoadTexture(); //�e�N�X�`�������[�h


	// Draw�֐�����Ă΂��֐�--------------------
	void PassDataToCB(DirectX::XMMATRIX& worldMatrix);  //�R���X
	void SetBufferToPipeline();                         //�e�o�b�t�@
};