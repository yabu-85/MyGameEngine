#include "Dice.h"
#include "Camera.h"

Dice::Dice()
{
	pVertexBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pConstantBuffer_ = nullptr;
	pTexture_ = nullptr;
}

Dice::~Dice()
{
	Quad::Release();
}



HRESULT Dice::InitializeVertex()
{

	// ���_���
	VERTEX vertices[] =
	{
		//1�ʁi�O
		{ XMVectorSet(-1.0f,  1.0f, -1.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) },  //���� 0
		{ XMVectorSet(1.0f,  1.0f, -1.0f, 0.0f), XMVectorSet(0.25f, 0.0f, 0.0f, 0.0f) },  //�E�� 1
		{ XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f), XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f) },  //�E�� 2
		{ XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f), XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f) },  //���� 3

		//2�ʁi�E
		{ XMVectorSet(1.0f,  1.0f, -1.0f, 0.0f), XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f) },   //���� 4
		{ XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f) },    //�E�� 5
		{ XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f), XMVectorSet(0.25f, 1.0f, 0.0f, 0.0f) },   //�E�� 6
		{ XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) },   //���� 7

		//3�ʁi��
		{ XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f) },    //���� 8
		{ XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f) },    //�E�� 9
		{ XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f), XMVectorSet(0.5f, 1.0f, 0.0f, 0.0f) },   //�E�� 10
		{ XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f), XMVectorSet(0.25f, 1.0f, 0.0f, 0.0f) },   //���� 11

		//4�ʁi��
		{ XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(0.25f, 0.0f, 0.0f, 0.0f) },   //���� 12
		{ XMVectorSet(-1.0f,  1.0f, -1.0f, 0.0f), XMVectorSet(0.5f, 0.0f, 0.0f, 0.0f) },  //�E�� 13
		{ XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f), XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f) },  //�E�� 14
		{ XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f), XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f) },  //���� 15

		//5�ʁi��j
		{ XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(0.75f, 0.0f, 0.0f, 0.0f) },   //���� 16
		{ XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) },     //�E�� 17
		{ XMVectorSet(1.0f,  1.0f, -1.0f, 0.0f), XMVectorSet(1.0f, 0.5f, 0.0f, 0.0f) },  //�E�� 18
		{ XMVectorSet(-1.0f,  1.0f, -1.0f, 0.0f), XMVectorSet(0.75f, 0.5f, 0.0f, 0.0f) },  //���� 19

		//6�ʁi���j
		{ XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f), XMVectorSet(0.5f, 0.0f, 0.0f, 0.0f) },  //���� 20
		{ XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f), XMVectorSet(0.75f, 0.0f, 0.0f, 0.0f) },  //�E�� 21
		{ XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f), XMVectorSet(0.75f, 0.5f, 0.0f, 0.0f) },   //�E�� 22
		{ XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f), XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f) },   //���� 23

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

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	///�w�b�_�[�������ĂȂ����猩�����Ȃ� �Ώ� Direct3D�w�b�_�[�ŏ����Ă��
	if (FAILED(hr)) {

		//�G���[����
		MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
}

HRESULT Dice::InitializeIndex()
{
	//�C���f�b�N�X��� �O�E�E�E���E���E��E��
	int index[] = { 0,1,2, 0,2,3, 4,5,6, 4,6,7, 8,9,10, 8,10,11, 12,13,14, 12,14,15, 16,17,18, 16,18,19, 20,21,22, 20,22,23 }; ///���v���Ȃ�230,012 �ł�����

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

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
}

void Dice::DrawIndex(XMMATRIX& worldMatrix)
{
	Direct3D::pContext_->DrawIndexed(36, 0, 0); //Dice initialize �̃C���f�b�N�X���ł��ꂽ��

}