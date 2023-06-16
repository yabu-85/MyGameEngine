#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include <DirectXMath.h> //XMVECTOR ���g�����߂̂��
#include <vector>

using namespace DirectX;

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}

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
	UINT64 vertexNum_; //���_��
	std::vector<VERTEX> vertices_;  //���_���
	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@

	UINT64 indexNnum; //�C���f�b�N�X���@�g�����킩���
	std::vector<int> index_; //�C���f�b�N�X���

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

