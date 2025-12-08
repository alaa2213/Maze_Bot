#pragma once
class Player {
public:
    // Position & Orientation
    float x, y, z;
    float angle; // Rotation around Y axis

    // Physics
    float velocityY; // For jumping
    bool isJumping;
    int lives;       // Start with 5
    bool canMove;    // Controls if player can move

    Player();

    // Movement Functions
    void moveForward();  // W
    void moveBackward(); // S
    void turnLeft();     // A
    void turnRight();    // D
    void jump();         // J 

    // Physics Loop (Call this every frame)
    void updatePhysics();

    // Collision Logic
    bool checkCollision(float wallX, float wallZ, float wallWidth, float wallDepth);

    // Getters
    float getX(); float getY(); float getZ();
    void loseLife(); // Deduct a life
    void freezeMovement(); // Disable movement
    void rotateTowards(float targetAngle, float rotationSpeed); // Smooth rotation
    bool hasLives(); // Check if player still has lives
};