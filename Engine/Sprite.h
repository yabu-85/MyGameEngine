#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include <DirectXMath.h> //XMVECTOR ���g�����߂̂��
#include <vector>
#include "Transform.h"

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
	HRESULT Load(string fileName);

	//�`��
	//�����Ftransform	�g�����X�t�H�[���N���X�I�u�W�F�N�g
	void Draw(Transform& transform);

	//���
	void Release();

	XMFLOAT3 GetTextureSize();

private:
	//  Initialize����Ă΂��֐�-----------------
	virtual void InitVertexData(); //���_���̏���
	HRESULT CreateVertexBuffer();  //���_�o�b�t�@���쐬

	virtual void InitIndexData();  //�C���f�b�N�X��������
	HRESULT CreateIndexBuffer();  //�C���f�b�N�X�o�b�t�@���쐬

	HRESULT CreateConstantBuffer(); //�R���X�^���g�o�b�t�@���쐬

	HRESULT LoadTexture(string fileName); //�e�N�X�`�������[�h

	// Draw�֐�����Ă΂��֐�--------------------
	void PassDataToCB(XMMATRIX worldMatrix);	//�R���X�^���g�o�b�t�@�Ɋe�����n��
	void SetBufferToPipeline();                         //�e�o�b�t�@

};

