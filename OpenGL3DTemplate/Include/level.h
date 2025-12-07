#pragma once
#pragma once
#include <vector>
#include "MODEL_3DS.h" // Your pre-written header

// 1. Define a simple struct for a single coin's data
struct Coin {
    float x,z;
    bool isActive; // If false, the coin is hidden (collected)
};
struct Wall {
    float x, z;
    float width, depth; // Size of the wall for collision
};

class Level {
public:
    Model_3DS coinModel;
    std::vector<Coin> coins;
    Model_3DS wallModel;
	std::vector<Wall> walls;

    // Animation Variables
    float rotationAngle; // 0 to 360 degrees
    float hoverTime;     // Used for the bobbing up/down effect
    int checkCoinCollisions(float playerX, float playerZ, float playerRadius);
    Level();
    void loadAssets();
    void update(float deltaTime); // Calculates new angle/height
    void draw();
    void addWall(float x, float z);

    // Collision Check
    // Returns TRUE if the coordinate (x,z) is inside any wall
    bool checkWallCollision(float x, float z, float playerRadius);
    // Helper to add coins easily
    void addCoin(float x, float z);
};