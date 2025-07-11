#include <Novice.h>
#include "ApplicationConfig.h"
#include "EngineMath.h"
#include "EngineMathFunctions.h"
#include "MatrixGenerators.h"
#include "DebugRenderer.h"
#include "MyDebugCamera.h"
//#include <algorithm>

// 追加の構造体
struct Spring {
	// アンカー。固定された端の位置
	Vector3 anchor;
	float naturalLength;      // 自然長
	float stiffness;          // 剛性。ばね定数k
	float dampingCoefficient; // 減衰。減衰係数c
};

struct Ball {
	Vector3 position;         // ボールの位置
	Vector3 velocity;         // ボールの速度
	Vector3 acceleration;     // ボールの加速度
	float mass;               // ボールの質量
	float radius;             // ボールの半径
	unsigned int color;       // ボールの色
};


const char kWindowTitle[] = "LE2B_01_アカミネ_レン_MT3_04-00";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);


	// ばねの初期化
	Spring spring{};
	spring.anchor = { 0.0f, 0.0f, 0.0f };     // アンカーの位置
	spring.naturalLength = 1.0f;              // 自然長
	spring.stiffness = 100.0f;                // 剛性。ばね定数k
	spring.dampingCoefficient = 2.0f;         // 減衰。減衰係数c

	Vector3 screenAnchor = { 0.0f, 0.0f, 0.0f };

	// ボールの初期化
	Ball ball{};
	ball.position = { 1.2f, 0.0f, 0.0f };     // ボールの位置
	ball.velocity = { 0.0f, 0.0f, 0.0f };     // ボールの速度
	ball.acceleration = { 0.0f, 0.0f, 0.0f }; // ボールの加速度
	ball.mass = 2.0f;                         // ボールの質量
	ball.radius = 0.05f;                      // ボールの半径
	ball.color = BLUE;                        // ボールの色

	Vector3 screenPosition = { 0.0f, 0.0f, 0.0f };

	Vector3 diff{};
	float length{}; // ばねの長さ
	Vector3 direction{}; // ばねの方向
	Vector3 restPosition{}; // 自然長の位置
	Vector3 displacement{}; // ばねの変位
	Vector3 restoringForce{}; // ばねの復元力
	Vector3 dampingForce{}; // 減衰力
	Vector3 force{}; // 力の合計

	// ばねを動かす
	bool isStretched = false;

	// カメラの設定
	MyDebugCamera debugCamera; // インスタンスを生成


	// 座標系の変換
	screenPosition = debugCamera.WorldToScreen(ball.position);
	screenAnchor = debugCamera.WorldToScreen(spring.anchor);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

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

		if (isStretched) {
			spring.naturalLength = 1.0f; // スイッチがonなら伸ばす
		} else {
			spring.naturalLength = 1.2f; // スイッチがoffなら自然長に戻す
		}


		diff = ball.position - spring.anchor;
		length = Length(diff); // ばねの長さ
		if (length != 0.0f) {
			direction = Normalize(diff); // ばねの方向
			restPosition = spring.anchor + direction * spring.naturalLength; // 自然長の位置
			displacement.x = length - spring.naturalLength;  // ばねの変位
			restoringForce.x = -direction.x * spring.stiffness * displacement.x; // ばねの復元力
			dampingForce = -spring.dampingCoefficient * ball.velocity; // 減衰力
			force = restoringForce + dampingForce; // 力の合計
			ball.acceleration = force / ball.mass; // 加速度の計算
		} else {
			// ばねの長さが0の場合（ボールがアンカーに重なっている場合）は力が働かない
			ball.acceleration = { 0.0f, 0.0f, 0.0f };
			// 完全に重なっている場合は速度もゼロにするのが安全。
			// これがないと、もし速度が残っているとゼロの長さからすり抜ける場合がある
			ball.velocity = { 0.0f, 0.0f, 0.0f };
		}

		// 加速度も速度もどちらも秒を基準とした値である
		// それが、1/60秒間(deltaTime)適用されたと考える
		ball.velocity += ball.acceleration * kDeltaTime; // 速度の更新
		ball.position += ball.velocity * kDeltaTime; // 位置の更新

		// 座標系の変換
		screenPosition = debugCamera.WorldToScreen(ball.position);
		screenAnchor = debugCamera.WorldToScreen(spring.anchor);


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

		DrawSphere(
			Sphere(ball.position, ball.radius),
			debugCamera.GetViewProjectionMatrix(),
			debugCamera.GetViewportMatrix(),
			ball.color
		);

		// ばねの描画
		if (length != 0.0f) {
			Novice::DrawLine(
				static_cast<int>(screenAnchor.x),
				static_cast<int>(screenAnchor.y), 
				static_cast<int>(screenPosition.x),
				static_cast<int>(screenPosition.y),
				WHITE);
		}


#ifdef _DEBUG
		// デバッグウィンドウ
		debugCamera.Draw();

		// ばねを伸ばすスイッチ
		ImGui::Begin("Spring");
		if (ImGui::Button("Start")) {
			isStretched = !isStretched; // スイッチの切り替え
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
