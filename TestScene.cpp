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
		//今回はSceneManagerだって確定しているからいいけどダウンキャストはしないようにしよう
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
