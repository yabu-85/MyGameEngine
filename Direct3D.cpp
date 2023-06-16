#include <d3dcompiler.h> //�N������������ <>
#include "Direct3D.h"	//�����ō������� ""
#include <assert.h>
#include <vector>

//�ϐ�
namespace Direct3D
{
	//���̃N���X�ł����g��Ȃ��E�����Ȃ��ق������S������cpp �ō���Ă�

	ID3D11Device* pDevice_;		//�f�o�C�X
	ID3D11DeviceContext* pContext_;		//�f�o�C�X�R���e�L�X�g
	IDXGISwapChain* pSwapChain_;		//�X���b�v�`�F�C��
	ID3D11RenderTargetView* pRenderTargetView_;	//�����_�[�^�[�Q�b�g�r���[

	ID3D11VertexShader* pVertexShader = nullptr;	//���_�V�F�[�_�[
	ID3D11PixelShader* pPixelShader_ = nullptr;		//�s�N�Z���V�F�[�_�[
	ID3D11InputLayout* pVertexLayout_ = nullptr;	//���_�C���v�b�g���C�A�E�g
	ID3D11RasterizerState* pRasterizerState_ = nullptr;	//���X�^���C�U�[
}



//������
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{
	///////////////////////////���낢�돀�����邽�߂̐ݒ�///////////////////////////////
	//���낢��Ȑݒ荀�ڂ��܂Ƃ߂��\����
	DXGI_SWAP_CHAIN_DESC scDesc;

	//�Ƃ肠�����S��0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//�`���̃t�H�[�}�b�g
	scDesc.BufferDesc.Width = winW;		//��ʕ�
	scDesc.BufferDesc.Height = winH;	//��ʍ���
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// ���F�g���邩

	//FPS�i1/60�b��1��j
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//���̑�
	scDesc.Windowed = TRUE;			//�E�B���h�E���[�h���t���X�N���[����
	scDesc.OutputWindow = hWnd;		//�E�B���h�E�n���h��
	scDesc.BufferCount = 1;			//�o�b�N�o�b�t�@�̖���
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�o�b�N�o�b�t�@�̎g��������ʂɕ`�悷�邽�߂�
	scDesc.SampleDesc.Count = 1;		//MSAA�i�A���`�G�C���A�X�j�̐ݒ�
	scDesc.SampleDesc.Quality = 0;		//�@�V


	////////////////��L�ݒ�����ƂɃf�o�C�X�A�R���e�L�X�g�A�X���b�v�`�F�C�����쐬////////////////////////
	D3D_FEATURE_LEVEL level;

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,				// �ǂ̃r�f�I�A�_�v�^���g�p���邩�H����Ȃ��nullptr��
		D3D_DRIVER_TYPE_HARDWARE,		// �h���C�o�̃^�C�v��n���B�ӂ���HARDWARE
		nullptr,				// ��L��D3D_DRIVER_TYPE_SOFTWARE�ɐݒ肵�Ȃ�������nullptr
		0,					// ���炩�̃t���O���w�肷��B�i�f�o�b�O����D3D11_CREATE_DEVICE_DEBUG�H�j
		nullptr,				// �f�o�C�X�A�R���e�L�X�g�̃��x����ݒ�Bnullptr�ɂ��Ƃ���OK
		0,					// ��̈����Ń��x�������w�肵����
		D3D11_SDK_VERSION,			// SDK�̃o�[�W�����B�K�����̒l
		&scDesc,				// ��ł��낢��ݒ肵���\����
		&pSwapChain_,				// ������������SwapChain�̃A�h���X���Ԃ��Ă���
		&pDevice_,				// ������������Device�A�h���X���Ԃ��Ă���
		&level,					// ������������Device�AContext�̃��x�����Ԃ��Ă���
		&pContext_);				// ������������Context�̃A�h���X���Ԃ��Ă���

	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�X���b�v�`�F�C���̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	///////////////////////////�����_�[�^�[�Q�b�g�r���[�쐬///////////////////////////////
	//�X���b�v�`�F�[������o�b�N�o�b�t�@���擾�i�o�b�N�o�b�t�@ �� �����_�[�^�[�Q�b�g�j
	ID3D11Texture2D* pBackBuffer;
	hr = pSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�X���b�v�`�F�C������o�b�N�o�b�t�@�̎擾/���s���܂���", "�G���[", MB_OK);
		return hr;
	}

	//�����_�[�^�[�Q�b�g�r���[���쐬
	hr = pDevice_->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView_);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	//�ꎞ�I�Ƀo�b�N�o�b�t�@���擾���������Ȃ̂ŉ��
	SAFE_RELEASE(pBackBuffer);

	///////////////////////////�r���[�|�[�g�i�`��͈́j�ݒ�///////////////////////////////
	//�����_�����O���ʂ�\������͈�
	D3D11_VIEWPORT vp;
	vp.Width = (float)winW;	//��
	vp.Height = (float)winH;//����
	vp.MinDepth = 0.0f;	//��O
	vp.MaxDepth = 1.0f;	//��
	vp.TopLeftX = 0;	//��
	vp.TopLeftY = 0;	//��

	//�f�[�^����ʂɕ`�悷�邽�߂̈�ʂ�̐ݒ�i�p�C�v���C���j
	
	//TOPOLOGY���̔z�u�̂��� TRIANGLELIST �g���C�A���O�����X�g �� �X�g���b�v
	pContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // �f�[�^�̓��͎�ނ��w��
	pContext_->OMSetRenderTargets(1, &pRenderTargetView_, nullptr);            // �`����ݒ�
	pContext_->RSSetViewports(1, &vp);

	//�V�F�[�_�[����
	hr = InitShader();
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�V�F�[�_�[�̏����Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

//�V�F�[�_�[����
HRESULT Direct3D::InitShader()
{
	HRESULT hr;

	//���_�V�F�[�_�̍쐬�i�R���p�C���j
	/*
		ID3DBlob* pCompileVS = nullptr;							//�o�[�W�����̎w�� �T_�O
	hr = D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
	assert(pCompileVS != nullptr); ///////////

	hr = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "���_�V�F�[�_�[�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	*/

	//���_�C���v�b�g���C�A�E�g

	/*
	 D3D11_INPUT_ELEMENT_DESC layout[] = {
		//�ʒu�f�[�^����
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 }, //UV���W
		{ "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(DirectX::XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },//�@��
	};
	hr = pDevice_->CreateInputLayout(layout, 3, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout_);

	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "���_�C���v�b�g���C�A�E�g�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	SAFE_RELEASE(pCompileVS)


	*/

	// �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j

	/*
		ID3DBlob* pCompilePS = nullptr;
	hr = D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�s�N�Z���V�F�[�_�[�̃R���p�C���Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	assert(pCompilePS != nullptr);//////////

	hr = pDevice_->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader_);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�s�N�Z���V�F�[�_�[�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	SAFE_RELEASE(pCompilePS);


	*/

	//���X�^���C�U�쐬
	/*
		D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK; //CULL
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE; //���v��肾���\�������
	hr = pDevice_->CreateRasterizerState(&rdc, &pRasterizerState_); //�N���G�C�g�I
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "���X�^���C�U�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
	*/
	


	//��������QD

	//���_�V�F�[�_�̍쐬�i�R���p�C���j
		ID3DBlob* pCompileVS = nullptr;							//�o�[�W�����̎w�� �T_�O
	hr = D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
	assert(pCompileVS != nullptr); ///////////

	hr = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "���_�V�F�[�_�[�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	//���_�C���v�b�g���C�A�E�g
	 std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		//�ʒu�f�[�^����
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 }, //UV���W
	};
	hr = pDevice_->CreateInputLayout(layout.data(), layout.size(), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout_);
//	hr = pDevice_->CreateInputLayout(layout, 2, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout_); vector�g����

	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "���_�C���v�b�g���C�A�E�g�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS)

	// �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	hr = D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�s�N�Z���V�F�[�_�[�̃R���p�C���Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	assert(pCompilePS != nullptr);//////////

	hr = pDevice_->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader_);
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "�s�N�Z���V�F�[�_�[�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}

	SAFE_RELEASE(pCompilePS);


	//���X�^���C�U�쐬
		D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK; //CULL
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE; //���v��肾���\�������
	hr = pDevice_->CreateRasterizerState(&rdc, &pRasterizerState_); //�N���G�C�g�I
	if (FAILED(hr)) {
		//�G���[����
		MessageBox(nullptr, "���X�^���C�U�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}






	//���ꂼ����f�o�C�X�R���e�L�X�g�ɃZ�b�g
	pContext_->VSSetShader(pVertexShader, NULL, 0);	//���_�V�F�[�_�[
	pContext_->PSSetShader(pPixelShader_, NULL, 0);	//�s�N�Z���V�F�[�_�[
	pContext_->IASetInputLayout(pVertexLayout_);	//���_�C���v�b�g���C�A�E�g
	pContext_->RSSetState(pRasterizerState_);		//���X�^���C�U�[

	return S_OK;
}

//�`��J�n
void Direct3D::BeginDraw()

{
	//�w�i�̐F
	float clearColor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };//R,G,B,A

	//��ʂ��N���A
	pContext_->ClearRenderTargetView(pRenderTargetView_, clearColor);

}

//�`��I��
void Direct3D::EndDraw()

{
	//�����p�ƕ`�悷��p��p�ӂ��ď����I�������@�����p����`��փR�s�[���ĕ\�����Ă���
	//�X���b�v�i�o�b�N�o�b�t�@��\�ɕ\������j
	pSwapChain_->Present(0, 0);
}

//�������
void Direct3D::Release()
{
	///create �� Relese
	///������� ��{�I�ɍ��������delete����@����͂��ł�����

	SAFE_RELEASE(pRenderTargetView_);
	SAFE_RELEASE(pSwapChain_);
	SAFE_RELEASE(pContext_);
	SAFE_RELEASE(pDevice_);

	SAFE_RELEASE(pRasterizerState_);
	SAFE_RELEASE(pVertexLayout_);
	SAFE_RELEASE(pPixelShader_);
	SAFE_RELEASE(pVertexShader);
}