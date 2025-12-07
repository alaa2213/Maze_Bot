//#include <iostream>
//#include <glut.h>
//#include "Include/Player.h"
//#include "Include/Camera.h"
//
//// --- GLOBAL VARIABLES ---
//Player player;
//Camera camera;
//
//// Screen dimensions
//int screenWidth = 800;
//int screenHeight = 600;
//
//// --- TEST WALL DATA ---
//float wallX = 0.0f;
//float wallY = 0.5f;
//float wallZ = -5.0f;
//float wallWidth = 2.0f;
//float wallDepth = 1.0f;
//
//// --- INITIALIZATION ---
//void init() {
//    glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // Sky color
//    glEnable(GL_DEPTH_TEST);              // Enable 3D layering
//
//    // Lighting Setup
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_COLOR_MATERIAL);
//
//    GLfloat lightPos[] = { 5.0f, 10.0f, 5.0f, 1.0f };
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
//}
//
//// --- HELPER: Draw a simple grid floor ---
//void drawGround() {
//    glDisable(GL_LIGHTING);
//    glColor3f(0.3f, 0.3f, 0.3f);
//    glBegin(GL_LINES);
//    for (float i = -20; i <= 20; i += 1.0f) {
//        glVertex3f(i, 0, -20); glVertex3f(i, 0, 20);
//        glVertex3f(-20, 0, i); glVertex3f(20, 0, i);
//    }
//    glEnd();
//    glEnable(GL_LIGHTING);
//}
//
//// --- DISPLAY FUNCTION ---
//void display() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();
//
//    // 1. UPDATE CAMERA
//    camera.update(player.x, player.y, player.z, player.angle);
//
//    gluLookAt(camera.eyeX, camera.eyeY, camera.eyeZ,
//        camera.centerX, camera.centerY, camera.centerZ,
//        camera.upX, camera.upY, camera.upZ);
//
//    // 2. DRAW SCENE
//    drawGround();
//
//    // Draw the Test Wall (Red)
//    glPushMatrix();
//    glColor3f(0.8f, 0.2f, 0.2f);
//    glTranslatef(wallX, wallY, wallZ);
//    glScalef(wallWidth, 1.0f, wallDepth);
//    glutSolidCube(1.0);
//    glPopMatrix();
//
//    // 3. DRAW PLAYER (Green Robot)
//    // FIX APPLIED HERE: Only draw the body if we are NOT in First Person
//    if (!camera.isFirstPerson) {
//        glPushMatrix();
//        glColor3f(0.0f, 1.0f, 0.0f);
//        glTranslatef(player.x, player.y + 0.5f, player.z);
//        glRotatef(player.angle, 0.0f, 1.0f, 0.0f);
//
//        // Body
//        glutSolidCube(1.0);
//
//        // Eyes (to see rotation)
//        glTranslatef(0.0f, 0.2f, 0.4f);
//        glColor3f(0.0f, 0.0f, 0.0f);
//        glutSolidSphere(0.1, 10, 10);
//        glPopMatrix();
//    }
//
//    glutSwapBuffers();
//}
//
//// --- KEYBOARD HANDLING ---
//void keyboard(unsigned char key, int x, int y) {
//    float oldX = player.x;
//    float oldZ = player.z;
//
//    switch (key) {
//    case 'w': player.moveForward(); break;
//    case 's': player.moveBackward(); break;
//    case 'a': player.turnLeft(); break;
//    case 'd': player.turnRight(); break;
//    case ' ':
//    case 'j': player.jump(); break;
//    case 27:  exit(0); break;
//    }
//
//    // Collision Check
//    // Note: We use half-widths for the check
//    if (player.checkCollision(wallX, wallZ, wallWidth / 2.0f + 0.5f, wallDepth / 2.0f + 0.5f)) {
//        player.x = oldX;
//        player.z = oldZ;
//        player.loseLife();
//    }
//
//    glutPostRedisplay();
//}
//
//// --- SPECIAL KEYS (Arrows) ---
//void specialKeys(int key, int x, int y) {
//    if (key == GLUT_KEY_UP) {
//        camera.toggleMode();
//    }
//    glutPostRedisplay();
//}
//
//// --- GAME LOOP TIMER ---
//void timer(int val) {
//    player.updatePhysics();
//    glutPostRedisplay();
//    glutTimerFunc(16, timer, 0);
//}
//
//// --- RESHAPE ---
//void reshape(int w, int h) {
//    screenWidth = w; screenHeight = h;
//    glViewport(0, 0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(45.0, (double)w / h, 0.1, 100.0);
//    glMatrixMode(GL_MODELVIEW);
//}
//
//// --- MAIN ---
//int main(int argc, char** argv) {
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//    glutInitWindowSize(screenWidth, screenHeight);
//    glutCreateWindow("Member 1 Test: Fixed Camera");
//
//    init();
//
//    glutDisplayFunc(display);
//    glutReshapeFunc(reshape);
//    glutKeyboardFunc(keyboard);
//    glutSpecialFunc(specialKeys);
//    glutTimerFunc(0, timer, 0);
//
//    glutMainLoop();
//    return 0;
//}

//MEMBER 3 FINAL TEST CODE

#include <iostream>
#include <cmath>
#include <algorithm>
#include "glut.h"         
#include "GameManager.h"   
#include "Include/Model_3DS.h"
#include "Include/Player.h" // Include the Player class
#include "Include/Camera.h"
#include "Include/level.h"
 // Add this to the top of the file

// --- Game Objects ---
Player player; // The object that will handle movement and collision logic
Model_3DS robocopModel;
Camera camera;
Model_3DS wallModel;
Model_3DS doorModel;
Model_3DS coinModel;
Level level;
float player_radius = 0.5f;  // Default; update after model load
// GLOBAL VARIABLES
GameManager game;
int currentLevel = 1; // Tracks if we are in Day (1) or Night (2)
bool isInitialized = false; // Flag to ensure initialization is complete
float wall_half_width = 1.0f; // Half dimensions for collision
float wall_half_depth = 1.0f;
float wall_offset_x = 0.0f; // Offset to align collision box with model
float wall_offset_z = 0.0f;
float player_offset_x = 0.0f; // Offset to align player collision box
float player_offset_z = 0.0f;
// --- Obstacle Positions ---
// Simplified maze layout: Reduced walls for better performance
// Using minimal walls to create a simple but functional maze
const int NUM_WALLS = 8;
float wallPositions[NUM_WALLS][2] = {
    // Strategic inner maze walls - minimal for performance (8 walls total)
    // Creating a simple path with obstacles
    {-6.0f, -6.0f},      // Lower left barrier
    {6.0f, -4.0f},       // Lower right barrier
    {-4.0f, -2.0f},      // Mid left
    {4.0f, 0.0f},        // Mid right
    {-6.0f, 2.0f},       // Upper left
    {6.0f, 4.0f},        // Upper right
    {-4.0f, 6.0f},       // Near door left
    {4.0f, 8.0f}         // Near door right (leaves path to door at z=11)
};
void drawGround() {
    glDisable(GL_LIGHTING);
    glColor3f(0.6f, 0.65f, 0.7f); // Brighter grey ground

    // Draw a large ground plane
    glBegin(GL_QUADS);
    glVertex3f(-50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, 50.0f);
    glVertex3f(-50.0f, 0.0f, 50.0f);
    glEnd();

    // Optional: Draw grid lines for better visual reference (lighter color)
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    for (float i = -50; i <= 50; i += 2.0f) {
        glVertex3f(i, 0.01f, -50); glVertex3f(i, 0.01f, 50);
        glVertex3f(-50, 0.01f, i); glVertex3f(50, 0.01f, i);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

// Check if player would collide at a given position
bool wouldCollideWithWalls(float testX, float testZ) {
    if (!isInitialized || NUM_WALLS <= 0) return false;
    for (int i = 0; i < NUM_WALLS; ++i) {
        float effective_x = wallPositions[i][0] + wall_offset_x;
        float effective_z = wallPositions[i][1] + wall_offset_z;
        if (player.wouldCollideAt(testX + player_offset_x, testZ + player_offset_z,
            effective_x, effective_z,
            wall_half_width, wall_half_depth)) {
            return true;
        }
    }
    return false;
}

bool checkWallCollisions(float& playerX, float& playerZ, float oldX, float oldZ) {
    // First check if current position collides
    if (wouldCollideWithWalls(playerX, playerZ)) {
        playerX = oldX; // Revert position
        playerZ = oldZ;
        game.handleInteraction("wall");
        return true;
    }
    return false;
}
void display() {
    // Safety check: don't draw if not initialized
    if (!isInitialized) return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Load coins and walls
    camera.update(player.x, player.y, player.z, player.angle);

    gluLookAt(camera.eyeX, camera.eyeY, camera.eyeZ,
        camera.centerX, camera.centerY, camera.centerZ,
        camera.upX, camera.upY, camera.upZ);

    // 1. LIGHTING
    game.setupLights(currentLevel);


    // 2. DRAW GROUND
    drawGround();

    level.draw();

    // 2. DRAW PLAYER (at the Player object's position)
   /* glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glTranslatef(player.x, player.y, player.z);
    glRotatef(player.angle, 0.0f, 1.0f, 0.0f);
    robocopModel.Draw();
    glPopMatrix();*/


    // 3. DRAW OBSTACLE WALLS (Maze) - only if model loaded successfully
    // Optimized: Only draw walls near the player to improve performance
    if (wallModel.visible && wallModel.numObjects > 0) {
        const float DRAW_DISTANCE = 20.0f; // Only draw walls within this distance
        for (int i = 0; i < NUM_WALLS; ++i) {
            // Calculate distance from player to wall
            float dx = wallPositions[i][0] - player.x;
            float dz = wallPositions[i][1] - player.z;
            float distanceSquared = dx * dx + dz * dz;
            
            // Only draw if wall is within draw distance
            if (distanceSquared < DRAW_DISTANCE * DRAW_DISTANCE) {
                glPushMatrix();
                glTranslatef(wallPositions[i][0], 0.0f, wallPositions[i][1]);
                wallModel.Draw();
                glPopMatrix();
            }
        }
    }

    // 4. DRAW THE DOOR - at the end of the maze (north end)
    if (doorModel.visible && doorModel.numObjects > 0) {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 11.0f); // At the end of the maze (north end, z = 11)
        doorModel.Draw();
        glPopMatrix();
    }

    // Draw player - only if model loaded successfully
    if (!camera.isFirstPerson && robocopModel.visible && robocopModel.numObjects > 0) {
        glEnable(GL_COLOR_MATERIAL);
        glPushMatrix();
        glTranslatef(player.x, player.y, player.z);
        glRotatef(player.angle, 0.0f, 1.0f, 0.0f);
        robocopModel.Draw();
        glPopMatrix();
    }

    // 5. DRAW HUD
    game.renderHUD();

    glutSwapBuffers();
}
void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        camera.toggleMode();
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Standard 3D perspective
    gluPerspective(60.0, (float)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
void timer(int val) {
    player.updatePhysics();
    int coinsFound = level.checkCoinCollisions(player.x, player.z, 0.5f);
    level.update(0.016f);
    glutPostRedisplay();

    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    // Store the player's position before any movement
    float oldX = player.x;
    float oldZ = player.z;
    float oldAngle = player.angle;

    switch (key) {
        // --- Player Movement ---
    case 'w': {
        // Calculate where player would move to
        float speed = 0.1f;
        float newX = player.x + speed * sin(player.angle * 0.0174532925f);
        float newZ = player.z + speed * cos(player.angle * 0.0174532925f);
        // Only move if no collision
        if (!wouldCollideWithWalls(newX, newZ)) {
            player.x = newX;
            player.z = newZ;
        } else {
            game.handleInteraction("wall");
        }
        break;
    }
    case 's': {
        float speed = 0.1f;
        float newX = player.x - speed * sin(player.angle * 0.0174532925f);
        float newZ = player.z - speed * cos(player.angle * 0.0174532925f);
        if (!wouldCollideWithWalls(newX, newZ)) {
            player.x = newX;
            player.z = newZ;
        } else {
            game.handleInteraction("wall");
        }
        break;
    }
    case 'a': {
        // Just rotate left (no movement with rotation)
        player.turnLeft();
        break;
    }
    case 'd': {
        // Just rotate right (no movement with rotation)
        player.turnRight();
        break;
    }
    case 'j': player.jump(); break;

        // --- Game State Controls ---
    case '1': currentLevel = 1; std::cout << "Level 1: Day (1)\n"; break;
    case '2': currentLevel = 2; std::cout << "Level 2: Night (2)\n"; break;
    case 'c': game.handleInteraction("coin"); std::cout << "Action: Collected Coin\n"; break;
    case 'v': game.handleInteraction("win"); std::cout << "Action: You Win!\n"; break;
    case 27: exit(0); break; // ESC to quit
    }

    // --- COLLISION DETECTION (backup check) ---
    // After moving, check if the player has collided with any wall (safety net)
    // Removed as collision is now checked before movement

    glutPostRedisplay();
}

// --- HELPER: Check wall collisions and revert position if needed ---


// --- HELPER: Draw a simple ground plane ---


void idle() {
    // Safety check: don't run if not initialized
    if (!isInitialized) return;

    player.updatePhysics(); // Apply gravity for jumping

    // Continuous collision detection - only check if player is already inside a wall
    // This prevents interference with keyboard movement but catches physics-based issues
    // Only revert if player is deeply inside a wall (not just touching)
    //if (wouldCollideWithWalls(player.x, player.z)) {
    //    // Try to push player back to a safe position
    //    // Check positions around current position to find a safe spot
    //    float safeX = player.x;
    //    float safeZ = player.z;
    //    bool foundSafe = false;

    //    // Try moving back in small increments
    //    for (float offset = 0.1f; offset <= 1.0f && !foundSafe; offset += 0.1f) {
    //        if (!wouldCollideWithWalls(player.x - offset, player.z)) {
    //            safeX = player.x - offset;
    //            safeZ = player.z;
    //            foundSafe = true;
    //        }
    //        else if (!wouldCollideWithWalls(player.x + offset, player.z)) {
    //            safeX = player.x + offset;
    //            safeZ = player.z;
    //            foundSafe = true;
    //        }
    //        else if (!wouldCollideWithWalls(player.x, player.z - offset)) {
    //            safeX = player.x;
    //            safeZ = player.z - offset;
    //            foundSafe = true;
    //        }
    //        else if (!wouldCollideWithWalls(player.x, player.z + offset)) {
    //            safeX = player.x;
    //            safeZ = player.z + offset;
    //            foundSafe = true;
    //        }
    //    }

    //    if (foundSafe) {
    //        player.x = safeX;
    //        player.z = safeZ;
    //    }
    //}

    game.update();          // Update light animation
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Member 3 Final Test");
    glutSpecialFunc(specialKeys);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.7f, 0.8f, 0.9f, 1.0f); // Bright sky blue background

    level.loadAssets();

    // --- LOAD ROBOCOP ---
    //// Check if file exists before loading
    //FILE* testFile = fopen("robocop.3ds", "rb");
    //if (testFile) {
    //    fclose(testFile);
    robocopModel.Load((char*)"robocop.3ds");
    robocopModel.scale = 0.1f;
    robocopModel.visible = true;
    robocopModel.ComputeBounds();

    // Use max of XZ for conservative circle radius (ignore Y for XZ collision)
    float player_half_x = (robocopModel.max_x - robocopModel.min_x) / 2.0f * robocopModel.scale;
    float player_half_z = (robocopModel.max_z - robocopModel.min_z) / 2.0f * robocopModel.scale;
    float player_radius = max(player_half_x, player_half_z);  // Or average, but max prevents clipping
    float player_offset_x = (robocopModel.min_x + robocopModel.max_x) / 2.0f * robocopModel.scale;
    float player_offset_z = (robocopModel.min_z + robocopModel.max_z) / 2.0f * robocopModel.scale;
    std::cout << "Player radius (scaled): " << player_radius << std::endl;
    std::cout << "Loaded robocop.3ds successfully" << std::endl;
    /* } else {
         std::cout << "Warning: robocop.3ds not found, skipping load" << std::endl;
         robocopModel.visible = false;
     }*/

     // --- LOAD WALL ---
    // testFile = fopen("wall.3ds", "rb");
    /* if (testFile) {
         fclose(testFile);*/
    wallModel.Load((char*)"wall.3ds");
    wallModel.scale = 0.05f;
    wallModel.lit = false;
    wallModel.visible = true;
    wallModel.ComputeBounds();  // Compute unscaled bounds

    // Compute scaled half-extents and any center offsets (if model not centered at 0)
    float wall_half_width = (wallModel.max_x - wallModel.min_x) / 2.0f * wallModel.scale;
    float wall_half_depth = (wallModel.max_z - wallModel.min_z) / 2.0f * wallModel.scale;
    float wall_offset_x = (wallModel.min_x + wallModel.max_x) / 2.0f * wallModel.scale;  // Usually ~0 if centered
    float wall_offset_z = (wallModel.min_z + wallModel.max_z) / 2.0f * wallModel.scale;
    std::cout << "Wall bounds (scaled): Width=" << wall_half_width * 2 << ", Depth=" << wall_half_depth * 2 << std::endl;
    std::cout << "Loaded wall.3ds successfully" << std::endl;
    /*} else {
        std::cout << "Warning: wall.3ds not found, skipping load" << std::endl;
        wallModel.visible = false;
    }*/

    // --- LOAD DOOR ---
  /*  testFile = fopen("door.3DS", "rb");
    if (testFile) {
        fclose(testFile);*/
    doorModel.Load((char*)"door.3DS");
    doorModel.scale = 0.1f;
    doorModel.visible = true;
    std::cout << "Loaded door.3DS successfully" << std::endl;
    /* } else {
         std::cout << "Warning: door.3DS not found, skipping load" << std::endl;
         doorModel.visible = false;
     }*/

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // Mark initialization as complete
    isInitialized = true;

    glutIdleFunc(idle);
    glutTimerFunc(0, timer, 0);

    std::cout << "--- MEMBER 3 TEST CONTROLS ---\n";
    std::cout << "[W,A,S,D] to Move | [Space] to Jump\n";
    std::cout << "[1/2] Day/Night   [C] Coin   [V] Win\n";
    std::cout << "Navigate through the walls to reach the door!\n";

    glutMainLoop();
    return 0;
}