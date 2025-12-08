#include "Include/level.h"
#include <glut.h>

Level::Level() {
    rotationAngle = 0.0f;
    hoverTime = 0.0f;
}

void Level::loadAssets() {
    // Make sure path is correct as discussed before!
    coinModel.Load((char*)"SilverDollarObverse.3ds");
     // <--- NEW
    /*for (int i = 0; i < 5; i++) {
        addWall(-2.0f, -5.0f - (i * 2.0f));
    }*/
    for (int i = 0; i < 10; i++) {
        // This places coins at Z = 0, -2, -4, -6...
        float zPosition = -(i * 2.0f);
		float xOffset = rand() % 5 - 2; // Random X between -2 and +2
        coins.push_back({ xOffset, zPosition, true });
    } // Far Left
}

// Call this function from your main timer/idle loop
void Level::update(float deltaTime) {
    // Update "Hover Time" (for scaling and bobbing effect)
    hoverTime += deltaTime * 2.0f; // Speed of animation
    
    // Update door rotation angle
    rotationAngle += 45.0f * deltaTime; // Rotate 45 degrees per second
    if (rotationAngle > 360.0f) rotationAngle -= 360.0f;
}

void Level::draw() {
    // Set Gold Material Properties so they look shiny
    GLfloat mat_ambient[] = { 0.24f, 0.19f, 0.07f, 1.0f };
    GLfloat mat_diffuse[] = { 0.75f, 0.60f, 0.22f, 1.0f };
    GLfloat mat_specular[] = { 0.62f, 0.55f, 0.36f, 1.0f };
    GLfloat mat_shininess[] = { 51.2f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    for (Coin& c : coins) {
        if (c.isActive) {
            glPushMatrix();
            // --- 1. Position & Floating Effect ---
            // We add sin(hoverTime) to Y to make it go up and down
            float hoverHeight = 0.5f + (0.2f * sin(hoverTime));
            glTranslatef(c.x, hoverHeight, c.z);

            // 2. Scale up and down using sine wave
            // Scale oscillates between 0.04f and 0.08f (base 0.06f ± 0.02f)
            float scaleVariation = 0.02f * sin(hoverTime);
            float scaleFactor = 0.06f + scaleVariation;
            glScalef(scaleFactor, scaleFactor, scaleFactor);

            // --- 3. Draw ---
            coinModel.Draw();
            glPopMatrix();
        }
    }
    //for (Wall& w : walls) {
    //    glPushMatrix();
    //    glTranslatef(w.x, 0.0f, w.z); // Walls sit on the ground (Y=0)

    //    // Adjust Scale if your wall model is too big/small
    //    glScalef(1.0f, 1.0f, 1.0f);
    //    glScalef(0.02f, 0.02f, 0.02f);
    //    glScalef(w.width, 1.0f, w.depth);
    //    wallModel.Draw();
    //    glPopMatrix();
    //}
}

void Level::drawDoor(Model_3DS& doorModel, float x, float y, float z) {
    if (doorModel.visible && doorModel.numObjects > 0) {
        glPushMatrix();
        glTranslatef(x, y, z);
        
        // Rotate the door around Y axis (vertical rotation)
        glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
        
        doorModel.Draw();
        glPopMatrix();
    }
}

void Level::addWall(float x, float z) {
    Wall w;
    w.x = x;
    w.z = z;
    // ADJUST THESE NUMBERS based on your actual 3D model size!
    w.width = 2.0f; // <--- Try doubling this (was likely 1.0f or 0.5f)
    w.depth = 2.0f; // Half-depth
    walls.push_back(w);
}
int Level::checkCoinCollisions(float playerX, float playerZ, float playerRadius) {
    int collectedCount = 0;
    float coinRadius = 0.5f; // Approx size of the coin
    float threshold = playerRadius + coinRadius;
    float thresholdSq = threshold * threshold; // Squared to avoid sqrt()

    for (int i = 0; i < coins.size(); i++) {
        // Only check coins that are still visible
        if (coins[i].isActive) {

            // Calculate distance squared
            float dx = playerX - coins[i].x;
            float dz = playerZ - coins[i].z;
            float distSq = (dx * dx) + (dz * dz);

            // Check collision
            if (distSq < thresholdSq) {
                coins[i].isActive = false; // Hide the coin!
                collectedCount++;
                 
            }
        }
    }
    return collectedCount;
}


void Level::addCoin(float x, float z) {
    Coin c;
    c.x = x;
	
   // Floating slightly above ground
    c.z = z;
    c.isActive = true;
    coins.push_back(c);
}
bool Level::checkWallCollision(float playerX, float playerZ, float playerRadius) {
    for (Wall& w : walls) {
        // AABB Collision (Axis-Aligned Bounding Box)
        // Check if Player is inside the Wall's box
        bool collisionX = (playerX + playerRadius > w.x - w.width) &&
            (playerX - playerRadius < w.x + w.width);

        bool collisionZ = (playerZ + playerRadius > w.z - w.depth) &&
            (playerZ - playerRadius < w.z + w.depth);

        if (collisionX && collisionZ) {
            return true; // HIT!
        }
    }
    return false; // No hit
}



