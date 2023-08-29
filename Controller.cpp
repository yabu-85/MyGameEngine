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
	if (Input::IsKey(DIK_A))
	{
		transform_.rotate_.y -= 1.0f;

	}
	if (Input::IsKey(DIK_D))
	{
		transform_.rotate_.y += 1.0f;

	}

	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vMove = { 0.0f, 0.0f, 0.1f, 0.0f };
	CXMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	CXMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(30));
	vMove = XMVector3TransformCoord(vMove, mRotY);
	XMFLOAT3 fMove;
	XMStoreFloat3(&fMove, vMove);

	if (Input::IsKey(DIK_W))
	{
		vPos += vMove;
		XMStoreFloat3(&transform_.position_, vPos);
	}

	if (Input::IsKey(DIK_S))
	{
		vPos -= vMove;
		XMStoreFloat3(&transform_.position_, vPos);
	}

	XMVECTOR vCam = { 0, 5, -10, 0 };
	vCam = XMVector3TransformCoord(vCam, mRotY);
	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, vPos + vCam);
	Camera::SetPosition(camPos);

	XMFLOAT3 camTar = transform_.position_;
	camTar = { camTar.x + fMove.x, camTar.y, camTar.z + fMove.z };
	Camera::SetTarget(camTar);

}

void Controller::Draw()
{
}

void Controller::Release()
{
}
