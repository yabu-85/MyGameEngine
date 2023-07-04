#include "Quad.h"
#include "Camera.h"

Quad::Quad():pVertexBuffer_(nullptr),pIndexBuffer_(nullptr), pConstantBuffer_(nullptr),pTexture_(nullptr), vertexNum_(0), indexNum_(0)
{
}

Quad::~Quad()
{
	Quad::Release();
}

void Quad::Release()
{
	SAFE_RELEASE(pTexture_);
	SAFE_RELEASE(pVertexBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pConstantBuffer_);
}

HRESULT Quad::Initialize()
{
	HRESULT hr;
	hr = CreateVertexBuffer(); //���_���̏���
	if (FAILED(hr)) {
		MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	hr = CreateIndexBuffer();  //�C���f�b�N�X�o�b�t�@���쐬
	if (FAILED(hr)) {
		MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	hr = CreateConstantBuffer(); //�R���X�^���g�o�b�t�@���쐬
	if (FAILED(hr)) {
		MessageBox(nullptr, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	hr = LoadTexture(); //�e�N�X�`���̃��[�h
	if (FAILED(hr)) {
		MessageBox(nullptr, "�e�N�X�`���̃��[�h�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

void Quad::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_3D);

	transform.Calclation();//�g�����X�t�H�[�����v�Z

	PassDataToCB(transform);
	SetBufferToPipeline();

	Direct3D::pContext_->DrawIndexed(index_.size(), 0, 0);
}


//private

void Quad::InitVertexData()
{
	// ���_���
	vertices_ =
	{
		{ XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, 1.0f, -1.0f, 0.0f) },   // �l�p�`�̒��_�i����j0
		{ XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 1.0f, -1.0f, 0.0f) },   // �l�p�`�̒��_�i�E��j1
		{ XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f) },   // �l�p�`�̒��_�i�E���j2
		{ XMVectorSet(-1.0f,-1.0f, 0.0f, 0.0f), XMVectorSet(0.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f) },   // �l�p�`�̒��_�i�����j3
	};

	vertexNum_ = vertices_.size();
}

HRESULT Quad::CreateVertexBuffer()
{
	InitVertexData();
	
	// ���_�f�[�^�p�o�b�t�@�̐ݒ�
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexNum_;

	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;

	data_vertex.pSysMem = vertices_.data();

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr)) {

		//�G���[����
		MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
}

void Quad::InitIndexData()
{
	//�C���f�b�N�X���
	index_ = { 0,2,3, 0,1,2 };

	indexNum_ = sizeof(index_);
}

HRESULT Quad::CreateIndexBuffer()
{
	InitIndexData();

	// �C���f�b�N�X�o�b�t�@�𐶐�����
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum_;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index_.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

void Quad::PassDataToCB(Transform transform)
{
	CONSTANT_BUFFER cb;

	//�R���X�^���g�o�b�t�@�ɓn�����
	cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());

	//�R���X�^���g�o�b�t�@�̍X�V
	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// �f�[�^��l�𑗂�

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//�ĊJ
}

void Quad::SetBufferToPipeline()
{
	//���_�o�b�t�@
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//�R���X�^���g�o�b�t�@
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p

}

HRESULT Quad::CreateConstantBuffer()
{
	//�R���X�^���g�o�b�t�@�쐬
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	HRESULT hr;
	// �R���X�^���g�o�b�t�@�̍쐬
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK); //
		return hr;
	}
}

HRESULT Quad::LoadTexture()
{
	pTexture_ = new Texture;

	HRESULT hr;
	hr = pTexture_->Load("Assets\\Dice.png");
	if (FAILED(hr))
	{
		MessageBox(NULL, "�e�N�X�`���̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
	return S_OK;
}
