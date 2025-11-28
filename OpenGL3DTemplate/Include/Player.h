#pragma once
class Player {
public:
    // Position & Orientation
    float x, y, z;
    float angle; // Rotation around Y axis

    // Physics
    float velocityY; // For jumping
    bool isJumping;
    int lives;       // Start with 5 [cite: 10]

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
    // Takes the coordinates of a wall to check if we hit it
    bool checkCollision(float wallX, float wallZ, float wallWidth, float wallDepth);

    // Getters for Camera to use
    float getX(); float getY(); float getZ();
	void loseLife(); // Deduct a life
};