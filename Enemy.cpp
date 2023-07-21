#include "Enemy.h"
#include "Engine/Input.h"
#include "MiniChara.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

Enemy::Enemy(GameObject* parent) : //�I�u�W�F�N�g�̓o�^��parent�ԍ��Ɩ��O�I
	GameObject(parent, "Enemy"), hModel_(-1)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_ = Model::Load("Assets/oden.fbx");
	assert(hModel_ >= 0);

	transform_.position_.z = 10.0f;

	SphereCollider* col = new SphereCollider(3.0);
	AddCollider(col);
}

void Enemy::Update()
{

}

void Enemy::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}
