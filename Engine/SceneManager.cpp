#include "SceneManager.h"
#include "Model.h"

#include "../TestScene.h"
#include "../PlayScene.h"

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}

SceneManager::SceneManager(GameObject* parent)
	:GameObject(parent, "SceneManager")
{
}

void SceneManager::Initialize()
{
	currentSceneID_ = SCENE_ID_TEST;
	nextSceneID_ = currentSceneID_;
	Instantiate<TestScene>(this);

}

void SceneManager::Update()
{

	//�V�[����؂�ւ���
	//���݂̃V�[���ƁA�l�N�X�g�V�[�����ʂ�������؂�ւ�
	if (currentSceneID_ != nextSceneID_) {
		
		//���̃V�[���̃I�u�W�F�N�g��S�폜
		auto scene = childList_.begin();
		(*scene)->ReleaseSub();
		SAFE_DELETE(*scene);
		childList_.clear();

		//���[�h�����f�[�^��S�폜
		Model::Release();

		//��t������
		switch (nextSceneID_) {
		case SCENE_ID_TEST:
			Instantiate<TestScene>(this);
			break;
		case SCENE_ID_PLAY:
			Instantiate<PlayScene>(this);
			break;
		}
		currentSceneID_ = nextSceneID_;
	}



}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

void SceneManager::ChanegeScene(SCENE_ID _next)
{
	nextSceneID_ = _next;
}
