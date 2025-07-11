#pragma once

#ifndef MY_DEBUG_CAMERA_H
#define MY_DEBUG_CAMERA_H

#include "EngineMath.h" // Vector3, Matrix4x4など
#include <imgui.h>      // ImGui関連のヘッダー

class MyDebugCamera {
public:
    // コンストラクタ
    MyDebugCamera();

	Vector3 WorldToScreen(const Vector3& worldPos);

    // 更新処理
    void Update();

    // ImGuiによるデバッグUIを表示
    void Draw();

    // getter
    const Matrix4x4& GetViewProjectionMatrix() const {
        return viewProjectionMatrix_;
    }
    const Matrix4x4& GetViewportMatrix() const {
        return viewportMatrix_;
    }

private:
    Vector3 cameraTranslate_;
    Vector3 cameraRotate_;

    Matrix4x4 cameraMatrix_;
    Matrix4x4 viewMatrix_;
    Matrix4x4 projectionMatrix_;
    Matrix4x4 viewProjectionMatrix_;
    Matrix4x4 viewportMatrix_;
};

#endif // DEBUG_CAMERA_H

