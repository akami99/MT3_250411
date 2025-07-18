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

struct Pendulum {
	Vector3 anchor;                // アンカーポイント。固定された端の位置
	float length;                 // 紐の長さ
	float angle;                  // 現在の角度
	float angularVelocity;        // 角速度ω
	float angularAcceleration;    // 角加速度
};

#endif // ENGINE_MATH_H