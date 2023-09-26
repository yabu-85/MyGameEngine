#include "TestScene.h"
#include "Stage.h"
#include "Controller.h"
#include "Player.h"

TestScene::TestScene(GameObject* parent)
	:GameObject(parent, "TestSecene")
{
}

void TestScene::Initialize()
{
	Instantiate<Stage>(this);
	Instantiate<Controller>(this);
	Instantiate<Player>(this);

}

void TestScene::Update()
{

}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
