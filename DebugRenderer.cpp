#include "DebugRenderer.h"
#include "EngineMathFunctions.h"
#include <Novice.h>
#include <numbers>

// 各種デバッグ用形状の描画

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

// AABBを描画する関数
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	// AABBの8つの頂点を求める
	Vector3 vertices[8] = {
		{ aabb.min.x, aabb.min.y, aabb.min.z }, // 0
		{ aabb.min.x, aabb.min.y, aabb.max.z }, // 1
		{ aabb.min.x, aabb.max.y, aabb.max.z }, // 2
		{ aabb.min.x, aabb.max.y, aabb.min.z }, // 3
		{ aabb.max.x, aabb.min.y, aabb.min.z }, // 4
		{ aabb.max.x, aabb.min.y, aabb.max.z }, // 5
		{ aabb.max.x, aabb.max.y, aabb.max.z }, // 6
		{ aabb.max.x, aabb.max.y, aabb.min.z }  // 7
	};

	// それぞれの頂点をスクリーン座標系に変換
	for (int i = 0; i < 8; ++i) {
		vertices[i] = Transform(Transform(vertices[i], viewProjectionMatrix), viewportMatrix);
	}

	// AABBのエッジを描画する
	// 12本のエッジを描画する
	// 0-1, 1-2, 2-3, 3-0, 4-5, 5-6, 6-7, 7-4, 0-4, 1-5, 2-6, 3-7

	// 0-1, 1-2, 2-3, 3-0
	Novice::DrawLine(
		static_cast<int>(vertices[0].x), static_cast<int>(vertices[0].y),
		static_cast<int>(vertices[1].x), static_cast<int>(vertices[1].y), color
	);
	Novice::DrawLine(
		static_cast<int>(vertices[1].x), static_cast<int>(vertices[1].y),
		static_cast<int>(vertices[2].x), static_cast<int>(vertices[2].y), color
	);
	Novice::DrawLine(
		static_cast<int>(vertices[2].x), static_cast<int>(vertices[2].y),
		static_cast<int>(vertices[3].x), static_cast<int>(vertices[3].y), color
	);
	Novice::DrawLine(
		static_cast<int>(vertices[3].x), static_cast<int>(vertices[3].y),
		static_cast<int>(vertices[0].x), static_cast<int>(vertices[0].y), color
	);

	// 4-5, 5-6, 6-7, 7-4
	Novice::DrawLine(
		static_cast<int>(vertices[4].x), static_cast<int>(vertices[4].y),
		static_cast<int>(vertices[5].x), static_cast<int>(vertices[5].y), color
	);
	Novice::DrawLine(
		static_cast<int>(vertices[5].x), static_cast<int>(vertices[5].y),
		static_cast<int>(vertices[6].x), static_cast<int>(vertices[6].y), color
	);
	Novice::DrawLine(
		static_cast<int>(vertices[6].x), static_cast<int>(vertices[6].y),
		static_cast<int>(vertices[7].x), static_cast<int>(vertices[7].y), color
	);
	Novice::DrawLine(
		static_cast<int>(vertices[7].x), static_cast<int>(vertices[7].y),
		static_cast<int>(vertices[4].x), static_cast<int>(vertices[4].y), color
	);

	// 0-4, 1-5, 2-6, 3-7
	Novice::DrawLine(
		static_cast<int>(vertices[0].x), static_cast<int>(vertices[0].y),
		static_cast<int>(vertices[4].x), static_cast<int>(vertices[4].y), color
	);
	Novice::DrawLine(
		static_cast<int>(vertices[1].x), static_cast<int>(vertices[1].y),
		static_cast<int>(vertices[5].x), static_cast<int>(vertices[5].y), color
	);
	Novice::DrawLine(
		static_cast<int>(vertices[2].x), static_cast<int>(vertices[2].y),
		static_cast<int>(vertices[6].x), static_cast<int>(vertices[6].y), color
	);
	Novice::DrawLine(
		static_cast<int>(vertices[3].x), static_cast<int>(vertices[3].y),
		static_cast<int>(vertices[7].x), static_cast<int>(vertices[7].y), color
	);
}

// 平面の描画関数
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = Multiply(plane.distance, plane.normal); // 1
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));  // 2
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };// 3
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]); // 4
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };// 5
	// 6
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}
	// pointsをそれぞれ結んでDrawLineで矩形を描画する。DrawTriangleを使って塗りつぶしても良いが、DepthがないのでMT3では分かりずらい
	Novice::DrawLine(static_cast<int>(points[0].x), static_cast<int>(points[0].y),
		static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(static_cast<int>(points[1].x), static_cast<int>(points[1].y),
		static_cast<int>(points[3].x), static_cast<int>(points[3].y), color);
	Novice::DrawLine(static_cast<int>(points[2].x), static_cast<int>(points[2].y),
		static_cast<int>(points[1].x), static_cast<int>(points[1].y), color);
	Novice::DrawLine(static_cast<int>(points[3].x), static_cast<int>(points[3].y),
		static_cast<int>(points[0].x), static_cast<int>(points[0].y), color);
}

// グリッドの描画

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

// 数値の画面表示

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