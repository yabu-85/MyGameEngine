#include "Player.h"
#include "Engine/Input.h"
#include "MiniChara.h"
#include "Engine/Model.h"

Player::Player(GameObject* parent): //オブジェクトの登録はparent番号と名前！
	GameObject(parent, "Player"), hModel_(-1)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Assets/oden.fbx");
	assert(hModel_ >= 0);

}

void Player::Update()
{
	transform_.rotate_.y++;
	
	if (Input::IsKeyUp(DIK_SPACE)) {
		GameObject* pBullet = Instantiate<MiniChara>(pParent_);
		pBullet->SetPosition(transform_.position_);
	}

	if (Input::IsKey(DIK_A)) transform_.position_.x -= 0.1f;
	if (Input::IsKey(DIK_D)) transform_.position_.x += 0.1f;
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
}
