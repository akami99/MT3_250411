#include <Novice.h>
#include "ApplicationConfig.h"
#include "EngineMath.h"
#include "EngineMathFunctions.h"
#include "MatrixGenerators.h"
#include "DebugRenderer.h"
#include "MyDebugCamera.h"
//#include <algorithm>


const char kWindowTitle[] = "LE2B_01_アカミネ_レン_MT3_";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	

	// カメラの設定
	MyDebugCamera debugCamera; // インスタンスを生成

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///



#ifdef _DEBUG
		// カメラの更新
		debugCamera.Update();

#endif // _DEBUG

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(debugCamera.GetViewProjectionMatrix(), debugCamera.GetViewportMatrix());




#ifdef _DEBUG
		// デバッグウィンドウ
		debugCamera.Draw();

#endif // _DEBUG

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
