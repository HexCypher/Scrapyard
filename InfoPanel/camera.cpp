#include "camera.h"

Camera::Camera(HANDLE processHandle) : hProcess(processHandle), yaw(0) {
    if (hProcess == NULL) {
        std::cerr << "Failed to obtain process handle." << std::endl;
    }
}

Camera::~Camera() {
    if (hProcess) {
        CloseHandle(hProcess);
    }
}

float Camera::readFloat(DWORD address) {
    float value = 0.0f;
    if (ReadProcessMemory(hProcess, (LPCVOID)(uintptr_t)address, &value, sizeof(value), nullptr) == 0) {
        std::cerr << "Failed to read memory at address: " << std::hex << address << " (Error Code: " << GetLastError() << ")" << std::endl;
    }
    return value;
}

void Camera::readYaw(DWORD yawAddress) {
    yaw = readFloat(yawAddress);
    std::cout << "Yaw: " << yaw << std::endl;
}
