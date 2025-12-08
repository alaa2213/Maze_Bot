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

// Define struct for collectible vases
struct Vase {
    float x, z;
    bool isActive; // If false, the vase is collected (hidden)
};

// Snowflake particle for Level 2 atmosphere
struct Snowflake {
    float x, y, z;
    float speed;
    float size;
};

class Level {
public:
    Model_3DS coinModel;
    std::vector<Coin> coins;
    Model_3DS wallModel;
    std::vector<Wall> walls;

    // Level 2 Models
    Model_3DS vaseModel;
    Model_3DS iceModel;      // Small decorative ice blocks
    Model_3DS iceWallModel;  // Large ice walls for Level 2
    Model_3DS portalModel;

    // Vector to store multiple vases
    std::vector<Vase> vases;
    
    // Snow particles for Level 2
    std::vector<Snowflake> snowflakes;

    // Portal position (target in Level 2)
    float portalX;
    float portalZ;

    // Animation Variables
    float rotationAngle; // 0 to 360 degrees - used for door and portal rotation
    float hoverTime;     // Used for the scaling up/down effect
    
    int checkCoinCollisions(float playerX, float playerZ, float playerRadius);
    int checkVaseCollisions(float playerX, float playerZ, float playerRadius);
    
    Level();
    void loadAssets();
    void update(float deltaTime); // Calculates new angle/height
    void draw();
    void drawLevel2Objects(); // Draw Level 2 specific objects
    void drawDoor(Model_3DS& doorModel, float x, float y, float z); // Draw rotating door
    void drawGroundLevel1(); // Draw Level 1 ground
    void drawGroundLevel2(); // Draw Level 2 ground
    void drawSnow(); // Draw falling snow particles
    void initSnow(); // Initialize snow particles
    void updateSnow(float deltaTime); // Update snow positions
    void addWall(float x, float z);
    void addVase(float x, float z); // Helper to add vases

    // Collision Check
    // Returns TRUE if the coordinate (x,z) is inside any wall
    bool checkWallCollision(float x, float z, float playerRadius);
    // Helper to add coins easily
    void addCoin(float x, float z);
    
    // Get portal position
    float getPortalX();
    float getPortalZ();
};