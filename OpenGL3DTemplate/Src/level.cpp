#include "level.h"
#include <glut.h>

Level::Level() {
    rotationAngle = 0.0f;
    hoverTime = 0.0f;
    // Set portal position at far end of Level 2
    portalX = 0.0f;
    portalZ = -10.0f; // Far end (opposite from spawn at z=8)
}

void Level::loadAssets() {
    // Level 1 Assets
    coinModel.Load((char*)"SilverDollarObverse.3ds");
    
    // Level 2 Assets
    vaseModel.Load((char*)"vase.3ds");
    vaseModel.scale = 0.1f;
    
    iceModel.Load((char*)"ice.3ds");
    iceModel.scale = 0.1f;
    
    iceWallModel.Load((char*)"ice.3ds");
    iceWallModel.scale = 0.3f;
    
    portalModel.Load((char*)"portal.3ds");
    portalModel.scale = 0.1f;
    
    // Place coins for Level 1
    for (int i = 0; i < 10; i++) {
        float zPosition = -(i * 2.0f);
        float xOffset = rand() % 5 - 2;
        coins.push_back({ xOffset, zPosition, true });
    }
    
    // Place multiple vases in Level 2
    addVase(-6.0f, -6.0f);
    addVase(6.0f, -6.0f);
    addVase(-6.0f, 6.0f);
    addVase(6.0f, 6.0f);
    addVase(-3.0f, -3.0f);
    addVase(3.0f, -3.0f);
    addVase(-3.0f, 3.0f);
    addVase(3.0f, 3.0f);
}

// Call this function from your main timer/idle loop
void Level::update(float deltaTime) {
    rotationAngle += 90.0f * deltaTime;
    if (rotationAngle > 360.0f) rotationAngle -= 360.0f;
    hoverTime += deltaTime * 2.0f;
}

void Level::draw() {
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
            float hoverHeight = 0.5f + (0.2f * sin(hoverTime));
            glTranslatef(c.x, hoverHeight, c.z);
            glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
            float scaleFactor = 0.06f;
            glScalef(scaleFactor, scaleFactor, scaleFactor);
            coinModel.Draw();
            glPopMatrix();
        }
    }
}

// Draw Level 2 specific objects INCLUDING ice walls
void Level::drawLevel2Objects() {
    glEnable(GL_COLOR_MATERIAL);
    
    // Draw all active vases
    for (Vase& v : vases) {
        if (v.isActive) {
            glPushMatrix();
            glTranslatef(v.x, 0.0f, v.z);
            glRotatef(rotationAngle * 0.5f, 0.0f, 1.0f, 0.0f);
            glScalef(0.1f, 0.1f, 0.1f);
            vaseModel.Draw();
            glPopMatrix();
        }
    }
    
    // Draw Ice blocks
    glPushMatrix();
    glTranslatef(3.0f, 0.0f, -3.0f);
    glScalef(0.15f, 0.15f, 0.15f);
    iceModel.Draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-3.0f, 0.0f, 3.0f);
    glScalef(0.15f, 0.15f, 0.15f);
    iceModel.Draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(3.0f, 0.0f, 3.0f);
    glScalef(0.15f, 0.15f, 0.15f);
    iceModel.Draw();
    glPopMatrix();
    
    // DRAW PORTAL AT FAR END (FIXED - NO ANIMATION)
    glPushMatrix();
    glTranslatef(portalX, 1.0f, portalZ); // No hover, no rotation
    glScalef(0.2f, 0.2f, 0.2f); // Larger for visibility
    portalModel.Draw();
    glPopMatrix();
}

void Level::addWall(float x, float z) {
    Wall w;
    w.x = x;
    w.z = z;
    w.width = 2.0f;
    w.depth = 2.0f;
    walls.push_back(w);
}

// Add vase helper function
void Level::addVase(float x, float z) {
    Vase v;
    v.x = x;
    v.z = z;
    v.isActive = true;
    vases.push_back(v);
}

int Level::checkCoinCollisions(float playerX, float playerZ, float playerRadius) {
    int collectedCount = 0;
    float coinRadius = 0.5f;
    float threshold = playerRadius + coinRadius;
    float thresholdSq = threshold * threshold;

    for (int i = 0; i < coins.size(); i++) {
        if (coins[i].isActive) {
            float dx = playerX - coins[i].x;
            float dz = playerZ - coins[i].z;
            float distSq = (dx * dx) + (dz * dz);

            if (distSq < thresholdSq) {
                coins[i].isActive = false;
                collectedCount++;
            }
        }
    }
    return collectedCount;
}

// Check vase collisions
int Level::checkVaseCollisions(float playerX, float playerZ, float playerRadius) {
    int collectedCount = 0;
    float vaseRadius = 0.5f;
    float threshold = playerRadius + vaseRadius;
    float thresholdSq = threshold * threshold;

    for (int i = 0; i < vases.size(); i++) {
        if (vases[i].isActive) {
            float dx = playerX - vases[i].x;
            float dz = playerZ - vases[i].z;
            float distSq = (dx * dx) + (dz * dz);

            if (distSq < thresholdSq) {
                vases[i].isActive = false;
                collectedCount++;
            }
        }
    }
    return collectedCount;
}

void Level::addCoin(float x, float z) {
    Coin c;
    c.x = x;
    c.z = z;
    c.isActive = true;
    coins.push_back(c);
}

bool Level::checkWallCollision(float playerX, float playerZ, float playerRadius) {
    for (Wall& w : walls) {
        bool collisionX = (playerX + playerRadius > w.x - w.width) &&
            (playerX - playerRadius < w.x + w.width);

        bool collisionZ = (playerZ + playerRadius > w.z - w.depth) &&
            (playerZ - playerRadius < w.z + w.depth);

        if (collisionX && collisionZ) {
            return true;
        }
    }
    return false;
}

// Get portal position
float Level::getPortalX() {
    return portalX;
}

float Level::getPortalZ() {
    return portalZ;
}



