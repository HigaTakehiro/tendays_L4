#include "TitleScene.h"
#include <DxLib.h>

TitleScene::TitleScene(SceneManager* sceneMgr) :
	BaseScene(sceneMgr)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
}

int TitleScene::Update(const KeyInput& input)
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
			nextScene = Scene::StageSelect;
		}
	}

	return 0;
}

void TitleScene::Draw()
{
	DrawString(10, 10, "タイトル", GetColor(0, 0, 0));
}
