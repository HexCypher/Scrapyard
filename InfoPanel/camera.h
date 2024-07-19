#ifndef CAMERA_H
#define CAMERA_H

#include <windows.h>
#include <iostream>

class Camera {
public:
    Camera(HANDLE processHandle);
    ~Camera();
    float readFloat(DWORD address);
    void readYaw(DWORD yawAddress);
    float getYaw() const { return yaw; }

private:
    HANDLE hProcess;
    float yaw;
};

#endif // CAMERA_H
