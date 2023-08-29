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

	//ベクトル真下とかなんでおかしくなるか、真下が見る方向を変える場所でそのせいで計算がおかしくなる
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vCam = { 0, 0, -10, 0 };
	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.1, 0.0f);
	XMMATRIX mRotateY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	XMMATRIX mRotateX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
	XMMATRIX mRotate = mRotateX * mRotateY;
	vMove = XMVector3TransformCoord(vMove, mRotate);

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
	vCam = XMVector3Transform(vCam, mRotate);
	Camera::SetPosition(vPos + vCam);
	Camera::SetTarget(transform_.position_);

}



void Controller::Draw()
{
}

void Controller::Release()
{
}
