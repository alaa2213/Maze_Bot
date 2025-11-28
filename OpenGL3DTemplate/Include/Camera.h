#pragma once
class Camera {
public:
    // Variables to control the view
    float eyeX, eyeY, eyeZ;
    float centerX, centerY, centerZ;
    float upX, upY, upZ;

    bool isFirstPerson; // To toggle modes

    // Constructor
    Camera();

    // Key function called every frame
    // playerX/Z: Where the robot is. 
    // playerAngle: Which way the robot is facing.
    void update(float playerX, float playerY, float playerZ, float playerAngle);

    // Call this when user presses Arrow Keys [cite: 32]
    void toggleMode();
};