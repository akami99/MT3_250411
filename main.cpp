#include <Novice.h>
#include "ApplicationConfig.h"
#include "EngineMath.h"
#include "EngineMathFunctions.h"
#include "MatrixGenerators.h"
#include "DebugRenderer.h"
#include "MyDebugCamera.h"
#include <cmath> // std::sin, std::cos
//#include <algorithm>

// 追加の構造体
struct Pendulum {
	Vector3 anchor;                // アンカーポイント。固定された端の位置
	float length;                 // 紐の長さ
	float angle;                  // 現在の角度
	float angularVelocity;        // 角速度ω
	float angularAcceleration;    // 角加速度
};


const char kWindowTitle[] = "LE2B_01_アカミネ_レン_MT3_04-02";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);


	// 振り子の設定
	Pendulum pendulum;
	pendulum.anchor = { 0.0f, 1.0f, 0.0f };
	pendulum.length = 0.8f;
	pendulum.angle = 0.7f;
	pendulum.angularVelocity = 0.0f;
	pendulum.angularAcceleration = 0.0f;

	// 振り子の位置を格納する変数
	Vector3 pendulumPosition;
	pendulumPosition.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
	pendulumPosition.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
	pendulumPosition.z = pendulum.anchor.z;

	bool isSwing = false; // 振り子が揺れているかどうかのフラグ

	// カメラの設定
	MyDebugCamera debugCamera; // インスタンスを生成

	// スクリーン変換用の変数
	Vector3 screenAnchor = debugCamera.WorldToScreen(pendulum.anchor);
	Vector3 screenPosition = debugCamera.WorldToScreen(pendulumPosition);

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

		// 振り子の更新処理
		if (isSwing) {
			// 重力加速度の設定
			const float g = 9.8f;
			// 角加速度の計算
			pendulum.angularAcceleration = -(g / pendulum.length) * std::sin(pendulum.angle);
			// 角速度の更新
			pendulum.angularVelocity += pendulum.angularAcceleration * kDeltaTime;
			// 角度の更新
			pendulum.angle += pendulum.angularVelocity * kDeltaTime;
			// 振り子の位置を計算
			pendulumPosition.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
			pendulumPosition.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
			pendulumPosition.z = pendulum.anchor.z;
			
			// 振り子の位置をスクリーン座標に変換
			screenAnchor = debugCamera.WorldToScreen(pendulum.anchor);
			screenPosition = debugCamera.WorldToScreen(pendulumPosition);
		}


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


		Novice::DrawLine(
			static_cast<int>(screenAnchor.x),
			static_cast<int>(screenAnchor.y),
			static_cast<int>(screenPosition.x),
			static_cast<int>(screenPosition.y),
			WHITE);
		DrawSphere(Sphere(pendulumPosition, 0.05f), debugCamera.GetViewProjectionMatrix(), debugCamera.GetViewportMatrix(), WHITE);



#ifdef _DEBUG
		// デバッグウィンドウ
		debugCamera.Draw();

		// 振り子を揺らすための処理
		ImGui::Begin("Pendulum Control");
		if (ImGui::Button("Start")) {
			isSwing = !isSwing; // 振り子の揺れをトグル
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
