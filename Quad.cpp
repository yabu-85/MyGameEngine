#include "Quad.h"

Quad::Quad():pVertexBuffer_(nullptr),pIndexBuffer_(nullptr)
{
}

Quad::~Quad()
{
}

void Quad::Initialize()
{
	// ���_���
	XMVECTOR vertices[] =
	{
		XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),	// �l�p�`�̒��_�i����j
		XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),	// �l�p�`�̒��_�i�E��j
		XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),	// �l�p�`�̒��_�i�E���j
		XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),	// �l�p�`�̒��_�i�����j		
	};


	// ���_�f�[�^�p�o�b�t�@�̐ݒ�
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	//�w�b�_�[�������ĂȂ����猩�����Ȃ� �Ώ� Direct3D�w�b�_�[�ŏ����Ă��


	//�C���f�b�N�X���
	int index[] = { 0,2,3, 0,1,2 }; //���v���Ȃ�230,012 �ł�����

	// �C���f�b�N�X�o�b�t�@�𐶐�����
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);

	//�R���X�^���g�o�b�t�@�쐬
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
}

void Quad::Draw()
{
}

void Quad::Release()
{
}