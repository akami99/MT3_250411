#include <Novice.h>
#include <cmath>
#include <cassert>
#include <imgui.h>
#include <numbers>


static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Matrix4x4 {
	float m[4][4];
};

struct Segment {
	Vector3 origin; //!<< 始点
	Vector3 diff;   //!<< 終点への差分ベクトル
};

struct Sphere {
	Vector3 center; //!< 中心点
	float radius;   //!< 半径
};

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

/// <summary>
/// ベクトル数値表示
/// </summary>
/// <param name="x">表示位置</param>
/// <param name="y">表示位置</param>
/// <param name="vector">表示するベクトル</param>
/// <param name="label">名前</param>
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

/// <summary>
/// 4x4行列数値表示
/// </summary>
/// <param name="x">表示位置</param>
/// <param name="y">表示位置</param>
/// <param name="matrix">表示する4x4行列</param>
/// <param name="label">名前</param>
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

/// <summary>
/// 加算
/// </summary>
/// <param name="v1">行列1</param>
/// <param name="v2">行列2</param>
/// <returns>行列の和</returns>
Vector3 Add(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 減算
/// </summary>
/// <param name="v1">引かれるベクトル</param>
/// <param name="v2">引くベクトル</param>
/// <returns>ベクトルの差</returns>
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 4x4行列の積
/// </summary>
/// <param name="m1">掛ける行列1</param>
/// <param name="m2">掛ける行列2</param>
/// <returns>行列の積</returns>
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 座標変換
/// </summary>
/// <param name="vector">変換したいベクトル</param>
/// <param name="matrix">変換させる行列</param>
/// <returns>変換させた座標</returns>
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);

// 最近接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

/// <summary>
/// X軸回転行列
/// </summary>
/// <param name="angle">角度</param>
/// <returns>X軸回転行列</returns>
Matrix4x4 MakeRotateXMatrix(float angle);

/// <summary>
/// Y軸回転行列
/// </summary>
/// <param name="angle">角度</param>
/// <returns>Y軸回転行列</returns>
Matrix4x4 MakeRotateYMatrix(float angle);

/// <summary>
/// Z軸回転行列
/// </summary>
/// <param name="angle">角度</param>
/// <returns>Z軸回転行列</returns>
Matrix4x4 MakeRotateZMatrix(float angle);

/// <summary>
/// 3次元アフィン変換行列
/// </summary>
/// <param name="scale">縮尺</param>
/// <param name="rotate">角度</param>
/// <param name="translate">移動量</param>
/// <returns>3次元アフィン行列</returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

/// <summary>
/// 透視投影行列
/// </summary>
/// <param name="fovY">縦画角</param>
/// <param name="aspectRatio">アスペクト比</param>
/// <param name="nearClip">近平面への距離</param>
/// <param name="farClip">遠平面への距離</param>
/// <returns>透視投影行列</returns>
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

/// <summary>
/// ビューポート変換行列
/// </summary>
/// <param name="left">左座標</param>
/// <param name="top">上座標</param>
/// <param name="width">横幅</param>
/// <param name="height">縦幅</param>
/// <param name="minDepth">最小深度値</param>
/// <param name="maxDepth">最大深度値</param>
/// <returns>ビューポート行列</returns>
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

/// <summary>
/// 球面をデカルト座標に変換
/// </summary>
/// <param name="radius">半径</param>
/// <param name="lat">緯度</param>
/// <param name="lon">経度</param>
/// <returns>デカルト座標</returns>
Vector3 SphericalToCartesian(float radius, float lat, float lon);

/// <summary>
/// 球を描画する関数
/// </summary>
/// <param name="sphere">球</param>
/// <param name="viewProjectionMatrix">ビュー射影行列</param>
/// <param name="viewportMatrix">ビューポート行列</param>
/// <param name="color">色</param>
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

/// <summary>
/// グリッドを描画する関数
/// </summary>
/// <param name="viewProjectionMatrix">ビュー射影行列</param>
/// <param name="viewportMatrix">ビューポート行列</param>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

/// <summary>
/// 逆行列
/// </summary>
/// <param name="m">逆行列に変換する行列</param>
/// <returns>逆行列</returns>
Matrix4x4 Inverse(const Matrix4x4& m);


const char kWindowTitle[] = "LE2B_01_アカミネ_レン_MT3_02_00";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);


	Segment segment{ {-2.0f, -1.0f, 0.0f}, {3.0f, 2.0f, 2.0f} };
	Vector3 point{ -1.5f, 0.6f, 0.6f };

	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	Vector3 closestPoint = ClosestPoint(point, segment);

	Sphere pointSphere{ point, 0.01f };// 1cmの弾を描画
	Sphere closestPointSphere{ closestPoint, 0.01f };


	// カメラの設定
	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
	Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

	Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f);


	// 線の始点と終点
	Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);


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

		project = Project(Subtract(point, segment.origin), segment.diff);
		closestPoint = ClosestPoint(point, segment);

		pointSphere = { point, 0.01f };// 1cmの弾を描画
		closestPointSphere = { closestPoint, 0.01f };

		start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
		end = Transform(Transform(Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);


#ifdef _DEBUG
		// カメラの更新
		cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		viewMatrix = Inverse(cameraMatrix);
		projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
		viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

#endif // _DEBUG

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);


		DrawSphere(pointSphere, viewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(closestPointSphere, viewProjectionMatrix, viewportMatrix, BLACK);

		Novice::DrawLine(
			static_cast<int>(start.x), static_cast<int>(start.y), 
			static_cast<int>(end.x), static_cast<int>(end.y),
			WHITE);


#ifdef _DEBUG
		// デバッグウィンドウ
		ImGui::Begin("Window");
		//ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		//ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("Point", &point.x, 0.01f);
		ImGui::DragFloat3("Segment origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Segment diff", &segment.diff.x, 0.01f);
		ImGui::DragFloat3("Project", &project.x, 0.01f);
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

// ベクトル数値表示
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

// 4x4行列数値表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + row * kRowHeight + kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

// 4x4行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result = {}; // w=1がデカルト座標系であるので(x,y,z,1)のベクトルとしてmatrixとの積を取る
	result.x =
		vector.x * matrix.m[0][0] +
		vector.y * matrix.m[1][0] +
		vector.z * matrix.m[2][0] +
		1.0f * matrix.m[3][0];
	result.y =
		vector.x * matrix.m[0][1] +
		vector.y * matrix.m[1][1] +
		vector.z * matrix.m[2][1] +
		1.0f * matrix.m[3][1];
	result.z =
		vector.x * matrix.m[0][2] +
		vector.y * matrix.m[1][2] +
		vector.z * matrix.m[2][2] +
		1.0f * matrix.m[3][2];
	float w =
		vector.x * matrix.m[0][3] +
		vector.y * matrix.m[1][3] +
		vector.z * matrix.m[2][3] +
		1.0f * matrix.m[3][3];
	assert(w != 0.0f); // ベクトルに対して基本的な操作を行う行列でwが０になることはありえない
	result.x /= w;  // w=1がデカルト座標系であるので、w除算することで同次座標をデカルト座標に戻す
	result.y /= w;
	result.z /= w;
	return result;
}

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2) {
	Vector3 result = {};
	float nullCheck = (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
	if (nullCheck == 0.0f) {
		return result;
	}

	float dotRatio =
		(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) /
		(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);

	result.x = dotRatio * v2.x;
	result.y = dotRatio * v2.y;
	result.z = dotRatio * v2.z;
	return result;
}

// 最近接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 result = {};
	Vector3 segVec = segment.diff;

	float segLenSq = segVec.x * segVec.x + segVec.y * segVec.y + segVec.z * segVec.z;
	if (segLenSq == 0.0f) {
		return segment.origin;
	}

	Vector3 pointVec = {
		point.x - segment.origin.x,
		point.y - segment.origin.y,
		point.z - segment.origin.z
	};
	float t = (pointVec.x * segVec.x + pointVec.y * segVec.y + pointVec.z * segVec.z) / segLenSq;
	// 線分内に制限
	if (t < 0.0f) {
		t = 0.0f;
	} else if (t > 1.0f) {
		t = 1.0f;
	}

	result.x = segment.origin.x + t * segVec.x;
	result.y = segment.origin.y + t * segVec.y;
	result.z = segment.origin.z + t * segVec.z;
	return result;
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float angle) {
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[3][3] = 1.0f;
	result.m[1][1] = std::cos(angle);
	result.m[1][2] = std::sin(angle);
	result.m[2][1] = -std::sin(angle);
	result.m[2][2] = std::cos(angle);
	return result;
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float angle) {
	Matrix4x4 result = {};
	result.m[1][1] = 1.0f;
	result.m[3][3] = 1.0f;
	result.m[0][0] = std::cos(angle);
	result.m[0][2] = -std::sin(angle);
	result.m[2][0] = std::sin(angle);
	result.m[2][2] = std::cos(angle);
	return result;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float angle) {
	Matrix4x4 result = {};
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	result.m[0][0] = std::cos(angle);
	result.m[0][1] = std::sin(angle);
	result.m[1][0] = -std::sin(angle);
	result.m[1][1] = std::cos(angle);
	return result;
}

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result = {};
	// X,Y,Z軸の回転をまとめる
	Matrix4x4 rotateXYZ =
		Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

	result.m[0][0] = scale.x * rotateXYZ.m[0][0];
	result.m[0][1] = scale.x * rotateXYZ.m[0][1];
	result.m[0][2] = scale.x * rotateXYZ.m[0][2];
	result.m[1][0] = scale.y * rotateXYZ.m[1][0];
	result.m[1][1] = scale.y * rotateXYZ.m[1][1];
	result.m[1][2] = scale.y * rotateXYZ.m[1][2];
	result.m[2][0] = scale.z * rotateXYZ.m[2][0];
	result.m[2][1] = scale.z * rotateXYZ.m[2][1];
	result.m[2][2] = scale.z * rotateXYZ.m[2][2];
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
}

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result{};
	//(1/a)*(1/tan(fovY/2)=1/(a*tan(fovY/2))
	result.m[0][0] = 1.0f / (aspectRatio * tanf(fovY / 2.0f));
	result.m[1][1] = 1.0f / tanf(fovY / 2.0f);
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][2] = -(farClip * nearClip) / (farClip - nearClip);
	return result;
}

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result{};
	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -(height / 2.0f);
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + (width / 2.0f);
	result.m[3][1] = top + (height / 2.0f);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1.0f;
	return result;
}

// 球面をデカルト座標に変換
Vector3 SphericalToCartesian(float radius, float lat, float lon) {
	float x = radius * cosf(lat) * cosf(lon);
	float y = radius * sinf(lat);
	float z = radius * cosf(lat) * sinf(lon);
	return { x, y, z };
}

// 球を描画する関数
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 12;   // 分割数
	const float kLonEvery = static_cast<float>(std::numbers::pi * 2 / kSubdivision);      // 経度分割1つ分の角度(φ)
	const float kLatEvery = static_cast<float>(std::numbers::pi / kSubdivision);      // 緯度分割1つ分の角度(θ)
	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = static_cast<float>(-std::numbers::pi / 2.0f + kLatEvery * latIndex);// 現在の緯度
		float latNext = lat + kLatEvery;

		// 経度の方向に分割 0 ~ π/2
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;// 現在の経度
			float lonNext = lon + kLonEvery;

			// world座標系でa,b,cを求める
			Vector3 a = Add(SphericalToCartesian(sphere.radius, lat, lon), sphere.center);
			Vector3 b = Add(SphericalToCartesian(sphere.radius, latNext, lon), sphere.center);
			Vector3 c = Add(SphericalToCartesian(sphere.radius, lat, lonNext), sphere.center);

			// a,b,cをScreen座標系まで変換...
			Vector3 screenA = Transform(Transform(a, viewProjectionMatrix), viewportMatrix);
			Vector3 screenB = Transform(Transform(b, viewProjectionMatrix), viewportMatrix);
			Vector3 screenC = Transform(Transform(c, viewProjectionMatrix), viewportMatrix);

			// ab,bc->acで線を引く
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y),
				static_cast<int>(screenB.x), static_cast<int>(screenB.y), color);
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y),
				static_cast<int>(screenC.x), static_cast<int>(screenC.y), color); // a→c
		}
	}
}

// グリッドを描画する関数
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;                                                  // Gridの半分の幅
	const uint32_t kSubdivision = 10;                                                   // 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / static_cast<float>(kSubdivision);// 1つ分の長さ
	// 奥から手前への線を徐々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		// 上の情報を使ってワ－ルド座標系上の始点と終点を求める
		float x = -kGridHalfWidth + kGridEvery * static_cast<float>(xIndex);

		Vector3 start = { x, 0.0f, -kGridHalfWidth };
		Vector3 end = { x, 0.0f, kGridHalfWidth };

		// スクリーン座標系まで変換を書ける
		Vector3 screenStart = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 screenEnd = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		// 変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF)、原点は黒ぐらいが良いが、何でも良い
		if (xIndex == kSubdivision / 2) {
			Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
				static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0x000000FF);
		} else {
			Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
				static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0xAAAAAAFF);
		}
	}
	// 左から右も同じように順々にひいていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// 奥から手前が左右に変わるだけ
		// 上の情報を使ってワ－ルド座標系上の始点と終点を求める
		float z = -kGridHalfWidth + kGridEvery * static_cast<float>(zIndex);

		Vector3 start = { -kGridHalfWidth, 0.0f, z };
		Vector3 end = { kGridHalfWidth, 0.0f, z };

		// スクリーン座標系まで変換を書ける
		Vector3 screenStart = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 screenEnd = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		// 変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF)、原点は黒ぐらいが良いが、何でも良い
		if (zIndex == kSubdivision / 2) {
			Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
				static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0x000000FF);
		} else {
			Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
				static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0xAAAAAAFF);
		}
	}
}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result;
	float determinant = 0;
	// 行列式を計算
	determinant =
		m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]
		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]
		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	// 逆行列を計算
	result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) / determinant;
	result.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) / determinant;
	result.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) / determinant;
	result.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) / determinant;

	result.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2]
		+ m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) / determinant;
	result.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) / determinant;
	result.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2]
		+ m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) / determinant;
	result.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) / determinant;

	result.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1]
		- m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) / determinant;
	result.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1]
		+ m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) / determinant;
	result.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1]
		- m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) / determinant;
	result.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1]
		+ m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) / determinant;

	result.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1]
		+ m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) / determinant;
	result.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1]
		- m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) / determinant;
	result.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1]
		+ m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) / determinant;
	result.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1]
		- m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) / determinant;

	return result;
}