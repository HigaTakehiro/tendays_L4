#include "GameScene.h"
#include <DxLib.h>

GameScene::GameScene(SceneManager* sceneMgr) :
	BaseScene(sceneMgr),
	x(0),
	y(0),
	colorFlag(ColorFlag::BLACK),
	bCount(0),
	wCount(0),
	isTie(false),
	isSkip(false),
	isFin(false)
{
	othello.Init();
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	othello.Load("./Resources/StageData/Test.csv");
	colorFlag = othello.GetStartColor();

	bCount = 0;
	wCount = 0;

	for (int i = 0; i < othello.GetSize(); i++)
	{
		if (othello.GetCell(i) == ColorFlag::BLACK || othello.GetCell(i) == ColorFlag::BIG_B)
		{
			bCount++;
		}
		else if (othello.GetCell(i) == ColorFlag::WHITE || othello.GetCell(i) == ColorFlag::BIG_W)
		{
			wCount++;
		}
	}
}

int GameScene::Update(const KeyInput& input)
{
	bool isMove = false;

	if (input.IsKeyTrigger(KEY_INPUT_R))
	{
		// リセット
		othello.Reset();
		colorFlag = ColorFlag::BLACK;
		isTie = false;
	}
	if (isFin)
	{
		isTie = (bCount == wCount);

		if (input.IsKeyTrigger(KEY_INPUT_RETURN))
		{
			sceneMgr->SceneChange(Scene::Title, true);
			return 1;
		}
	}
	else
	{
		if (input.IsKeyTrigger(KEY_INPUT_T))
		{
			othello.IsSkip(ColorFlag::WHITE);
		}
		if (input.IsKeyTrigger(KEY_INPUT_LEFT))
		{
			isMove = (x - 1) >= 0;
			isMove &= othello.GetCell(static_cast<size_t>(y * othello.GetWidth() + x - 1)) != ColorFlag::HOLE;

			if (isMove)
			{
				x -= 1;
			}
		}
		if (input.IsKeyTrigger(KEY_INPUT_RIGHT))
		{
			isMove = (x + 1) < othello.GetWidth();
			isMove &= othello.GetCell(static_cast<size_t>(y * othello.GetWidth() + x + 1)) != ColorFlag::HOLE;

			if (isMove)
			{
				x += 1;
			}
		}
		if (input.IsKeyTrigger(KEY_INPUT_UP))
		{
			isMove = (y - 1) >= 0;
			isMove &= othello.GetCell(static_cast<size_t>((y - 1) * othello.GetWidth() + x)) != ColorFlag::HOLE;

			if (isMove)
			{
				y -= 1;
			}
		}
		if (input.IsKeyTrigger(KEY_INPUT_DOWN))
		{
			isMove = (y + 1) < othello.GetHeight();
			isMove &= othello.GetCell(static_cast<size_t>((y + 1) * othello.GetWidth() + x)) != ColorFlag::HOLE;

			if (isMove)
			{
				y += 1;
			}
		}
		if (input.IsKeyTrigger(KEY_INPUT_RETURN))
		{
			if (othello.IsSkip(colorFlag))
			{
				if (isSkip)
				{
					isFin = true;
				}
				else
				{
					isSkip = true;

					if (colorFlag == ColorFlag::BLACK)
					{
						colorFlag = ColorFlag::WHITE;
					}
					else if (colorFlag == ColorFlag::WHITE)
					{
						colorFlag = ColorFlag::BLACK;
					}
				}
			}
			else
			{

				isSkip = false;

				if (othello.Put(x, y, colorFlag) != 0)
				{
					if (colorFlag == ColorFlag::BLACK)
					{
						colorFlag = ColorFlag::WHITE;
					}
					else if (colorFlag == ColorFlag::WHITE)
					{
						colorFlag = ColorFlag::BLACK;
					}
				}

				bCount = 0;
				wCount = 0;

				for (int i = 0; i < othello.GetSize(); i++)
				{
					if (othello.GetCell(i) == ColorFlag::BLACK || othello.GetCell(i) == ColorFlag::BIG_B)
					{
						bCount++;
					}
					else if (othello.GetCell(i) == ColorFlag::WHITE || othello.GetCell(i) == ColorFlag::BIG_W)
					{
						wCount++;
					}
				}
			}
		}
	}

	return 0;
}

void GameScene::Draw()
{
	int offsetX = 10;
	int offsetY = 10;
	unsigned int color = 0;

	if (colorFlag == ColorFlag::BLACK)
	{
		color = GetColor(0x00, 0x00, 0x00);
	}
	else if (colorFlag == ColorFlag::WHITE)
	{
		color = GetColor(0xFF, 0xFF, 0xFF);
	}

	othello.Draw(offsetX, offsetY);
	DrawBox(x * Othello::circleSize + 5 + offsetX, y * Othello::circleSize + 5 + offsetY,
			(x + 1) * Othello::circleSize - 5 + offsetX, (y + 1) * Othello::circleSize - 5 + offsetY, GetColor(0, 0, 0), false);

	if (isTie)
	{
		DrawString(800 + offsetX, Othello::circleSize, "引き分け", GetColor(0xFF, 0xFF, 0xFF));
	}
	else if (isFin)
	{
		DrawString(800 + offsetX, Othello::circleSize, "置ける場所が無い", GetColor(0xFF, 0xFF, 0xFF));
	}
	else
	{
		DrawCircle(800 + offsetX, Othello::circleSize, Othello::circleSize / 2, color);
	}

	DrawFormatString(800 + offsetX, 200, GetColor(0xFF, 0xFF, 0xFF), "黒:%d", bCount);
	DrawFormatString(800 + offsetX, 220, GetColor(0xFF, 0xFF, 0xFF), "白:%d", wCount);
}
