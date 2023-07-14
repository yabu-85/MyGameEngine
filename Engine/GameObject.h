#pragma once

#include <list>
#include "Transform.h"
#include <string>

using std::string;
using std::list;

class GameObject
{
protected:
	list<GameObject *>		childList_;
	Transform				transform_;
	GameObject*				pParent_;
	string					objectName_;
	bool					dead_;

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	void KillMe() { dead_ = true; };
	bool IsDead() { return (dead_ != false); };

	void SetTransform(Transform t) { transform_ = t; };
	void SetPosition(XMFLOAT3 t) { transform_.position_ = t; };

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;

	void DrawSub();
	void UpdateSub();
	void ReleaseSub();

	template <class T>
	T* Instantiate(GameObject* parent)
	{
		T* p;
		p = new T(parent);
		p->Initialize();
		parent->childList_.push_back(p);
		return p;
	}
};