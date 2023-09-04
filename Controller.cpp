#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Fbx.h"

Controller::Controller(GameObject* parent)
	:GameObject(parent, "Controller")
{
}

Controller::~Controller()
{
}

void Controller::Initialize()
{
	transform_.position_.x = 0.5f;
	transform_.position_.z = 0.0f;
	transform_.rotate_.x = 0.0f;
}

void Controller::Update()
{
	if (Input::IsKey(DIK_LEFTARROW))
	{
		transform_.rotate_.y -= 1.0f;
	}
	if (Input::IsKey(DIK_RIGHTARROW))
	{
		transform_.rotate_.y += 1.0f;
	}
	if (Input::IsKey(DIK_UPARROW))
	{
		transform_.rotate_.x += 1.0f;
		if (transform_.rotate_.x > 89.0f) transform_.rotate_.x = 89.0f;
	}
	if (Input::IsKey(DIK_DOWNARROW))
	{
		transform_.rotate_.x -= 1.0f;
		if (transform_.rotate_.x < -89.0f) transform_.rotate_.x = -89.0f;
	}

	//�x�N�g���^���Ƃ��Ȃ�ł��������Ȃ邩�A�^�������������ς���ꏊ�ł��̂����Ōv�Z�����������Ȃ�
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vCam = { 0, 0, -10, 0 };
	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.1, 0.0f);
	XMMATRIX mRotateY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	XMMATRIX mRotateX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
	XMMATRIX mRotate = mRotateX * mRotateY;
	vMove = XMVector3TransformCoord(vMove, mRotate);
	//�㉺�̈ړ����������Ȃ�����XMFLOAT3�ɂ��Ă����O�ɂ��Ă���Normalize���đ�����Ă�
	XMFLOAT3 fMove;
	XMStoreFloat3(&fMove, vMove);
	fMove.y = 0;
	vMove = XMLoadFloat3(&fMove);
	vMove = XMVector3Normalize(vMove) * 0.1f;

	if (Input::IsKey(DIK_W))
	{
		vPos += vMove;
	}
	if (Input::IsKey(DIK_S))
	{
		vPos -= vMove;
	}
	if (Input::IsKey(DIK_A))
	{
		vPos += XMVector3Cross(vMove, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	}
	if (Input::IsKey(DIK_D))
	{
		vPos -= XMVector3Cross(vMove, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	}
	//�����ړ����Ă�
	XMStoreFloat3(&transform_.position_, vPos);

	//�㉺�̈ړ�
	if (Input::IsKey(DIK_F))
	{
		transform_.position_.y += 0.2f;
	}
	if (Input::IsKey(DIK_E))
	{
		transform_.position_.y -= 0.2f;
	}

	vCam = XMVector3Transform(vCam, mRotate);
	XMFLOAT3 cameraPos;
	XMStoreFloat3(&cameraPos, vPos + vCam);

	//RayCast
	if (Input::IsKey(DIK_SPACE)) {
		RayCastData data;
		data.start = XMFLOAT3(0, 10, 0);   //���C�̔��ˈʒu
		data.dir = XMFLOAT3(0,-1,0);       //���C�̕���

		RayCastData a;
		Fbx* pFbx = new Fbx;
		pFbx->Fbx::RayCast(&a);

		if (data.hit) transform_.position_.z += 0.1f;

	}

	Camera::SetPosition(cameraPos);
	Camera::SetTarget(transform_.position_);

}



void Controller::Draw()
{
}

void Controller::Release()
{
}
