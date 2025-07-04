#pragma once

#ifndef ENGINE_MATH_H
#define ENGINE_MATH_H

// 必要であれば他のインクルードをここに書く

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Matrix4x4 {
	float m[4][4];
};

struct Segment {
	Vector3 origin; //!< 始点
	Vector3 diff;   //!< 終点への差分ベクトル
};

struct Line {
	Vector3 origin; //!< 始点
	Vector3 diff;   //!< 終点への差分ベクトル
};

struct Ray {
	Vector3 origin; //!< 始点
	Vector3 diff;   //!< 終点への差分ベクトル
};

struct Sphere {
	Vector3 center; //!< 中心点
	float radius;   //!< 半径
};

struct AABB {
	Vector3 min; //!< 最小点
	Vector3 max; //!< 最大点
};

struct Triangle {
	Vector3 vertices[3]; //!< 頂点
};

struct Plane {
	Vector3 normal; //!< 法線
	float distance; //!< 距離
};

#endif // ENGINE_MATH_H