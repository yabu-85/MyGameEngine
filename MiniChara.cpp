#include "MiniChara.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"

MiniChara::MiniChara(GameObject* parent):
	GameObject(parent, "MiniChara"), pFbx_(nullptr)
{
}

MiniChara::~MiniChara()
{
}

void MiniChara::Initialize()
{
	pFbx_ = new Fbx;
	pFbx_->Load("Assets/oden.fbx");

	transform_.scale_.x = 0.2f;
	transform_.scale_.y = 0.2f;
	transform_.scale_.z = 0.2f;

	transform_.position_.x = 2.0f;
	transform_.position_.y = 1.0f;

}

void MiniChara::Update()
{

	if (Input::IsKey(DIK_F)) KillMe();
	if (Input::IsKey(DIK_W)) transform_.position_.x += 0.1f;
	if (Input::IsKey(DIK_S)) transform_.position_.x -= 0.1f;
}

void MiniChara::Draw()
{
	pFbx_->Draw(transform_);
}

void MiniChara::Release()
{
}