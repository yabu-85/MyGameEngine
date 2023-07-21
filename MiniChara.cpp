#include "MiniChara.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

MiniChara::MiniChara(GameObject* parent):
	GameObject(parent, "MiniChara"), hModel_(-1)
{
}

MiniChara::~MiniChara()
{
}

void MiniChara::Initialize()
{
	hModel_ = Model::Load("Assets/oden.fbx");

	transform_.scale_.x = 0.2f;
	transform_.scale_.y = 0.2f;
	transform_.scale_.z = 0.2f;

	SphereCollider* col = new SphereCollider(1.0);
	AddCollider(col);
}

void MiniChara::Update()
{
	transform_.position_.z += 0.5f;
	if (transform_.position_.z > 30.0f) KillMe();
}

void MiniChara::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void MiniChara::Release()
{
}