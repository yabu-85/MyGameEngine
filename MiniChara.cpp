#include "MiniChara.h"
#include "Engine/Fbx.h"

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
}

void MiniChara::Update()
{
}

void MiniChara::Draw()
{
	pFbx_->Draw(transform_);
}

void MiniChara::Release()
{
	pFbx_->Release();
	delete pFbx_;
}

