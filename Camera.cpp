#include "Camera.h"

namespace Camera {
	//�ϐ�
	XMVECTOR position_;	//�J�����̈ʒu�i���_�j
	XMVECTOR target_;	//����ʒu�i�œ_�j
	XMMATRIX viewMatrix_;	//�r���[�s��
	XMMATRIX projMatrix_;	//�v���W�F�N�V�����s��
}

//������
void Camera::Initialize()
{
	/*
	XMVECTOR position = { 0, 3, -10, 0 };	//�J�����̈ʒu
	XMVECTOR target = { 0, 0, 0, 0 };	//�J�����̏œ_

	�r���[�s�� XMVectorSet=��͂ǂ�����	
	XMMATRIX view = XMMatrixLookAtLH(position, target, XMVectorSet(0, 1, 0, 0));
	
	�ˉe�s�� ��p(���W�A��)�E�A�X�y�N�g��E�N���b�s���O�i���Q
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);
	
	Fov�͎���p�̂��ƁiField of View�j�����XM_PIDIV4 1��/4 ������S�T�x �����قǂ߂���Y�[��(���ߊ����Ȃ��Ȃ�
	�N���b�s���O�̓�̒l�̍��͂Ȃ�ׂ��������@���̃T�C�Y�����k���邩��@�傫�������Z�t�@�C�e�B���O���N����
	*/

	position_ = XMVectorSet(0, 3, -10, 0);	//�J�����̈ʒu
	target_ = XMVectorSet(0, 0, 0, 0);	//�J�����̏œ_

	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)800 / (FLOAT)600, 0.1f, 100.0f);
}

//�X�V
void Camera::Update()
{
	//�r���[�s��̍쐬
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));
}

//�ʒu��ݒ�
void Camera::SetPosition(XMVECTOR position)
{
	position_ = position;
}

void Camera::SetPosition(XMFLOAT3 position)
{
	//float3�����̃Z�b�g�֐����ĂԂƏ��Vector�^�ŃZ�b�g�����
	//���������������ē������Ƃ���Ƃ��ɕ֗�

	SetPosition(XMLoadFloat3(&position));
}

//�œ_��ݒ�
void Camera::SetTarget(XMVECTOR target)
{
	target_ = target;
}

void Camera::SetTarget(XMFLOAT3 target)
{
	SetTarget(XMLoadFloat3(&target));
}

//�r���[�s����擾
XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

//�v���W�F�N�V�����s����擾
XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}