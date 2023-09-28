#include "Texture.h"
#include "Direct3D.h"
#include <DirectXTex.h>
#include <wincodec.h>

#pragma comment( lib, "DirectXTex.lib")
#pragma comment( lib, "WindowsCodecs.lib" )

Texture::Texture()
	:pSampler_(nullptr),pSRV_(nullptr), size_(XMFLOAT3(0, 0, 0))
{
}

Texture::~Texture()
{
	Release();
}

HRESULT Texture::Load(string fileName)
{
	using namespace DirectX;
	//�摜�ǂݍ��ݕ���

	wchar_t wtext[FILENAME_MAX];
	size_t ret;
	mbstowcs_s(&ret, wtext, fileName.c_str(), fileName.length());

	TexMetadata metadata; //�t���̃f�[�^�����Ƃ�
	ScratchImage image;
	HRESULT hr;
	hr = LoadFromWICFile(wtext, WIC_FLAGS::WIC_FLAGS_NONE, &metadata, image);

	if (FAILED(hr)) {
		MessageBox(nullptr, "�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}

	//�摜�T�C�Y��ۑ�
    size_.x = static_cast<float>(metadata.width);
    size_.y = static_cast<float>(metadata.height);
    size_.z = 0.0f;

	//�T���v���[�̍쐬
	D3D11_SAMPLER_DESC  SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = Direct3D::pDevice_->CreateSamplerState(&SamDesc, &pSampler_);
	if (FAILED(hr)) {
		MessageBox(nullptr, "�T���v���[�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}

	//�V�F�[�_�[���\�[�X�r���[�̍쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	hr = CreateShaderResourceView(Direct3D::pDevice_, image.GetImages(), image.GetImageCount(), metadata, &pSRV_);
	if (FAILED(hr)) {
		MessageBox(nullptr, "�V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void Texture::Release()
{
	SAFE_RELEASE(pSampler_);
	SAFE_RELEASE(pSRV_);
}
