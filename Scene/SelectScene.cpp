#include "SelectScene.h"
#include <DxLib.h>

SelectScene::SelectScene(SceneManager* sceneMgr) :
	BaseScene(sceneMgr)
{
}

SelectScene::~SelectScene()
{
}

void SelectScene::Init()
{
}

int SelectScene::Update(const KeyInput& input)
{
	if (isSceneDest)
	{
		if (changeAnimation.GetChange())
		{
			sceneMgr->SceneChange(nextScene, true);
			return 1;
		}
	}
	else
	{
		if (input.IsKeyTrigger(KEY_INPUT_RETURN))
		{
			isSceneDest = true;
			changeAnimation.Start();
			nextScene = Scene::Game;
		}
	}

	return 0;
}

void SelectScene::Draw()
{
	DrawString(10, 10, "ステージセレクト", GetColor(0, 0, 0));
}
