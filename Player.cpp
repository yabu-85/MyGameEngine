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

	Instantiate<MiniChara>(this);
	GameObject* pco2 = Instantiate<MiniChara>(this);
	pco2->SetTransformPos(XMFLOAT3(-2.0, 1.0f, 0));

	pco2 = nullptr;
	delete pco2;
}

void Player::Update()
{
	transform_.rotate_.y++;

	if (Input::IsKey(DIK_SPACE)) KillMe();
	if (Input::IsKey(DIK_A)) transform_.position_.x += 0.1f;
	if (Input::IsKey(DIK_D)) transform_.position_.x -= 0.1f;
}

void Player::Draw()
{
	pFbx->Draw(this->transform_);
}

void Player::Release()
{
}
