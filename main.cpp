#include <DxLib.h>
#include "./Scene/SceneManager.h"
#include "./Input/KeyInput.h"
#include "./Othello/Othello.h"

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "title";

// ウィンドウ横幅
const int WIN_WIDTH = 960;

// ウィンドウ縦幅
const int WIN_HEIGHT = 740;

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

	SceneManager sceneMgr;

	KeyInput key;

	while (1)
	{
		// 更新処理
		key.Update();
		sceneMgr.Loop(key);

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