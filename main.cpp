#include <Novice.h>
#include "ApplicationConfig.h"
#include "EngineMath.h"
#include "EngineMathFunctions.h"
#include "MatrixGenerators.h"
#include "DebugRenderer.h"
#include "MyDebugCamera.h"
#include <cmath>
#include <numbers>
//#include <algorithm>


const char kWindowTitle[] = "LE2B_01_アカミネ_レン_MT3_04-01";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	float PI = std::numbers::pi_v<float>; // πの値を取得

	// 球のパラメータ
	Sphere sphere; // 球のインスタンス
	sphere.center = { 0.8f, 0.0f, 0.0f }; // 球の中心座標
	sphere.radius = 0.08f; // 球の半径
	uint32_t color = WHITE; // 球の色

	// 円のパラメータ
	Vector3 center = {0.0f, 0.0f, 0.0f}; // 中心座標
	float radius = 0.8f; // 半径
	// 角速度（ラジアン/秒）
	float angularVelocity = PI; // 1秒あたりの角度変化量（ラジアン）
	float angle = 0.0f; // 初期角度（円の中心に向かう方向）
	
	// 回転するかどうかのフラグ
	bool isRotating = false; // 初期状態では回転しない


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

		if (keys[DIK_SPACE] != 0 && preKeys[DIK_SPACE] == 0) { // スペースキーが押されたら
			isRotating = !isRotating; // 回転フラグをトグル
		}

		if (isRotating) { // 回転フラグが立っている場合
			// 角度を更新
			angle += angularVelocity * kDeltaTime; // 角度を更新
			if (angle >= 2.0f * PI) { // 2πを超えたらリセット
				angle -= 2.0f * PI;
			}
		} else {
			// 回転しない場合は角度をリセット
			angle = 0.0f;
		}
		
		// 円運動の中心からの相対位置を計算
		float x = radius * cosf(angle);
		float y = radius * sinf(angle); // 通常、Y軸を上下方向と考える場合、円運動はXZ平面で行われます

		// 球の中心座標を更新
		// 円の中心座標に相対位置を加算
		sphere.center.x = center.x + x;
		sphere.center.y = center.y + y;
		sphere.center.z = center.z;

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

		// デバッグ用の球を描画
		DrawSphere(
			sphere,
			debugCamera.GetViewProjectionMatrix(),
			debugCamera.GetViewportMatrix(),
			color); // 球の中心と半径、色を指定して描画



#ifdef _DEBUG
		// デバッグウィンドウ
		debugCamera.Draw();

		// 円運動開始/停止のトグル
		ImGui::Begin("Circle Motion Control");
		if (ImGui::Button("Start", ImVec2(50, 20))) {
			isRotating = !isRotating;
		}
		ImGui::End();

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
