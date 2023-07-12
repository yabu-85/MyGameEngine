#include "GameObject.h"

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}

GameObject::GameObject():
	pParent_(nullptr), objectName_(""), dead_(false)
{
}

GameObject::GameObject(GameObject* parent, const std::string& name):
	pParent_(parent), objectName_(name), dead_(false)
{
	childList_.clear();
	transform_ = parent->transform_;

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