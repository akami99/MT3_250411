#include <Novice.h>
#include "ApplicationConfig.h"
#include "EngineMath.h"
#include "EngineMathFunctions.h"
#include "MatrixGenerators.h"
#include "DebugRenderer.h"
#include "MyDebugCamera.h"
#include <cmath>
//#include <algorithm>

// 追加の関数

// 球との衝突判定を行う関数
bool IsCollision(const Sphere& sphere, const Plane& plane) {
	// 平面の法線ベクトルを正規化
	Vector3 normal = Normalize(plane.normal);
	// 球の中心から平面までの距離を計算
	float distance = Dot(plane.normal, sphere.center) + plane.distance;
	// 球の半径と平面までの距離を比較
	return distance <= sphere.radius;
}

// ベクトルを法線方向に投影する関数
Vector3 Project(const Vector3& vector, const Vector3& normal) {
	// ベクトルを法線方向に投影する
	float dotProduct = Dot(vector, normal);
	return normal * dotProduct;
}

const char kWindowTitle[] = "LE2B_01_アカミネ_レン_MT3_04-04";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// 重力の設定
	float gravity = -9.8f;

	// 平面の設定
	Plane plane;
	plane.normal = { -0.2f, 0.9f, -0.3f }; // 平面の法線ベクトル
	plane.distance = 0.0f; // 平面の原点からの距離

	// 球の設定
	Ball ball;
	ball.position = { 0.8f, 1.2f, 0.3f }; // 球の位置
	ball.velocity = { 0.0f, 0.0f, 0.0f }; // 球の速度
	ball.acceleration = { 0.0f, 0.0f, 0.0f }; // 球の加速度
	ball.mass = 2.0f; // 球の質量
	ball.radius = 0.05f; // 球の半径
	ball.color = WHITE; // 球の色

	// 反発係数の設定
	float e = 0.8f;

	// 球を落とすかどうかのフラグ
	bool isDrop = false;
	

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

		if (isDrop) {
			// 球に重力を適用する
			ball.acceleration = { 0.0f, gravity, 0.0f }; // 重力の影響を加える
		} else {
			ball.position = { 0.8f, 1.2f, 0.3f }; // 球の位置を初期位置に戻す
			ball.velocity = { 0.0f, 0.0f, 0.0f }; // 球の速度をゼロにする
			// 球を落とすフラグが立っていない場合は加速度をゼロにする
			ball.acceleration = { 0.0f, 0.0f, 0.0f };
		}

		// 球の位置を更新する
		ball.velocity += ball.acceleration * kDeltaTime; // 速度の更新
		ball.position += ball.velocity * kDeltaTime; // 位置の更新

		if (IsCollision(Sphere{ball.position, ball.radius}, plane)) {
			Vector3 reflected = Reflect(ball.velocity, plane.normal); // 反射ベクトルの計算
			Vector3 projectToNomal = Project(reflected, plane.normal); // 法線方向への投影
			Vector3 movingDirection = reflected - projectToNomal; // 反射ベクトルから法線方向への投影を引く
			ball.velocity = projectToNomal * e + movingDirection; // 反発係数による減衰を法線方向にだけ入れて速度を更新
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

		// 平面を描画
		DrawPlane(plane, debugCamera.GetViewProjectionMatrix(), debugCamera.GetViewportMatrix(), WHITE);

		// 球を描画
		DrawSphere(Sphere{ball.position, ball.radius}, debugCamera.GetViewProjectionMatrix(), debugCamera.GetViewportMatrix(), ball.color);



#ifdef _DEBUG
		// デバッグウィンドウ
		debugCamera.Draw();

		// 球を落とすトグル
		ImGui::Begin("Ball Control");
		if (ImGui::Button("Start")) {
			isDrop = !isDrop; // ボタンが押されたらフラグをトグル
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
