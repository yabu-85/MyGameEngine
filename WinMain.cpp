//�C���N���[�h
#include <Windows.h>
#include "Direct3D.h"
#include "Quad.h"
#include "Dice.h"
#include "Camera.h"

//�萔�錾
const char* WIN_CLASS_NAME = "SampleGame";
const char* WIN_CLASS_TITLEBAR = "�T���v���Q�[��";
const int WINDOW_WIDTH = 800;  //�E�B���h�E�̕�
const int WINDOW_HEIGHT = 600; //�E�B���h�E�̍���

//�v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//�G���g���[�|�C���g Handl Instance
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) //APIENTRY main �ɂ����݂�����
{
	//�E�B���h�E�N���X�i�݌v�}�j���쐬
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);             //���̍\���̂̃T�C�Y
	wc.hInstance = hInstance;                   //�C���X�^���X�n���h��
	wc.lpszClassName = WIN_CLASS_NAME;            //�E�B���h�E�N���X��
	wc.lpfnWndProc = WndProc;                   //�E�B���h�E�v���V�[�W��
	wc.style = CS_VREDRAW | CS_HREDRAW;         //�X�^�C���i�f�t�H���g�j
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //�A�C�R��
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //�������A�C�R��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //�}�E�X�J�[�\��
	wc.lpszMenuName = NULL;                     //���j���[�i�Ȃ��j
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //�w�i�i���j

	RegisterClassEx(&wc);  //�N���X��o�^ �݌v�}������ă��W�ɓ����


	//�E�B���h�E�T�C�Y�̌v�Z
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE); //������false �̓��j���[������Ȃ������߂�
	int winW = winRect.right - winRect.left;     //�E�B���h�E��
	int winH = winRect.bottom - winRect.top;     //�E�B���h�E����


	//�E�B���h�E���쐬
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //�E�B���h�E�N���X��
		WIN_CLASS_TITLEBAR,     //�^�C�g���o�[�ɕ\��������e
		WS_OVERLAPPEDWINDOW, //�X�^�C���i���ʂ̃E�B���h�E�j
		CW_USEDEFAULT,       //�\���ʒu���i���܂����j
		CW_USEDEFAULT,       //�\���ʒu��i���܂����j
		WINDOW_WIDTH,                 //�E�B���h�E��
		WINDOW_HEIGHT,                 //�E�B���h�E����
		NULL,                //�e�E�C���h�E�i�Ȃ��j
		NULL,                //���j���[�i�Ȃ��j
		hInstance,           //�C���X�^���X
		NULL                 //�p�����[�^�i�Ȃ��j
	);
	// | WS_VISIBLE  <--�������̂ł��\���ł��� �T���v���Q�[���̉�
	//WS_OVERLAPPEDWINDOW | WS_VISIBLE,

	//�E�B���h�E��\��
	ShowWindow(hWnd, nCmdShow); //������E�B���h�E��������悤�ɂ���

	//Direct3D������
	HRESULT hr;
	hr = Direct3D::Initialize(winW, winH, hWnd);
	if (FAILED(hr)) {
		PostQuitMessage(0);  //�v���O�����I��
	}

	Camera::Initialize();
	Camera::SetTarget(XMFLOAT3(0, 0, 0));

#if 0
	Quad* q = new Quad;
	
	hr = q->InitializeVertex();
	if (FAILED(hr)) PostQuitMessage(0); 

	hr = q->InitializeIndex();
	if (FAILED(hr)) PostQuitMessage(0);

	hr = q->InitializeConstantBuffer();
	if (FAILED(hr)) PostQuitMessage(0);

#else
	Dice* d = new Dice;

	hr = d->InitializeVertex();
	if (FAILED(hr)) PostQuitMessage(0);

	hr = d->InitializeIndex();
	if (FAILED(hr)) PostQuitMessage(0);

	hr = d->InitializeConstantBuffer();
	if (FAILED(hr)) PostQuitMessage(0);

#endif

	//���b�Z�[�W���[�v�i�����N����̂�҂j
	MSG msg; //MSG�����b�Z�[�W����邽�߂̌^
	ZeroMemory(&msg, sizeof(msg)); //�z�񂷂ׂĂɂO��������
	while (msg.message != WM_QUIT) //�I������������܂�
	{
		//���b�Z�[�W����  ���b�Z�[�W���D��@���b�Z���Ȃ���΃Q�[���̏���
		//����PeekMessage �֐���define ����Ă� 
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) //PeekMessage �����Ƀ��b�Z�����܂��Ă���
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//���b�Z�[�W�Ȃ�
		else
		{
			Camera::Update();

			//�Q�[���̏���
			Direct3D::BeginDraw();

#if 0
			XMMATRIX matT = XMMatrixTranslation(0, 0, 0);

			q->Draw(matT);
			q->DrawIndex(matT);

#else
			static float a = 0;
			a += 0.04f;

			XMMATRIX matR1 = XMMatrixRotationY(XMConvertToRadians(a)); //n���ɉ��x��]���郉�W�A���̒l���o��
			XMMATRIX matR2 = XMMatrixRotationX(XMConvertToRadians(a + a)); //n���ɉ��x��]���郉�W�A���̒l���o��
			XMMATRIX matT = XMMatrixTranslation(0, 0, 0);
			XMMATRIX matS = XMMatrixScaling(1, 1, 1);

			//R * T���Ƃ܂���]����
			XMMATRIX mat = matR1 * matR2 * matT * matS;

			d->Draw(mat); //�Q�ƂŎ󂯎���Ă邩��v�Z����n���Ȃ��H
			d->DrawIndex(mat);

#endif

			//�`�揈��
			Direct3D::EndDraw();

		}
	}

	Direct3D::Release();

#if 0
	SAFE_DELETE(q);
#else
	SAFE_DELETE(d);
#endif

	return 0;
}

//�������������ꂽ�玩���I�ɌĂ΂��֐����R�[���o�b�N                       �����̎��ʔԍ�
//�N���b�N�Ɖ����ꂽ��J���Ă邷�ׂẴE�B���h�E�ŃE�B���h�E�v���V�[�W����ǂ�� hWnd ��n��
//UINT �ɃJ�[�\�����킹��Ƃ킩�邯�� unsigned int �̗��� typedef ���g���Ɩ����ł���
//W, L PARAM paramerter �ڍ׏���ǉ����Ȃǂ������Ă���

//�E�B���h�E�v���V�[�W���i�������������ɂ�΂��֐��@���N���b�N�Ƃ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY: //WM_DESTROY = �E�B���h�E������ꂽ  �ϐ��E�N�����錾�ֈړ��i�S��������) �{���͑S���̂�����Ȃ��Ƃ����Ȃ����ǂ߂�ǂ����牺default��
		PostQuitMessage(0);  //�v���O�����I��
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam); //�Ȃ��case �ɂ�������Ȃ������� Windows��default�̋@�\������悤�ɂ��Ă���
}