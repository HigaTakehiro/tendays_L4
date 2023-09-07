#include "GameScene.h"
#include <DxLib.h>

GameScene::GameScene(SceneManager* sceneMgr) :
	BaseScene(sceneMgr),
	x(0),
	y(0),
	colorFlag(Color::BLACK),
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
	othello.Load("./Resources/StageData/stage01_HIGA.csv");
	colorFlag = othello.GetStartColor();
}

int GameScene::Update(const KeyInput& input)
{
	bool isMove = false;

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
		if (input.IsKeyTrigger(KEY_INPUT_R))
		{
			// リセット
			othello.Reset();
			colorFlag = Color::BLACK;
			isTie = false;
		}
		if (input.IsKeyTrigger(KEY_INPUT_LEFT))
		{
			isMove = (x - 1) >= 0;
			isMove &= othello.GetCell(static_cast<size_t>(y * othello.GetWidth() + x - 1)) != Color::HOLE;

			if (isMove)
			{
				x -= 1;
			}
		}
		if (input.IsKeyTrigger(KEY_INPUT_RIGHT))
		{
			isMove = (x + 1) < othello.GetWidth();
			isMove &= othello.GetCell(static_cast<size_t>(y * othello.GetWidth() + x + 1)) != Color::HOLE;

			if (isMove)
			{
				x += 1;
			}
		}
		if (input.IsKeyTrigger(KEY_INPUT_UP))
		{
			isMove = (y - 1) >= 0;
			isMove &= othello.GetCell(static_cast<size_t>((y - 1) * othello.GetWidth() + x)) != Color::HOLE;

			if (isMove)
			{
				y -= 1;
			}
		}
		if (input.IsKeyTrigger(KEY_INPUT_DOWN))
		{
			isMove = (y + 1) < othello.GetHeight();
			isMove &= othello.GetCell(static_cast<size_t>((y + 1) * othello.GetWidth() + x)) != Color::HOLE;

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

					if (colorFlag == Color::BLACK)
					{
						colorFlag = Color::WHITE;
					}
					else if (colorFlag == Color::WHITE)
					{
						colorFlag = Color::BLACK;
					}
				}
			}
			else
			{

				isSkip = false;

				if (othello.Put(x, y, colorFlag) != 0)
				{
					if (colorFlag == Color::BLACK)
					{
						colorFlag = Color::WHITE;
					}
					else if (colorFlag == Color::WHITE)
					{
						colorFlag = Color::BLACK;
					}
				}

				bCount = 0;
				wCount = 0;

				for (size_t i = 0; i < othello.GetSize(); i++)
				{
					if (othello.GetCell(i) == Color::BLACK)
					{
						bCount++;
					}
					else if (othello.GetCell(i) == Color::WHITE)
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

	if (colorFlag == Color::BLACK)
	{
		color = GetColor(0x00, 0x00, 0x00);
	}
	else if (colorFlag == Color::WHITE)
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
}
