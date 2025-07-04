#pragma once

#ifndef DEBUG_RENDERER_H
#define DEBUG_RENDERER_H

#include "EngineMath.h" // Sphere, AABB, Planeなどの構造体定義を含む
#include <cstdint>      // uint32_t for color

// 定数の宣言
static const int kRowHeight = 20;
static const int kColumnWidth = 60;

// 各種デバッグ用形状の描画

/// <summary>
/// 球を描画する関数
/// </summary>
/// <param name="sphere">球</param>
/// <param name="viewProjectionMatrix">ビュー射影行列</param>
/// <param name="viewportMatrix">ビューポート行列</param>
/// <param name="color">色</param>
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

/// <summary>
/// AABBを描画する関数
/// </summary>
/// <param name="aabb">AABB1</param>
/// <param name="viewProjectionMatrix">ビュー射影行列</param>
/// <param name="viewportMatrix">ビューポート行列</param>
/// <param name="color">色</param>
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

/// <summary>
/// 平面の描画関数
/// </summary>
/// <param name="plane">平面</param>
/// <param name="viewProjectionMatrix">ビュー射影行列</param>
/// <param name="viewportMatrix">ビューポート行列</param>
/// <param name="color">色</param>
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// グリッドの描画

/// <summary>
/// グリッドを描画する関数
/// </summary>
/// <param name="viewProjectionMatrix">ビュー射影行列</param>
/// <param name="viewportMatrix">ビューポート行列</param>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

// 数値の画面表示

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

#endif // DEBUG_RENDERER_H