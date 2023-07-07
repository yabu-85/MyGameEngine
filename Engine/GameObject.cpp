#include "GameObject.h"

GameObject::GameObject():
	pParent_(nullptr)
{
}

GameObject::GameObject(GameObject* parent, const std::string& name):
	pParent_(parent), objectName_(name)
{
	childList_.clear();
	transform_ = parent->transform_;

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
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
		(*itr)->UpdateSub();
}
