#pragma once

#include <list>
#include "Transform.h"
#include <string>

using std::string;
using std::list;

class SphereCollider;

class GameObject
{
protected:
	list<GameObject *>		childList_;
	Transform				transform_;
	GameObject*				pParent_;
	string					objectName_;
	bool					dead_;
	SphereCollider*			pCollider_;

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
	void DrawSub();
	void UpdateSub();
	void ReleaseSub();

	void KillMe() { dead_ = true; };
	bool IsDead() { return (dead_ != false); };
	void SetTransform(Transform _t) { transform_ = _t; };
	void SetPosition(XMFLOAT3 _t) { transform_.position_ = _t; };
	XMFLOAT3 GetPosition() { return transform_.position_; };
	GameObject* FindChildObject(string _objName);
	GameObject* GetRootJob();
	GameObject* FindObject(string _objName);

	void AddCollider(SphereCollider* pCollider);
	void Collision(GameObject* pTarget);
	void RoundRobin(GameObject* pTarget);

	template <class T>
	T* Instantiate(GameObject* parent)
	{
		T* p;
		p = new T(parent);
		p->Initialize();
		childList_.push_back(p);
		return p;
	}
};