#pragma once

#ifndef MATRIX_GENERATORS_H
#define MATRIX_GENERATORS_H

#include "EngineMath.h" // Matrix4x4構造体定義を含む

// 回転行列の生成

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

// アフィン変換行列の生成

/// <summary>
/// 3次元アフィン変換行列
/// </summary>
/// <param name="scale">縮尺</param>
/// <param name="rotate">角度</param>
/// <param name="translate">移動量</param>
/// <returns>3次元アフィン行列</returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// 射影行列の生成

/// <summary>
/// 透視投影行列
/// </summary>
/// <param name="fovY">縦画角</param>
/// <param name="aspectRatio">アスペクト比</param>
/// <param name="nearClip">近平面への距離</param>
/// <param name="farClip">遠平面への距離</param>
/// <returns>透視投影行列</returns>
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// ビューポート行列の生成

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

#endif // MATRIX_GENERATORS_H