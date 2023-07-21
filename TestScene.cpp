#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"

TestScene::TestScene(GameObject* parent)
	:GameObject(parent, "TestSecene")
{
}

void TestScene::Initialize()
{
	
}

void TestScene::Update()
{
	if (Input::IsKey(DIK_W)) {
		//�����SceneManager�����Ċm�肵�Ă��邩�炢�����ǃ_�E���L���X�g�͂��Ȃ��悤�ɂ��悤
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChanegeScene(SCENE_ID_PLAY);

	}
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
