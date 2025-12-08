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

// Level 2 Maze Layout - WIDER SPACING for better navigation
// Player spawns at (0, 10) and must navigate to portal at (0, -14)
// Increased spacing between walls from 2 units to 4+ units
const int NUM_WALLS_L2 = 16;
float wallPositionsL2[NUM_WALLS_L2][2] = {
    // Outer boundary walls - wider perimeter
    {-10.0f, -12.0f},  // Bottom-left corner
    {10.0f, -12.0f},   // Bottom-right corner
    {-10.0f, 8.0f},    // Top-left
    {10.0f, 8.0f},     // Top-right
    
    // Left corridor walls - better spacing
    {-7.0f, -8.0f},
    {-7.0f, 0.0f},
    {-7.0f, 6.0f},
    
    // Right corridor walls - better spacing
    {7.0f, -8.0f},
    {7.0f, 0.0f},
    {7.0f, 6.0f},
    
    // Strategic center obstacles - wider gaps
    {-3.0f, -4.0f},
    {3.0f, -4.0f},
    {0.0f, 2.0f},
    
    // Portal area guards - with clear path
    {-4.0f, -10.0f},
    {4.0f, -10.0f},
    
    // Upper barrier
    {0.0f, 4.5f}
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
        // Draw rotating door using level.drawDoor
        level.drawDoor(doorModel, doorL1_X, 0.0f, doorL1_Z);
    }
    else if (currentLevel == 2) {
        for (int i = 0; i < NUM_WALLS_L2; ++i) {
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

    // Check coin collisions in Level 1 and update score
    if (currentLevel == 1 && game.isPlaying() && !gameLost) {
        int coinsCollected = level.checkCoinCollisions(player.x, player.z, 0.5f);
        if (coinsCollected > 0) {
            for (int i = 0; i < coinsCollected; i++) {
                game.handleInteraction("coin");
            }
            std::cout << "Collected " << coinsCollected << " coin(s)! Score updated." << std::endl;
        }
    }

    // Check vase collisions in Level 2 and update score
    if (currentLevel == 2 && game.isPlaying() && !gameLost) {
        int vasesCollected = level.checkVaseCollisions(player.x, player.z, 0.5f);
        if (vasesCollected > 0) {
            for (int i = 0; i < vasesCollected; i++) {
                game.handleInteraction("coin");
            }
            std::cout << "Collected " << vasesCollected << " vase(s)! Score updated." << std::endl;
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
            glClearColor(0.1f, 0.15f, 0.25f, 1.0f); // Darker blue-ish for snowy night
            player.x = 0.0f;
            player.z = 10.0f; // Updated spawn for wider maze
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
        player.x = 0; player.z = 10.0f; // Updated spawn for wider maze
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
    int numWalls = (currentLevel == 1) ? 5 : NUM_WALLS_L2;
    float wallWidth = (currentLevel == 2) ? 1.5f : 1.0f;
    float wallDepth = (currentLevel == 2) ? 1.5f : 1.0f;

    for (int i = 0; i < numWalls; ++i) {
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
    switch (key) {
    case GLUT_KEY_UP:
        // UP arrow = First Person
        camera.setFirstPerson();
        std::cout << "Camera Mode: First Person" << std::endl;
        break;
    case GLUT_KEY_DOWN:
        // DOWN arrow = Third Person
        camera.setThirdPerson();
        std::cout << "Camera Mode: Third Person" << std::endl;
        break;
    }
    glutPostRedisplay();
}

// Mouse click callback
void mouse(int button, int state, int x, int y) {
    // Don't allow actions if game is lost
    if (gameLost) return;
    
    // Check for left mouse button click (press)
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        player.jump();
        std::cout << "Mouse Jump!" << std::endl;
    }
    
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
    glutMouseFunc(mouse);  // Register mouse callback
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}