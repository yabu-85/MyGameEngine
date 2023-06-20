#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include <DirectXMath.h> //XMVECTOR ���g�����߂̂��
#include <vector>

using namespace DirectX;

class Sprite
{
	//�R���X�^���g�o�b�t�@�[
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matW;
	};

	//���_���
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};
	
protected:
	INT64 vertexNum_; //���_��  ������Ȃ�
	UINT64 indexNum_; //�C���f�b�N�X��
	
	std::vector<int> index_; //�C���f�b�N�X���
	std::vector<VERTEX> vertices_;  //���_���

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_; //�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	Texture* pTexture_; //�e�N�X�`��

public:
	Sprite();
	~Sprite();

	//�������i�|���S����\�����邽�߂̊e���������
	//�߂�l�F����/���s
	HRESULT Initialize();

	//�`��
	//�����FworldMatrix ���[���h�s��
	void Draw(XMMATRIX& worldMatrix);

	//���
	void Release();

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

