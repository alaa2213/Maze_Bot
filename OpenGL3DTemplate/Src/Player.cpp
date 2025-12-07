#include "Include/Player.h"
#include <cmath> // Required for sin() and cos()
#include <iostream>
#include <algorithm> // Required for std::max and std::min

#define DEG2RAD(a) (a * 0.0174532925)

// Constructor: Set starting values
Player::Player() {
    x = 0.0f;
    y = 0.0f;     // Start on the ground
    z = 0.0f;
    angle = 0.0f; // Facing "North" initially

    velocityY = 0.0f;
    isJumping = false;
    lives = 5; // [cite: 10] Player starts with 5 lives
}

// --- Movement Logic [cite: 30] ---

void Player::moveForward() {
    // Move "Forward" relative to where the player is facing
    // We update x and z based on the angle
    float speed = 0.1f;
    x += speed * sin(DEG2RAD(angle));
    z += speed * cos(DEG2RAD(angle));
}

void Player::moveBackward() {
    float speed = 0.1f;
    x -= speed * sin(DEG2RAD(angle));
    z -= speed * cos(DEG2RAD(angle));
}

void Player::turnLeft() {
    // Rotate left
    float rotateSpeed = 5.0f;
    angle += rotateSpeed;

    // Move forward in the new direction
    float speed = 0.1f;
    x += speed * sin(DEG2RAD(angle));
    z += speed * cos(DEG2RAD(angle));
}


void Player::turnRight() {
    // Rotate right
    float rotateSpeed = 5.0f;
    angle -= rotateSpeed;

    // Move forward in the new direction
    float speed = 0.1f;
    x += speed * sin(DEG2RAD(angle));
    z += speed * cos(DEG2RAD(angle));
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
    // Player's bounding circle radius. Adjust if needed to fit your model.
    float playerRadius = 0.5f;

    // Find the closest point on the wall's AABB to the center of the player's circle
    float closestX = std::max(objX - objWidth, std::min(x, objX + objWidth));
    float closestZ = std::max(objZ - objDepth, std::min(z, objZ + objDepth));

    // Calculate the distance between the circle's center and this closest point
    float distanceX = x - closestX;
    float distanceZ = z - closestZ;
    float distanceSquared = (distanceX * distanceX) + (distanceZ * distanceZ);

    // If the distance is less than the circle's radius, a collision occurs.
    // We use squared values to avoid a costly square root operation.
    if (distanceSquared < (playerRadius * playerRadius)) {
        // [cite: 39] Collision detected
        return true;
    }

    return false;
}

void Player::loseLife() {
    if (lives > 0) {
        lives--;
        // Optional: Reset position or bounce back slightly here
        // x = ...
        std::cout << "Ouch! Lives remaining: " << lives << std::endl;
    }
}