#include "Player.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"
#include <list>
#include <vector>

Player::Player(GameObject* parent): //オブジェクトの登録はparent番号と名前！
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
	this->transform_.scale_.x = 0.5f;

	Instantiate<MiniChara>(this);

	std::list<std::pair<int, string>> a = {
		{1,"stringA"},
		{2,"stringB"},
	};

	std::vector<float> vec = { 1,2,3,4,5 };
}

void Player::Update()
{
	transform_.rotate_.y++;

	if (Input::IsKey(DIK_SPACE)) KillMe();
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
