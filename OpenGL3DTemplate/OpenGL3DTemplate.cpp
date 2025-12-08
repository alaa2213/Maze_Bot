//#include <iostream>
//#include <cmath> // Added for distance calculations
//#include "glut.h"         
//#include "GameManager.h"   
//#include "Include/Model_3DS.h"
//#include "Include/Player.h" 
//#include "Include/Camera.h"
//#include "Include/level.h"
//
//// --- Game Objects ---
//Player player;
//Model_3DS robocopModel;
//Camera camera;
//Model_3DS wallModel;
//Model_3DS doorModel;
//Model_3DS coinModel; // Note: You might want to load a different model for Level 2 Portal if available
//Level level;
//
//// GLOBAL VARIABLES
//GameManager game;
//int currentLevel = 1;
//
//// --- Level 1 Obstacles ---
//float wallPositionsL1[5][2] = {
//    {0.0f, -5.0f},  {-4.0f, 2.0f},  {4.0f, 4.0f}, { 2.0f , 5.0f}, {6.0f , 7.0f}
//};
//
//// --- Level 2 Obstacles (New Layout) ---
//float wallPositionsL2[5][2] = {
//    {-5.0f, -5.0f}, {5.0f, -5.0f}, {0.0f, 0.0f}, {-5.0f, 5.0f}, {5.0f, 5.0f}
//};
//
//// --- Door/Target Locations ---
//// Level 1 Door Location
//float doorL1_X = 0.0f;
//float doorL1_Z = -10.0f; // Changed from -7.0f to -10.0f (3 units further)
//
//void display() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();
//
//    // UPDATE CAMERA
//    camera.update(player.x, player.y, player.z, player.angle);
//    gluLookAt(camera.eyeX, camera.eyeY, camera.eyeZ,
//        camera.centerX, camera.centerY, camera.centerZ,
//        camera.upX, camera.upY, camera.upZ);
//
//    // DRAW SCENE ENVIRONMENT (Floor/Sky)
//    level.draw();
//
//    // SETUP LIGHTS BASED ON LEVEL
//    game.setupLights(currentLevel);
//
//    // DRAW PLAYER 
//    // Only draw body if not in First Person
//    if (!camera.isFirstPerson) {
//        glEnable(GL_COLOR_MATERIAL);
//        glPushMatrix();
//        glTranslatef(player.x, player.y, player.z);
//        glRotatef(player.angle, 0.0f, 1.0f, 0.0f);
//        robocopModel.Draw();
//        glPopMatrix();
//    }
//
//    // --- LEVEL 1 RENDERING ---
//    if (currentLevel == 1) {
//        // Draw Level 1 Walls
//        for (int i = 0; i < 5; ++i) {
//            glPushMatrix();
//            glTranslatef(wallPositionsL1[i][0], 0.0f, wallPositionsL1[i][1]);
//            wallModel.Draw();
//            glPopMatrix();
//        }
//
//        // Draw Level 1 Door (The Target)
//        glPushMatrix();
//        glTranslatef(doorL1_X, 0.0f, doorL1_Z);
//        doorModel.Draw();
//        glPopMatrix();
//    }
//    // --- LEVEL 2 RENDERING ---
//    else if (currentLevel == 2) {
//        // Draw Level 2 Walls (Different Layout)
//        for (int i = 0; i < 5; ++i) {
//            glPushMatrix();
//            glTranslatef(wallPositionsL2[i][0], 0.0f, wallPositionsL2[i][1]);
//            // Optional: You could use a different texture or scale here for Ice Walls
//            glScalef(1.0f, 1.5f, 1.0f); // Make them taller for distinction
//            wallModel.Draw();
//            glPopMatrix();
//        }
//
//        // (Optional) Draw Level 2 Target/Portal here if you have one
//    }
//
//    // DRAW HUD
//    game.renderHUD();
//
//    glutSwapBuffers();
//}
//
//void specialKeys(int key, int x, int y) {
//    if (key == GLUT_KEY_UP) {
//        camera.toggleMode();
//    }
//    glutPostRedisplay();
//}
//
//void reshape(int w, int h) {
//    if (h == 0) h = 1;
//    glViewport(0, 0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(60.0, (float)w / h, 0.1, 100.0);
//    glMatrixMode(GL_MODELVIEW);
//}
//
//// --- UPDATED TIMER: HANDLES TRANSITION LOGIC ---
//void timer(int val) {
//    player.updatePhysics();
//    level.update(0.016f); // Update g lights/objects
//
//    // Check Coin Collection (delegated to Level class)
//    int coinsFound = level.checkCoinCollisions(player.x, player.z, 0.5f);
//
//    // --- LEVEL TRANSITION LOGIC ---
//    if (currentLevel == 1) {
//        // Check distance to Door (Simple box collision)
//        // Door is at (0, -7). We check if player is within 1.0 unit.
//        if (abs(player.x - doorL1_X) < 1.0f && abs(player.z - doorL1_Z) < 1.0f) {
//            std::cout << ">>> DOOR REACHED! TRANSITIONING TO LEVEL 2 <<<\n";
//
//            // 1. Change State
//            currentLevel = 2;
//
//            // 2. Play Sound
//            game.handleInteraction("win"); // Plays win sound as a transition effect
//
//            // 3. Reset Player Position (Spawn at start of Lvl 2)
//            player.x = 0.0f;
//            player.z = 8.0f; // Spawn them on the opposite side or safe area
//            player.angle = 180.0f; // Face the new maze
//        }
//    }
//
//    glutPostRedisplay();
//    glutTimerFunc(16, timer, 0);
//}
//
//void keyboard(unsigned char key, int x, int y) {
//    float oldX = player.x;
//    float oldZ = player.z;
//
//    switch (key) {
//    case 'w': player.moveForward(); break;
//    case 's': player.moveBackward(); break;
//    case 'a': player.turnLeft(); break;
//    case 'd': player.turnRight(); break;
//    case ' ': player.jump(); break;
//        // Debug keys
//    case '1': currentLevel = 1; break;
//    case '2': currentLevel = 2; break;
//    case 'c': game.handleInteraction("coin"); break;
//    case 27: exit(0); break;
//    }
//
//    // --- COLLISION DETECTION ---
//    bool hasCollided = false;
//
//    // Pointer to the current level's wall array
//    float (*currentWalls)[2] = (currentLevel == 1) ? wallPositionsL1 : wallPositionsL2;
//
//    // Fixed Loop: Check all 5 walls (User code had i < 3)
//    for (int i = 0; i < 5; ++i) {
//        // Check collision with the current level's walls
//        if (player.checkCollision(currentWalls[i][0], currentWalls[i][1], 1.0f, 1.0f)) {
//            hasCollided = true;
//            break;
//        }
//    }
//
//    if (hasCollided) {
//        player.x = oldX;
//        player.z = oldZ;
//        game.handleInteraction("wall"); // Sound + Life deduction
//    }
//
//    glutPostRedisplay();
//}
//
//void idle() {
//    player.updatePhysics();
//    game.update();
//    glutPostRedisplay();
//}
//
//int main(int argc, char** argv) {
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//    glutInitWindowSize(800, 600);
//    glutCreateWindow("MazeBot Final");
//    glutSpecialFunc(specialKeys);
//
//    glEnable(GL_DEPTH_TEST);
//    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//
//    level.loadAssets();
//
//    // Load Models
//    robocopModel.Load((char*)"robocop.3ds");
//    robocopModel.scale = 0.1f; // Fixed scale assignment
//
//    wallModel.Load((char*)"wall.3ds");
//    wallModel.scale = 0.05f;
//    wallModel.lit = false;
//
//    doorModel.Load((char*)"door.3DS");
//    doorModel.scale = 0.1f;
//
//    glutDisplayFunc(display);
//    glutReshapeFunc(reshape);
//    glutKeyboardFunc(keyboard);
//    glutIdleFunc(idle);
//    glutTimerFunc(0, timer, 0);
//
//    std::cout << "--- GAME START ---\n";
//    std::cout << "Reach the door to enter Level 2!\n";
//
//    glutMainLoop();
//    return 0;
//}


//WORKING FINAL CODE AFTER FIXES
//#include <iostream>
//#include <cmath>
//#include "glut.h"         
//#include "GameManager.h"   
//#include "Include/Model_3DS.h"
//#include "Include/Player.h" 
//#include "Include/Camera.h"
//#include "Include/level.h"
//
//// --- Game Objects ---
//Player player;
//Model_3DS robocopModel;
//Camera camera;
//Model_3DS wallModel;
//Model_3DS doorModel;
//Level level;
//GameManager game;
//
//// Global State
//int currentLevel = 1;
//float doorL1_X = 1.0f;
//float doorL1_Z = -12.0f;  // Changed from -7.0f to -12.0f (5 units further from the nearest wall)
//
//// --- OBSTACLE DEFINITIONS (GLOBAL) ---
//float wallPositionsL1[5][2] = {
//    {0.0f, -5.0f}, {-4.0f, 2.0f}, {4.0f, 4.0f}, {2.0f, 5.0f}, {6.0f, 7.0f}
//};
//
//float wallPositionsL2[5][2] = {
//    {-5.0f, -5.0f}, {5.0f, -5.0f}, {0.0f, 0.0f}, {-5.0f, 5.0f}, {5.0f, 5.0f}
//};
//
//void display() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();
//
//    // 1. Camera Logic
//    camera.update(player.x, player.y, player.z, player.angle);
//    gluLookAt(camera.eyeX, camera.eyeY, camera.eyeZ,
//        camera.centerX, camera.centerY, camera.centerZ,
//        camera.upX, camera.upY, camera.upZ);
//
//    // 2. Draw Shared Environment
//    level.draw();
//    game.setupLights(currentLevel);
//
//    // 3. Draw Player
//    if (!camera.isFirstPerson) {
//        glEnable(GL_COLOR_MATERIAL);
//        glPushMatrix();
//        glTranslatef(player.x, player.y, player.z);
//        glRotatef(player.angle, 0.0f, 1.0f, 0.0f);
//        robocopModel.Draw();
//        glPopMatrix();
//    }
//
//    // 4. Draw Level Specific Objects
//    if (currentLevel == 1) {
//        // Draw L1 Walls
//        for (int i = 0; i < 5; ++i) {
//            glPushMatrix();
//            glTranslatef(wallPositionsL1[i][0], 0.0f, wallPositionsL1[i][1]);
//            wallModel.Draw();
//            glPopMatrix();
//        }
//        // Draw Door
//        glPushMatrix();
//        glTranslatef(doorL1_X, 0.0f, doorL1_Z);
//        doorModel.Draw();
//        glPopMatrix();
//    }
//    else if (currentLevel == 2) {
//        // Draw L2 Walls
//        for (int i = 0; i < 5; ++i) {
//            glPushMatrix();
//            glTranslatef(wallPositionsL2[i][0], 0.0f, wallPositionsL2[i][1]);
//            glScalef(1.0f, 1.5f, 1.0f); // Taller ice walls
//            wallModel.Draw();
//            glPopMatrix();
//        }
//    }
//
//    game.renderHUD();
//    glutSwapBuffers();
//}
//
//// --- PHYSICS TIMER (Runs 60 times per second) ---
//void timer(int val) {
//    // 1. Move Player
//    player.updatePhysics();
//
//    // 2. Update Animations
//    level.update(0.016f);
//
//    // 3. Check Door Collision (Level 1 Exit)
//    if (currentLevel == 1) {
//        if (abs(player.x - doorL1_X) < 1.0f && abs(player.z - doorL1_Z) < 1.0f) {
//            std::cout << "Level 1 Complete!\n";
//            currentLevel = 2;
//
//            // TELEPORT PLAYER TO SAFE START ZONE
//            player.x = 0.0f;
//            player.z = 8.0f;
//            player.angle = 180.0f;
//            game.handleInteraction("win");
//        }
//    }
//
//    glutPostRedisplay();
//    glutTimerFunc(16, timer, 0);
//}
//
//void keyboard(unsigned char key, int x, int y) {
//    float oldX = player.x;
//    float oldZ = player.z;
//
//    switch (key) {
//    case 'w': player.moveForward(); break;
//    case 's': player.moveBackward(); break;
//    case 'a': player.turnLeft(); break;
//    case 'd': player.turnRight(); break;
//    case ' ': player.jump(); break;
//
//        // --- DEBUG KEYS (FIXED) ---
//    case '1':
//        currentLevel = 1;
//        player.x = 0; player.z = 0; // Reset to safe spot
//        break;
//    case '2':
//        currentLevel = 2;
//        player.x = 0; player.z = 8.0f; // Reset to safe spot (Avoids the 0,0 wall)
//        break;
//
//    case 27: exit(0); break;
//    }
//
//    // --- DYNAMIC COLLISION DETECTION ---
//    bool hasCollided = false;
//    float (*activeWalls)[2] = (currentLevel == 1) ? wallPositionsL1 : wallPositionsL2;
//
//    for (int i = 0; i < 5; ++i) {
//        // Check collision against the active level's walls
//        if (player.checkCollision(activeWalls[i][0], activeWalls[i][1], 1.0f, 1.0f)) {
//            hasCollided = true;
//            break;
//        }
//    }
//
//    if (hasCollided) {
//        player.x = oldX; // Revert movement
//        player.z = oldZ;
//        game.handleInteraction("wall");
//    }
//
//    glutPostRedisplay();
//}
//
//void specialKeys(int key, int x, int y) {
//    if (key == GLUT_KEY_UP) camera.toggleMode();
//    glutPostRedisplay();
//}
//
//void reshape(int w, int h) {
//    if (h == 0) h = 1;
//    glViewport(0, 0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(60.0, (float)w / h, 0.1, 100.0);
//    glMatrixMode(GL_MODELVIEW);
//}
//
//int main(int argc, char** argv) {
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//    glutInitWindowSize(800, 600);
//    glutCreateWindow("MazeBot Final Fixed");
//
//    glEnable(GL_DEPTH_TEST);
//    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//
//    level.loadAssets();
//    robocopModel.Load((char*)"robocop.3ds");
//    robocopModel.scale = 0.1f;
//    wallModel.Load((char*)"wall.3ds");
//    wallModel.scale = 0.05f;
//    doorModel.Load((char*)"door.3DS");
//    doorModel.scale = 0.1f;
//
//    glutDisplayFunc(display);
//    glutReshapeFunc(reshape);
//    glutKeyboardFunc(keyboard);
//    glutSpecialFunc(specialKeys);
//
//    // REMOVED: glutIdleFunc(idle); <-- This was likely causing the freeze
//    glutTimerFunc(0, timer, 0);
//
//    glutMainLoop();
//    return 0;
//}
#include <iostream>
#include <cmath>
#include "glut.h"         
#include "GameManager.h"   
#include "Include/Model_3DS.h"
#include "Include/Player.h" 
#include "Include/Camera.h"
#include "Include/level.h"

// --- Game Objects ---
Player player;
Model_3DS robocopModel;
Camera camera;
Model_3DS wallModel;
Model_3DS iceWallModel;
Model_3DS doorModel;
Level level;
GameManager game;

// Global State
int currentLevel = 1;
float doorL1_X = 1.0f;
float doorL1_Z = -12.0f;

// Win/Lose state variables
bool isRotatingToWin = false;
float targetWinAngle = 0.0f;
bool hasReachedPortal = false; // CHANGED: Track if player reached portal (not passed through)
bool gameLost = false;

// Add this global variable at the top of the file, near other global variables
int rotationFrames = 0; // Tracks the number of frames during rotation

// --- OBSTACLE DEFINITIONS ---
float wallPositionsL1[5][2] = {
    {0.0f, -5.0f}, {-4.0f, 2.0f}, {4.0f, 4.0f}, {2.0f, 5.0f}, {6.0f, 7.0f}
};

float wallPositionsL2[5][2] = {
    {-5.0f, -5.0f}, {5.0f, -5.0f}, {0.0f, 0.0f}, {-5.0f, 5.0f}, {5.0f, 5.0f}
};

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera.update(player.x, player.y, player.z, player.angle);
    gluLookAt(camera.eyeX, camera.eyeY, camera.eyeZ,
        camera.centerX, camera.centerY, camera.centerZ,
        camera.upX, camera.upY, camera.upZ);

    game.setupLights(currentLevel);

    if (currentLevel == 1) {
        level.draw();
    }

    if (!camera.isFirstPerson) {
        glEnable(GL_COLOR_MATERIAL);
        glPushMatrix();
        glTranslatef(player.x, player.y, player.z);
        glRotatef(player.angle, 0.0f, 1.0f, 0.0f);
        robocopModel.Draw();
        glPopMatrix();
    }

    if (currentLevel == 1) {
        for (int i = 0; i < 5; ++i) {
            glPushMatrix();
            glTranslatef(wallPositionsL1[i][0], 0.0f, wallPositionsL1[i][1]);
            wallModel.Draw();
            glPopMatrix();
        }
        glPushMatrix();
        glTranslatef(doorL1_X, 0.0f, doorL1_Z);
        doorModel.Draw();
        glPopMatrix();
    }
    else if (currentLevel == 2) {
        for (int i = 0; i < 5; ++i) {
            glPushMatrix();
            glTranslatef(wallPositionsL2[i][0], 0.0f, wallPositionsL2[i][1]);
            glScalef(0.3f, 0.8f, 0.3f);
            iceWallModel.Draw();
            glPopMatrix();
        }
        level.drawLevel2Objects();
    }

    game.renderHUD();
    glutSwapBuffers();
}

void timer(int val) {
    // Check for game loss condition
    if (!gameLost && !player.hasLives()) {
        std::cout << "***** GAME OVER - NO LIVES LEFT! *****\n";
        gameLost = true;
        player.freezeMovement();
        game.handleInteraction("wall");
    }
    
    // Handle 360-degree rotation before win
    if (isRotatingToWin) {
        std::cout << "Rotating: Current angle = " << player.angle << ", Target = " << targetWinAngle << std::endl; // DEBUG
        
        // Rotate at constant speed (6 degrees per frame = 360 degrees in 60 frames = 1 second)
        player.angle += 6.0f;
        
        // Keep angle in 0-360 range
        if (player.angle >= 360.0f) {
            player.angle -= 360.0f;
        }
        
        rotationFrames++;
        
        std::cout << "Rotation frames: " << rotationFrames << " / 60" << std::endl; // DEBUG
        
        // Complete rotation after 60 frames (1 full 360-degree spin)
        if (rotationFrames >= 60) {
            std::cout << "***** PORTAL REACHED! YOU WIN! *****\n";
            game.handleInteraction("win");
            player.freezeMovement();
            isRotatingToWin = false;
            rotationFrames = 0;
        }
    }
    
    // Only update physics if game is still playing
    if (game.isPlaying() && !gameLost) {
        player.updatePhysics();
    }
    
    level.update(0.016f);
    game.update();

    // Check vase collisions in Level 2
    if (currentLevel == 2 && game.isPlaying() && !gameLost) {
        int vasesCollected = level.checkVaseCollisions(player.x, player.z, 0.5f);
        if (vasesCollected > 0) {
            game.handleInteraction("coin");
        }
        
        float portalX = level.getPortalX();
        float portalZ = level.getPortalZ();
        
        float dx = player.x - portalX;
        float dz = player.z - portalZ;
        float distance = sqrt((dx * dx) + (dz * dz));
        
        float winRadius = 2.5f;
        
        if (distance < winRadius && !hasReachedPortal && !isRotatingToWin) {
            std::cout << ">>> Player reached the portal! Starting 360 spin! <<<\n";
            hasReachedPortal = true;
            isRotatingToWin = true;
            rotationFrames = 0; // Reset frame counter
        }
        
        if (distance < 5.0f) {
            std::cout << "Distance to portal: " << distance << " (need < " << winRadius << ")" << std::endl;
        }
    }

    // Level 1 door collision
    if (currentLevel == 1 && !gameLost) {
        if (abs(player.x - doorL1_X) < 1.0f && abs(player.z - doorL1_Z) < 1.0f) {
            std::cout << "Level 1 Complete! Entering Level 2...\n";
            currentLevel = 2;
            glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
            player.x = 0.0f;
            player.z = 8.0f;
            player.angle = 180.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    // Don't allow movement if game is lost
    if (gameLost) {
        if (key == 27) exit(0);
        return;
    }
    
    float oldX = player.x;
    float oldZ = player.z;

    switch (key) {
    case 'w': player.moveForward(); break;
    case 's': player.moveBackward(); break;
    case 'a': player.turnLeft(); break;
    case 'd': player.turnRight(); break;
    case ' ': player.jump(); break;
    case 'j': 
    case 'J': 
        player.jump(); 
        break;

    case '1':
        currentLevel = 1;
        player.x = 0; player.z = 0;
        player.canMove = true;
        player.lives = 5;
        gameLost = false;
        hasReachedPortal = false;
        isRotatingToWin = false;
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        break;
    case '2':
        currentLevel = 2;
        player.x = 0; player.z = 8.0f;
        player.canMove = true;
        player.lives = 5;
        gameLost = false;
        hasReachedPortal = false;
        isRotatingToWin = false;
        glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
        break;

    case 27: exit(0); break;
    }

    // Collision detection
    bool hasCollided = false;
    float (*activeWalls)[2] = (currentLevel == 1) ? wallPositionsL1 : wallPositionsL2;
    float wallWidth = (currentLevel == 2) ? 1.5f : 1.0f;
    float wallDepth = (currentLevel == 2) ? 1.5f : 1.0f;

    for (int i = 0; i < 5; ++i) {
        if (player.checkCollision(activeWalls[i][0], activeWalls[i][1], wallWidth, wallDepth)) {
            hasCollided = true;
            break;
        }
    }

    if (hasCollided) {
        player.x = oldX;
        player.z = oldZ;
        game.handleInteraction("wall");
        std::cout << "COLLISION! " << (currentLevel == 2 ? "Ice" : "Wall") 
                  << " hit. Lives: " << player.lives << std::endl;
    }

    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) camera.toggleMode();
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("MazeBot Final Fixed");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    level.loadAssets();
    robocopModel.Load((char*)"robocop.3ds");
    robocopModel.scale = 0.1f;
    wallModel.Load((char*)"wall.3ds");
    wallModel.scale = 0.05f;
    iceWallModel.Load((char*)"ice.3ds");
    iceWallModel.scale = 0.3f;
    doorModel.Load((char*)"door.3DS");
    doorModel.scale = 0.1f;

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}