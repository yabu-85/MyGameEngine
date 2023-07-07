#include "Player.h"
#include "Engine/Fbx.h"

Player::Player(GameObject* parent): //�I�u�W�F�N�g�̓o�^��parent�ԍ��Ɩ��O�I
	GameObject(parent, "Player"), pFbx(nullptr)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	pFbx = new Fbx;
	pFbx->Load("Assets/oden.fbx");
	this->transform_.scale_.x = 3.0f;
}

void Player::Update()
{
	transform_.rotate_.y += 1.0f;
}

void Player::Draw()
{
	pFbx->Draw(this->transform_);
}

void Player::Release()
{
	pFbx->Release();
	delete pFbx;
}
