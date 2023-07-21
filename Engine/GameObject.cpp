#include "GameObject.h"
#include "SphereCollider.h"

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}

GameObject::GameObject():
	pParent_(nullptr), objectName_(""), dead_(false), pCollider_(nullptr)
{
}

GameObject::GameObject(GameObject* parent, const std::string& name):
	pParent_(parent), objectName_(name), dead_(false), pCollider_(nullptr)
{
	childList_.clear();

	if (pParent_ != nullptr)
		this->transform_.pParent_ = &(parent->transform_);

}

GameObject::~GameObject()
{
}

void GameObject::DrawSub()
{
	Draw();
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
		(*itr)->DrawSub();
}

void GameObject::UpdateSub()
{
	Update();

	RoundRobin(GetRootJob());

	for (auto it = childList_.begin(); it != childList_.end();)
	{
		if ((*it)->IsDead() == true)
		{
			(*it)->ReleaseSub();
			SAFE_DELETE(*it);
			it = childList_.erase(it);
		}
		else
		{
			it++;
		}
	}

	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
		(*itr)->UpdateSub();
}

void GameObject::ReleaseSub()
{
	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
		(*it)->ReleaseSub();
		SAFE_DELETE(*it);
	}

	Release();
}

GameObject* GameObject::FindChildObject(string _objName)
{
	if (_objName == this->objectName_) {
		return (this); //自分が_objNameだった
	}
	else {
		for (auto itr = childList_.begin(); itr != childList_.end(); itr++) {
			GameObject* obj = (*itr)->FindChildObject(_objName);
			if (obj != nullptr)
				return obj;
		}
	}
	return nullptr;
}

//RootJobにたどり着くまで再起呼び出しして探してる
GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr)
		return this;

	return pParent_->GetRootJob();
}

GameObject* GameObject::FindObject(string _objName)
{
	return GetRootJob()->FindChildObject(_objName);
}

void GameObject::AddCollider(SphereCollider* pCollider)
{
	pCollider_ = pCollider;
}

void GameObject::Collision(GameObject* pTarget)
{
	//ターゲットコライダーがアタッチされていない
	if (pTarget == this && pTarget->pCollider_ == nullptr)
		return;
	
	float dist = (transform_.position_.x - pTarget->transform_.position_.x) * (transform_.position_.x - pTarget->transform_.position_.x) +
		(transform_.position_.y - pTarget->transform_.position_.y) * (transform_.position_.y - pTarget->transform_.position_.y) +
		(transform_.position_.z - pTarget->transform_.position_.z) * (transform_.position_.z - pTarget->transform_.position_.z);

	float rDist = (this->pCollider_->GetRadius() + pCollider_->GetRadius()) * (this->pCollider_->GetRadius() + pCollider_->GetRadius());

	//当たった
	if (dist <= rDist) {
		//Oncollision
		int a = 0;
	}

}

void GameObject::RoundRobin(GameObject* pTarget)
{
 	if (pCollider_ == nullptr)
		return;

	//自分とターゲット
	if (pTarget->pCollider_ != nullptr)
		Collision(pTarget);

	for(auto itr = pTarget->childList_.begin(); itr != pTarget->childList_.end(); itr++)
		RoundRobin(*itr);

}
