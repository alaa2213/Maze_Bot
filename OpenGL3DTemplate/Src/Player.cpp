#include "Include/Player.h"
#include <cmath> // Required for sin() and cos()
#include <iostream>
#include <algorithm> // Required for std::max and std::min
#include <vector> // Required for std::vector

#define DEG2RAD(a) (a * 0.0174532925)

// Constructor: Set starting values
Player::Player() {
    // Start at a safe position away from walls (southwest corner of maze)
    x = -9.0f;
    y = 0.0f;     // Start on the ground
    z = -9.0f;
    angle = 0.0f; // Facing "North" initially

    velocityY = 0.0f;
    isJumping = false;
    lives = 5; // [cite: 10] Player starts with 5 lives
}
extern float player_radius;  // Tells Player class it exists elsewhere

// --- Movement Logic [cite: 30] ---

void Player::moveForward() {
    // Move "Forward" relative to where the player is facing
    // We update x and z based on the angle
    float speed = 0.1f;
    float newX = x + speed * sin(DEG2RAD(angle));
    float newZ = z + speed * cos(DEG2RAD(angle));
    bool canMove = true;
    for (const auto& wall : walls) {
        if (wouldCollideAt(newX, newZ, wall.x, wall.z, wall.width, wall.depth)) {
            canMove = false;
            loseLife();
            break;
        }
    }
    if (canMove) {
        x = newX;
        z = newZ;
    }
}

void Player::moveBackward() {
    float speed = 0.1f;
    float newX = x - speed * sin(DEG2RAD(angle));
    float newZ = z - speed * cos(DEG2RAD(angle));
    bool canMove = true;
    for (const auto& wall : walls) {
        if (wouldCollideAt(newX, newZ, wall.x, wall.z, wall.width, wall.depth)) {
            canMove = false;
            loseLife();
            break;
        }
    }
    if (canMove) {
        x = newX;
        z = newZ;
    }
}

void Player::turnLeft() {
    // Rotate left only (no movement)
    float rotateSpeed = 5.0f;
    angle += rotateSpeed;
}

void Player::turnRight() {
    // Rotate right only (no movement)
    float rotateSpeed = 5.0f;
    angle -= rotateSpeed;
}

// --- Jumping & Gravity Logic [cite: 31] ---

void Player::jump() {
    // Only jump if we are currently on the ground
    if (!isJumping) {
        velocityY = 0.2f; // Initial jump force
        isJumping = true;
    }
}

void Player::updatePhysics() {
    // 1. Apply Gravity
    if (isJumping || y > 0.0f) {
        y += velocityY;       // Move player up/down
        velocityY -= 0.01f;   // Gravity pulls velocity down
    }

    // 2. Ground Collision Check
    if (y <= 0.0f) {
        y = 0.0f;             // Snap to floor
        isJumping = false;    // We landed
        velocityY = 0.0f;
    }
}

// --- Collision Logic (Circle-AABB) ---
// Returns TRUE if the player's bounding circle hits a bounding box (like a wall)
bool Player::checkCollision(float objX, float objZ, float objWidth, float objDepth) {
    return wouldCollideAt(x, z, objX, objZ, objWidth, objDepth);
}

// Check if a specific position would cause a collision
bool Player::wouldCollideAt(float testX, float testZ, float objX, float objZ, float objWidth, float objDepth) {
    // Player's bounding circle radius. Adjust if needed to fit your model.
    float playerRadius = player_radius;

    // Find the closest point on the wall's AABB to the center of the player's circle
    float closestX = std::max(objX - objWidth, std::min(testX, objX + objWidth));
    float closestZ = std::max(objZ - objDepth, std::min(testZ, objZ + objDepth));

    // Calculate the distance between the circle's center and this closest point
    float distanceX = testX - closestX;
    float distanceZ = testZ - closestZ;
    float distanceSquared = (distanceX * distanceX) + (distanceZ * distanceZ);

    // If the distance is less than or equal to the circle's radius, a collision occurs.
    // We use squared values to avoid a costly square root operation.
    // Using <= to include edge cases
    if (distanceSquared <= (playerRadius * playerRadius)) {
        return true;
    }

    return false;
}

void Player::addWall(float wallX, float wallZ, float wallWidth, float wallDepth) {
    walls.push_back({wallX, wallZ, wallWidth, wallDepth});
}

void Player::loseLife() {
    if (lives > 0) {
        lives--;
        // Optional: Reset position or bounce back slightly here
        // x = ...
        std::cout << "Ouch! Lives remaining: " << lives << std::endl;
    }
}