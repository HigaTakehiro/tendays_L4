#include "BaseScene.h"

SceneChangeAnimation BaseScene::changeAnimation{};

BaseScene::BaseScene(SceneManager* sceneMgr) :
	sceneMgr(sceneMgr),
	isSceneDest(false),
	nextScene(Scene::Title)
{
	changeAnimation.Init();
}

BaseScene::~BaseScene()
{
	sceneMgr = nullptr;
}
