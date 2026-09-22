#ifndef CAMERA3D_HPP
#define CAMERA3D_HPP

#include "Vector3.hpp"

namespace Futura {

class Camera3D {
public:
    Vector3 target;
    float distance;
    float yaw;       // In degrees
    float pitch;     // In degrees
    float fov;       // Field of view in degrees
    int focusFloor;  // -1 for all floors, 0..N for specific floor focus

    Camera3D();
    
    Vector3 getPosition() const;
    void rotate(float deltaYaw, float deltaPitch);
    void zoom(float deltaDistance);
    void pan(float deltaX, float deltaY);
    void reset();
    void setFocusFloor(int floorIndex);
};

} // namespace Futura

#endif // CAMERA3D_HPP
