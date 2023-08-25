#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"

Controller::Controller(GameObject* parent)
	:GameObject(parent, "Controller"), cameraPos_{0,0,0}, cameraTar_{0,0,0}
{
}

Controller::~Controller()
{
}

void Controller::Initialize()
{
	cameraTar_ = { 1.5f, 0.1f, 1.0f };
}

void Controller::Update()
{

	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vMove = { 0.0, 0.0, 0.0, 0.0 };
	bool flag = false;

	if (Input::IsKey(DIK_A))
	{
		vMove += { -0.1, 0.0, 0.0, 0.0 };
		flag = true;

	}
	if (Input::IsKey(DIK_D))
	{
		vMove += { 0.1, 0.0, 0.0, 0.0 };
		flag = true;

	}
	if (Input::IsKey(DIK_W))
	{
		vMove += { 0.0, 0.0, 0.1, 0.0 };
		flag = true;

	}
	if (Input::IsKey(DIK_S)) 
	{
		vMove += { 0.0, 0.0, -0.1, 0.0 };
		flag = true;

	}

	if (flag) {
		CXMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
		vMove = XMVector3TransformCoord(vMove, mRotY);

		vPos += vMove;
		XMStoreFloat3(&transform_.position_, vPos);
		XMStoreFloat3(&cameraTar_, vPos);

		Camera::SetTarget(cameraTar_);

	}
}

void Controller::Draw()
{
}

void Controller::Release()
{
}
