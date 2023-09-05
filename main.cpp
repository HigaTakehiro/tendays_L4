#include <DxLib.h>
#include "./Input/KeyInput.h"
#include "./Othello/Othello.h"

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "title";

// ウィンドウ横幅
const int WIN_WIDTH = 1360;

// ウィンドウ縦幅
const int WIN_HEIGHT = 765;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0xFF);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	Othello othello;
	othello.Init();
	int x = 0;
	int y = 0;
	Color colorFlag = Color::BLACK;

	KeyInput key;
	bool isMove = false;

	while (1)
	{
		// 画面クリア
		ClearDrawScreen();

		// 更新処理
		key.Update();
		isMove = false;

		if (key.IsKeyTrigger(KEY_INPUT_LEFT))
		{
			isMove = x - 1 >= 0;
			isMove &= othello.GetCell(static_cast<size_t>(y * othello.GetWidth() + x - 1)) != Color::HOLE;

			if (isMove)
			{
				x -= 1;
			}
		}
		if (key.IsKeyTrigger(KEY_INPUT_RIGHT))
		{
			isMove = x + 1 < othello.GetWidth();
			isMove &= othello.GetCell(static_cast<size_t>(y * othello.GetWidth() + x + 1)) != Color::HOLE;

			if (isMove)
			{
				x += 1;
			}
		}
		if (key.IsKeyTrigger(KEY_INPUT_UP))
		{
			isMove = y - 1 >= 0;
			isMove &= othello.GetCell(static_cast<size_t>((y - 1) * othello.GetWidth() + x)) != Color::HOLE;

			if (isMove)
			{
				y -= 1;
			}
		}
		if (key.IsKeyTrigger(KEY_INPUT_DOWN))
		{
			isMove = y + 1 < othello.GetHeight();
			isMove &= othello.GetCell(static_cast<size_t>((y + 1) * othello.GetWidth() + x)) != Color::HOLE;

			if (isMove)
			{
				y += 1;
			}
		}
		if (key.IsKeyTrigger(KEY_INPUT_RETURN))
		{
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
		}

		// 描画処理
		int offsetX = 5;
		int offsetY = 5;

		othello.Draw(offsetX, offsetY);
		DrawBox(x * Othello::circleSize + offsetX, y * Othello::circleSize + offsetY,
				(x + 1) * Othello::circleSize + offsetX, (y + 1) * Othello::circleSize + offsetY, GetColor(0, 0, 0), false);

		unsigned int color = 0;
		if (colorFlag == Color::BLACK)
		{
			color = GetColor(0x00, 0x00, 0x00);
		}
		else if (colorFlag == Color::WHITE)
		{
			color = GetColor(0xFF, 0xFF, 0xFF);
		}
		DrawCircle(800, Othello::circleSize, Othello::circleSize / 2, color);

		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1)
		{
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == TRUE)
		{
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	return 0;
}