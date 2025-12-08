#include "Include/level.h"
#include <glut.h>

Level::Level() {
    rotationAngle = 0.0f;
    hoverTime = 0.0f;
    // Set portal position at far end of Level 2
    portalX = 0.0f;
    portalZ = -14.0f; // Further back for wider maze
    
    // Initialize snow particles for Level 2
    initSnow();
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
    portalModel.scale = 0.05f;
    
    // Place coins for Level 1 - optimized for WIDER maze with 10 walls
    // Player spawns at (0, 10) and door is at (1, -12)
    
    // Starting area - wide open
    addCoin(0.0f, 9.0f);      // Center at spawn
    addCoin(-5.0f, 8.0f);     // Left side
    addCoin(5.0f, 8.0f);      // Right side
    
    // Upper section - wide corridors
    addCoin(-6.0f, 4.5f);     // Left corridor
    addCoin(0.0f, 5.0f);      // Center path
    addCoin(6.0f, 4.5f);      // Right corridor
    
    // Middle section - open center
    addCoin(-5.0f, 0.0f);     // Left middle
    addCoin(0.0f, 0.0f);      // Center middle
    addCoin(5.0f, 0.0f);      // Right middle
    
    // Lower section - wide approach to door
    addCoin(-6.0f, -4.5f);    // Lower left
    addCoin(0.0f, -5.0f);     // Center lower
    addCoin(6.0f, -4.5f);     // Lower right
    
    // Final approach to door
    addCoin(0.0f, -8.0f);     // Near door center
    addCoin(-2.0f, -10.0f);   // Left of door
    addCoin(2.0f, -10.0f);    // Right of door
    
    // Place vases in Level 2 - in open areas away from walls for better visibility
    // Positioned strategically in safe, visible zones for WIDER maze
    addVase(-5.0f, 7.0f);    // Top left - safe zone
    addVase(5.0f, 7.0f);     // Top right - safe zone
    addVase(-2.0f, 5.0f);    // Upper middle left
    addVase(2.0f, 5.0f);     // Upper middle right
    addVase(-5.0f, 1.0f);    // Middle left corridor
    addVase(5.0f, 1.0f);     // Middle right corridor
    addVase(-2.0f, -2.0f);   // Lower middle left
    addVase(2.0f, -2.0f);    // Lower middle right
    addVase(-5.0f, -6.0f);   // Bottom left
    addVase(5.0f, -6.0f);    // Bottom right
    addVase(0.0f, 3.0f);     // Center open area
    addVase(-3.0f, -11.0f);  // Near portal left
    addVase(3.0f, -11.0f);   // Near portal right
    addVase(0.0f, -3.0f);    // Center lower area
}

// Call this function from your main timer/idle loop
void Level::update(float deltaTime) {
    // Update rotation angle for door and portal
    rotationAngle += 45.0f * deltaTime; // 45 degrees per second
    if (rotationAngle > 360.0f) rotationAngle -= 360.0f;
    
    // Update hover time for scaling effect
    hoverTime += deltaTime * 2.0f;
    
    // Update snow particles
    updateSnow(deltaTime);
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

    // Draw Level 1 ground (grass/outdoor theme)
    drawGroundLevel1();

    // Draw coins with enhanced visibility - larger size, higher hover, and glow effect
    for (Coin& c : coins) {
        if (c.isActive) {
            glPushMatrix();
            
            // Position with more prominent hover effect
            float hoverHeight = 0.7f + (0.3f * sin(hoverTime)); // Increased from 0.5 + 0.2
            glTranslatef(c.x, hoverHeight, c.z);
            
            // Larger scale for better visibility
            float scaleVariation = 0.03f * sin(hoverTime); // Increased from 0.02
            float scaleFactor = 0.09f + scaleVariation; // Increased from 0.06
            glScalef(scaleFactor, scaleFactor, scaleFactor);
            
            // Enhanced golden material for better visibility
            GLfloat coinMat_ambient[] = { 0.4f, 0.35f, 0.1f, 1.0f };
            GLfloat coinMat_diffuse[] = { 1.0f, 0.9f, 0.3f, 1.0f }; // Brighter gold
            GLfloat coinMat_specular[] = { 1.0f, 1.0f, 0.8f, 1.0f };
            GLfloat coinMat_shininess[] = { 80.0f };
            
            glMaterialfv(GL_FRONT, GL_AMBIENT, coinMat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, coinMat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, coinMat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, coinMat_shininess);
            
            coinModel.Draw();
            glPopMatrix();
        }
    }
}

// Draw Level 1 ground - grass/outdoor theme
void Level::drawGroundLevel1() {
    glDisable(GL_LIGHTING);
    
    // Draw a textured ground plane (greenish for grass)
    glColor3f(0.2f, 0.6f, 0.2f); // Green grass color
    glBegin(GL_QUADS);
    glVertex3f(-50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, 50.0f);
    glVertex3f(-50.0f, 0.0f, 50.0f);
    glEnd();
    
    // Draw grid lines for better depth perception
    glColor3f(0.15f, 0.5f, 0.15f); // Darker green for grid
    glBegin(GL_LINES);
    for (float i = -50; i <= 50; i += 2.0f) {
        glVertex3f(i, 0.01f, -50); 
        glVertex3f(i, 0.01f, 50);
        glVertex3f(-50, 0.01f, i); 
        glVertex3f(50, 0.01f, i);
    }
    glEnd();
    
    glEnable(GL_LIGHTING);
}

// Draw Level 2 ground - ice/cave theme
void Level::drawGroundLevel2() {
    glDisable(GL_LIGHTING);
    
    // Draw an icy ground plane (brighter bluish-white for fresh snow/ice)
    glColor3f(0.85f, 0.92f, 0.98f); // Brighter snow-white with blue tint
    glBegin(GL_QUADS);
    glVertex3f(-50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, 50.0f);
    glVertex3f(-50.0f, 0.0f, 50.0f);
    glEnd();
    
    // Draw grid lines with icy crystalline blue color
    glColor3f(0.6f, 0.75f, 0.9f); // Crystalline ice blue for grid
    glBegin(GL_LINES);
    for (float i = -50; i <= 50; i += 2.0f) {
        glVertex3f(i, 0.01f, -50); 
        glVertex3f(i, 0.01f, 50);
        glVertex3f(-50, 0.01f, i); 
        glVertex3f(50, 0.01f, i);
    }
    glEnd();
    
    glEnable(GL_LIGHTING);
}

// Draw Level 2 specific objects INCLUDING ice walls
void Level::drawLevel2Objects() {
    glEnable(GL_COLOR_MATERIAL);
    
    // Draw Level 2 ground first
    drawGroundLevel2();
    
    // Draw falling snow for atmospheric effect
    drawSnow();
    
    // Draw all active vases with scaling AND hover animation for better visibility
    for (Vase& v : vases) {
        if (v.isActive) {
            glPushMatrix();
            
            // Add hover effect like coins for better visibility
            float hoverHeight = 0.5f + (0.15f * sin(hoverTime + 0.5f)); // Slightly different phase
            glTranslatef(v.x, hoverHeight, v.z);
            
            // Scale up and down using sine wave (slightly different phase than coins)
            float scaleVariation = 0.03f * sin(hoverTime + 1.0f); // Larger variation for visibility
            float scaleFactor = 0.12f + scaleVariation; // Larger base scale
            glScalef(scaleFactor, scaleFactor, scaleFactor);
            
            vaseModel.Draw();
            glPopMatrix();
        }
    }
    
    // Draw decorative Ice blocks (non-collidable) - more spread out
    glPushMatrix();
    glTranslatef(5.0f, 0.0f, -6.0f);
    glScalef(0.15f, 0.15f, 0.15f);
    iceModel.Draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-5.0f, 0.0f, 4.0f);
    glScalef(0.15f, 0.15f, 0.15f);
    iceModel.Draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(5.5f, 0.0f, 5.0f);
    glScalef(0.15f, 0.15f, 0.15f);
    iceModel.Draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-5.5f, 0.0f, -2.0f);
    glScalef(0.15f, 0.15f, 0.15f);
    iceModel.Draw();
    glPopMatrix();
    
    // DRAW PORTAL WITH ROTATION and proper texture settings
    glPushMatrix();
    
    // Enable texturing for the portal
    glEnable(GL_TEXTURE_2D);
    
    // Set material properties for better texture visibility
    GLfloat portalMat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat portalMat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat portalMat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat portalMat_shininess[] = { 32.0f };
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, portalMat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, portalMat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, portalMat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, portalMat_shininess);
    
    glTranslatef(portalX, 1.0f, portalZ);
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis
    glScalef(0.15f, 0.15f, 0.15f); // Updated scale for better texture display
    
    portalModel.Draw();
    
    glPopMatrix();
}

// Draw door with rotation
void Level::drawDoor(Model_3DS& doorModel, float x, float y, float z) {
    if (doorModel.visible && doorModel.numObjects > 0) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis
        doorModel.Draw();
        glPopMatrix();
    }
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

// Initialize snow particles for Level 2
void Level::initSnow() {
    snowflakes.clear();
    // Create 200 snowflakes
    for (int i = 0; i < 200; i++) {
        Snowflake snow;
        // Random position across the level
        snow.x = -30.0f + (rand() % 60);
        snow.y = (rand() % 30);
        snow.z = -30.0f + (rand() % 60);
        // Random fall speed
        snow.speed = 0.5f + (rand() % 100) / 200.0f;
        // Random size
        snow.size = 0.05f + (rand() % 50) / 1000.0f;
        snowflakes.push_back(snow);
    }
}

// Update snow particle positions
void Level::updateSnow(float deltaTime) {
    for (Snowflake& snow : snowflakes) {
        // Make snow fall down
        snow.y -= snow.speed * deltaTime * 10.0f;
        
        // Add gentle horizontal drift for realism
        snow.x += sin(snow.y * 0.5f) * deltaTime * 0.5f;
        
        // Reset snowflake to top when it reaches ground
        if (snow.y < 0.0f) {
            snow.y = 30.0f;
            snow.x = -30.0f + (rand() % 60);
            snow.z = -30.0f + (rand() % 60);
        }
    }
}

// Draw falling snow particles
void Level::drawSnow() {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f); // White with slight transparency
    
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (const Snowflake& snow : snowflakes) {
        glVertex3f(snow.x, snow.y, snow.z);
    }
    glEnd();
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}









