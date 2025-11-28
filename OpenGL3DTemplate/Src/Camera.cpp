#include "Include/Camera.h"
#include <cmath>
#include <iostream>

#define DEG2RAD(a) (a * 0.0174532925)

Camera::Camera() {
    // Default to Third Person view
    isFirstPerson = false;

    // Up vector is almost always (0, 1, 0) in games
    upX = 0.0f; upY = 1.0f; upZ = 0.0f;
}

// [cite: 32] Toggle between views
void Camera::toggleMode() {
    isFirstPerson = !isFirstPerson;
    std::cout << "Camera Mode: " << (isFirstPerson ? "First Person" : "Third Person") << std::endl;
}

// Calculates where the camera should be based on player position
void Camera::update(float playerX, float playerY, float playerZ, float playerAngle) {

    float angleRad = DEG2RAD(playerAngle);

    if (isFirstPerson) {
        // --- FIRST PERSON [cite: 27] ---
        // Camera is exactly at the player's eye level
        eyeX = playerX;
        eyeY = playerY + 1.0f; // +1.0 height for the "eye"
        eyeZ = playerZ;

        // Look at a point in front of the player
        centerX = eyeX + sin(angleRad);
        centerY = eyeY; // Look straight ahead
        centerZ = eyeZ + cos(angleRad);
    }
    else {
       // --- THIRD PERSON [cite: 28] ---
        // Camera is behind and slightly above the player
        float distanceFromPlayer = 5.0f;
        float heightAbovePlayer = 3.0f;

        // Calculate position "behind" the player
        // We subtract sin/cos to go opposite to the facing direction
        eyeX = playerX - (distanceFromPlayer * sin(angleRad));
        eyeY = playerY + heightAbovePlayer;
        eyeZ = playerZ - (distanceFromPlayer * cos(angleRad));

        // Look directly at the player
        centerX = playerX;
        centerY = playerY;
        centerZ = playerZ;
    }
}