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
#include "glut.h"         
#include "GameManager.h"   
#include "Include/Model_3DS.h"
#include "Include/Player.h" // Include the Player class

// --- Game Objects ---
Player player; // The object that will handle movement and collision logic
Model_3DS robocopModel;
Model_3DS wallModel;
Model_3DS doorModel;

// GLOBAL VARIABLES
GameManager game;
int currentLevel = 1; // Tracks if we are in Day (1) or Night (2)

// --- Obstacle Positions ---
// Array to hold the positions of our 3 wall obstacles
float wallPositions[3][2] = {
    {0.0f, -5.0f},  // Wall 1
    {-4.0f, 2.0f},   // Wall 2
    {4.0f, 2.0f}    // Wall 3
};


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera: High Angle "Top Down" view that follows the player
    gluLookAt(player.x, 15.0, player.z + 10, // Eye position (follows player)
              player.x, 0.0, player.z,      // Center position (looks at player)
              0.0, 1.0, 0.0);                // Up vector

    // 1. LIGHTING
    game.setupLights(currentLevel);

    // 2. DRAW PLAYER (at the Player object's position)
    glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glTranslatef(player.x, player.y, player.z);
    glRotatef(player.angle, 0.0f, 1.0f, 0.0f);
    robocopModel.Draw();
    glPopMatrix();

    // 3. DRAW OBSTACLE WALLS
    for (int i = 0; i < 3; ++i) {
        glPushMatrix();
        glTranslatef(wallPositions[i][0], 0.0f, wallPositions[i][1]);
        wallModel.Draw();
        glPopMatrix();
    }

    // 4. DRAW THE DOOR
    // Position the door behind the first wall (Wall 1 is at z = -5.0f)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -7.0f); // Behind the first wall at z = -5.0
    doorModel.Draw();
    glPopMatrix();

    // 5. DRAW HUD
    game.renderHUD();

    glutSwapBuffers();
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

void keyboard(unsigned char key, int x, int y) {
    // Store the player's position before any movement
    float oldX = player.x;
    float oldZ = player.z;

    switch (key) {
        // --- Player Movement ---
        case 'w': player.moveForward(); break;
        case 's': player.moveBackward(); break;
        case 'a': player.turnLeft(); break;
        case 'd': player.turnRight(); break;
        case ' ': player.jump(); break;

        // --- Game State Controls ---
        case '1': currentLevel = 1; std::cout << "Level 1: Day (Static Light)\n"; break;
        case '2': currentLevel = 2; std::cout << "Level 2: Night (Moving Light)\n"; break;
        case 'c': game.handleInteraction("coin"); std::cout << "Action: Collected Coin\n"; break;
        case 'v': game.handleInteraction("win"); std::cout << "Action: You Win!\n"; break;
        case 27: exit(0); break; // ESC to quit
    }

    // --- COLLISION DETECTION ---
    // After moving, check if the player has collided with any wall
    for (int i = 0; i < 3; ++i) {
        // checkCollision(wallX, wallZ, wallHalfWidth, wallHalfDepth)
        // We use 1.0f for half-width/depth as a generous bounding box for the wall model
        if (player.checkCollision(wallPositions[i][0], wallPositions[i][1], 1.0f, 1.0f)) {
            player.x = oldX; // Revert position to before the move
            player.z = oldZ;
            
            // This handles BOTH life deduction AND sound playback
            game.handleInteraction("wall"); 
            
            std::cout << "Collision with wall " << i + 1 << "!\n";
            break; // Exit loop after one collision
        }
    }

    glutPostRedisplay();
}

void idle() {
    player.updatePhysics(); // Apply gravity for jumping
    game.update();          // Update light animation
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Member 3 Final Test");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Grey background
    
    // --- LOAD ROBOCOP ---
    robocopModel.Load((char*)"robocop.3ds");
    robocopModel.scale = 0.1f; 

    // --- LOAD WALL ---
    wallModel.Load((char*)"wall.3ds"); 
    wallModel.scale = 0.05f; 
    wallModel.lit = false; 

    // --- LOAD DOOR ---
    doorModel.Load((char*)"door.3DS");
    doorModel.scale = 0.1f; // Increased scale to make it more visible

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    std::cout << "--- MEMBER 3 TEST CONTROLS ---\n";
    std::cout << "[W,A,S,D] to Move | [Space] to Jump\n";
    std::cout << "[1/2] Day/Night   [C] Coin   [V] Win\n";
    std::cout << "Navigate through the walls to reach the door!\n";

    glutMainLoop();
    return 0;
}