#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"

Controller::Controller(GameObject* parent)
	:GameObject(parent, "Controller")
{
}

Controller::~Controller()
{
}

void Controller::Initialize()
{
	transform_.position_.x = 7.5f;
	transform_.position_.z = 5.0f;
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

	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vCam = { 0, 5, -10, 0 };
	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.1, 0.0f);
	CXMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	vMove = XMVector3TransformCoord(vMove, mRotY);
	XMFLOAT3 fMove;
	XMStoreFloat3(&fMove, vMove);
	XMVECTOR vCamTransformed = XMVector3TransformCoord(vCam, mRotY);

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

	XMStoreFloat3(&transform_.position_, vPos);

	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, vPos + vCamTransformed);
	Camera::SetPosition(camPos);

	XMFLOAT3 camTar = { transform_.position_.x + fMove.x, 0.0f, transform_.position_.z + fMove.z };
	Camera::SetTarget(camTar);
}



void Controller::Draw()
{
}

void Controller::Release()
{
}
