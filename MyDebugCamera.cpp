#include "MyDebugCamera.h"
#include "ApplicationConfig.h" // kWindowWidth, kWindowHeightなどの定数
#include "EngineMathFunctions.h" // Add, Subtract, Multiply, Inverseなど
#include "MatrixGenerators.h"    // MakeAffineMatrix, MakePerspectiveFovMatrix, MakeViewportMatrixなど

// コンストラクタでの初期化
MyDebugCamera::MyDebugCamera() : cameraTranslate_{ 0.0f, 1.9f, -6.49f }, cameraRotate_{ 0.26f, 0.0f, 0.0f } {
    // 初期行列の計算
    cameraMatrix_ = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate_, cameraTranslate_);
    viewMatrix_ = Inverse(cameraMatrix_);
    projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
    viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);

    viewportMatrix_ = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f);
}

Vector3 MyDebugCamera::WorldToScreen(const Vector3& worldPos) {
    // ワールド座標をスクリーン座標に変換
    return Transform(Transform(worldPos, viewProjectionMatrix_), viewportMatrix_);
}

void MyDebugCamera::Update() {
#ifdef _DEBUG
    // カメラの更新
    cameraMatrix_ = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate_, cameraTranslate_);
    viewMatrix_ = Inverse(cameraMatrix_);
    projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
    viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
#endif // _DEBUG
}

void MyDebugCamera::Draw() {
#ifdef _DEBUG
    // デバッグウィンドウの表示
    ImGui::Begin("DebugCamera Window"); // ウィンドウ名は既存と区別できるように変更
    ImGui::Text("Camera");
    ImGui::DragFloat3("CameraTranslate", &cameraTranslate_.x, 0.01f);
    ImGui::DragFloat3("CameraRotate", &cameraRotate_.x, 0.01f);
    ImGui::End();
#endif // _DEBUG
}