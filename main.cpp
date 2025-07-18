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
struct ConicalPendulum {
	Vector3 anchor;          // アンカーポイント。固定された端の位置
	float length;            // 紐の長さ
	float halfApexAngle;     // 円錐の頂角の半分
	float angle;             // 現在の角度
	float angularVelocity;   // 角速度ω
};


const char kWindowTitle[] = "LE2B_01_アカミネ_レン_MT3_04-03";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// 円錐振り子の設定
	ConicalPendulum conicalPendulum;
	conicalPendulum.anchor = { 0.0f, 1.0f, 0.0f };
	conicalPendulum.length = 0.8f;
	conicalPendulum.halfApexAngle = 0.7f;
	conicalPendulum.angle = 0.0f;
	conicalPendulum.angularVelocity = 0.0f;

	float radius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
	float height = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;
	
	// おもりの位置を計算
	Ball ball;
	ball.position.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * radius;
	ball.position.y = conicalPendulum.anchor.y - height;
	ball.position.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;
	ball.velocity = { 0.0f, 0.0f, 0.0f };
	ball.acceleration = { 0.0f, 0.0f, 0.0f };
	ball.radius = 0.05f;
	ball.mass = 0.1f;
	ball.color = WHITE;

	bool isSwing = false; // 振り子が揺れているかどうかのフラグ


	// カメラの設定
	MyDebugCamera debugCamera; // インスタンスを生成

	// スクリーン変換
	Vector3 screenAnchor = debugCamera.WorldToScreen(conicalPendulum.anchor);
	Vector3 screenBallPosition = debugCamera.WorldToScreen(ball.position);


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

		// 円錐振り子の更新
		if (isSwing) {
			// 角加速度を計算 (重力によるトルクを考慮)
			float gravity = 9.8f;
			conicalPendulum.angularVelocity = std::sqrtf(gravity / conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle));
			conicalPendulum.angle += conicalPendulum.angularVelocity * kDeltaTime;
			// おもりの位置を更新
			radius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
			height = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;
			
			ball.position.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * radius;
			ball.position.y = conicalPendulum.anchor.y - height;
			ball.position.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;

			// スクリーン変換
			screenAnchor = debugCamera.WorldToScreen(conicalPendulum.anchor);
			screenBallPosition = debugCamera.WorldToScreen(ball.position);
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

		// 円錐振り子のおもりを描画
		Novice::DrawLine(
			static_cast<int>(screenAnchor.x), 
			static_cast<int>(screenAnchor.y), 
			static_cast<int>(screenBallPosition.x),
			static_cast<int>(screenBallPosition.y),
			WHITE);

		DrawSphere(Sphere(ball.position, 0.05f), debugCamera.GetViewProjectionMatrix(), debugCamera.GetViewportMatrix(), WHITE);


#ifdef _DEBUG
		// デバッグウィンドウ
		debugCamera.Draw();

		// 円錐振り子揺らす
		ImGui::Begin("Conical Pendulum Control");
		if (ImGui::Button("Start")) {
			isSwing = !isSwing;
		}
		ImGui::DragFloat("Length", &conicalPendulum.length, 0.01f, 0.1f, 2.0f);
		ImGui::DragFloat("HalfApexAngle", &conicalPendulum.halfApexAngle, 0.01f, 0.0f, 1.5f);
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
