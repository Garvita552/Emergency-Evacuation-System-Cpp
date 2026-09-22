#include "../../include/Core/Camera3D.hpp"
#include <algorithm>
#include <cmath>

namespace Futura {

constexpr float DEG2RAD = 3.14159265358979323846f / 180.0f;

Camera3D::Camera3D()
    : target(15.0f, 4.0f, 15.0f), distance(35.0f), yaw(45.0f), pitch(35.0f), fov(60.0f), focusFloor(-1) {}

Vector3 Camera3D::getPosition() const {
    float radYaw = yaw * DEG2RAD;
    float radPitch = pitch * DEG2RAD;

    float x = target.x + distance * std::cos(radPitch) * std::sin(radYaw);
    float y = target.y + distance * std::sin(radPitch);
    float z = target.z + distance * std::cos(radPitch) * std::cos(radYaw);

    return Vector3(x, y, z);
}

void Camera3D::rotate(float deltaYaw, float deltaPitch) {
    yaw += deltaYaw;
    pitch += deltaPitch;
    pitch = std::clamp(pitch, -85.0f, 85.0f);
}

void Camera3D::zoom(float deltaDistance) {
    distance -= deltaDistance;
    distance = std::clamp(distance, 5.0f, 150.0f);
}

void Camera3D::pan(float deltaX, float deltaY) {
    float radYaw = yaw * DEG2RAD;
    Vector3 right(std::cos(radYaw), 0.0f, -std::sin(radYaw));
    Vector3 up(0.0f, 1.0f, 0.0f);

    target += right * deltaX + up * deltaY;
}

void Camera3D::reset() {
    target = Vector3(15.0f, 4.0f, 15.0f);
    distance = 35.0f;
    yaw = 45.0f;
    pitch = 35.0f;
    focusFloor = -1;
}

void Camera3D::setFocusFloor(int floorIndex) {
    focusFloor = floorIndex;
    if (floorIndex >= 0) {
        target.y = floorIndex * 4.0f + 1.5f;
    } else {
        target.y = 4.0f;
    }
}

} // namespace Futura
