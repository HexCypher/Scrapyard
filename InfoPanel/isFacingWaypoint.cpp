#include"isFacingWaypoint.h"

// Function to determine if facing the waypoint with a specified tolerance
bool isFacingWaypoint(float currentYaw, float waypointX, float waypointY, float playerX, float playerY, float tolerance = 15.0f) {
    // Calculate the angle from the player to the waypoint
    float deltaX = waypointX - playerX;
    float deltaY = waypointY - playerY;
    float desiredYaw = static_cast<float>(atan2(deltaY, deltaX) * (180.0 / 3.141592653589793));

    // Normalize the desiredYaw to the range [0, 360)
    if (desiredYaw < 0) {
        desiredYaw += 360.0f;
    }

    // Normalize the currentYaw to the range [0, 360)
    currentYaw = fmod(currentYaw, 360.0f);
    if (currentYaw < 0) {
        currentYaw += 360.0f;
    }

    // Calculate the angle difference
    float angleDifference = fabs(currentYaw - desiredYaw);

    // Adjust for circular range
    if (angleDifference > 180.0f) {
        angleDifference = 360.0f - angleDifference;
    }

    // Debug output
    std::cout << "Current Yaw: " << currentYaw << " Desired Yaw: " << desiredYaw << " Angle Difference: " << angleDifference << std::endl;

    // Check if the angle difference is within the tolerance
    return angleDifference <= tolerance;
}
