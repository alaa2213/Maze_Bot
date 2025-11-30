#include <iostream>
#include <glut.h>  // Make sure your Project Properties point to this
#include "Include/Player.h"

#include "Include/Camera.h"

// --- GLOBAL VARIABLES ---
Player player;
Camera camera;

// Screen dimensions
int screenWidth = 800;
int screenHeight = 600;

// --- TEST WALL DATA ---
// We place a wall at (0, 0, -5) to test collision
float wallX = 0.0f;
float wallY = 0.5f; // Center height
float wallZ = -5.0f;
float wallWidth = 2.0f; // Total width (logic uses half-width)
float wallDepth = 1.0f; // Total depth

// --- INITIALIZATION ---
void init() {
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // Light blue sky color
    glEnable(GL_DEPTH_TEST);              // Enable Z-buffer (3D layering)

    // Basic Lighting so 3D shapes look 3D
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL); // Let us use glColor3f with lighting

    // Light position (Sun)
    GLfloat lightPos[] = { 5.0f, 10.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

// --- HELPER: Draw a simple grid floor ---
void drawGround() {
    glDisable(GL_LIGHTING); // Turn off light for lines
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    for (float i = -20; i <= 20; i += 1.0f) {
        glVertex3f(i, 0, -20); glVertex3f(i, 0, 20);
        glVertex3f(-20, 0, i); glVertex3f(20, 0, i);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

// --- DISPLAY FUNCTION (Called every frame) ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 1. UPDATE CAMERA
    // Pass the player's info to the camera so it knows where to look
    camera.update(player.x, player.y, player.z, player.angle);

    // Apply the view matrix
    gluLookAt(camera.eyeX, camera.eyeY, camera.eyeZ,
        camera.centerX, camera.centerY, camera.centerZ,
        camera.upX, camera.upY, camera.upZ);

    // 2. DRAW SCENE
    drawGround();

    // Draw the Test Wall (Red)
    glPushMatrix();
    glColor3f(0.8f, 0.2f, 0.2f);
    glTranslatef(wallX, wallY, wallZ);
    glScalef(wallWidth, 1.0f, wallDepth); // Scale cube to look like a wall
    glutSolidCube(1.0);
    glPopMatrix();

    // 3. DRAW PLAYER (Green Robot)
    glPushMatrix();
    glColor3f(0.0f, 1.0f, 0.0f);
    glTranslatef(player.x, player.y + 0.5f, player.z); // +0.5 to sit on floor
    glRotatef(player.angle, 0.0f, 1.0f, 0.0f);         // Rotate player

    // Body
    glutSolidCube(1.0);

    // Little "Eyes" to indicate direction
    glTranslatef(0.0f, 0.2f, 0.4f); // Move forward/up
    glColor3f(0.0f, 0.0f, 0.0f);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    glutSwapBuffers();
}

// --- KEYBOARD HANDLING ---
void keyboard(unsigned char key, int x, int y) {
    // Save previous position to restore if we hit a wall
    float oldX = player.x;
    float oldZ = player.z;

    // Movement Inputs
    switch (key) {
    case 'w': player.moveForward(); break;
    case 's': player.moveBackward(); break;
    case 'a': player.turnLeft(); break;
    case 'd': player.turnRight(); break;
    case ' ': // Spacebar
    case 'j': player.jump(); break;
    case 27:  exit(0); break; // ESC to quit
    }

    // --- COLLISION CHECK (Crucial for Member 1) ---
    // If we moved into the wall, move back!
    // Note: wallWidth/2 because your class likely expects "half-width" or radius
    if (player.checkCollision(wallX, wallZ, wallWidth / 2.0f + 0.5f, wallDepth / 2.0f + 0.5f)) {
        player.x = oldX;
        player.z = oldZ;
        player.loseLife(); // Test life deduction
    }

    glutPostRedisplay();
}

// --- SPECIAL KEYS (Arrows) ---
void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        camera.toggleMode();
    }
    glutPostRedisplay();
}

// --- GAME LOOP TIMER ---
void timer(int val) {
    // Apply gravity every 16ms (approx 60 FPS)
    player.updatePhysics();

    glutPostRedisplay(); // Request redraw
    glutTimerFunc(16, timer, 0);
}

// --- RESHAPE (Handle window resizing) ---
void reshape(int w, int h) {
    screenWidth = w; screenHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// --- MAIN ---
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Member 1 Test: Physics & Camera");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys); // For Arrow keys
    glutTimerFunc(0, timer, 0);   // Start physics loop

    glutMainLoop();
    return 0;
}






//MEMBER 3 FINAL TEST CODE

#include <iostream>
#include "glut.h"         
#include "GameManager.h"   

// GLOBAL VARIABLES
GameManager game;
int currentLevel = 1; // Tracks if we are in Day (1) or Night (2)

// A simple object to test lighting on
void drawTestObject() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);

    // --- NEW: Make it SHINY ---
    // This adds a white "hotspot" reflection so you can see the light move
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    // ---------------------------

    glColor3f(1.0f, 0.0f, 0.0f); // Red Sphere
    glutSolidSphere(1.0, 50, 50);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera Position: 5 units back, looking at center
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // 1. SETUP LIGHTS based on the current level variable
    game.setupLights(currentLevel);

    // Enable Color Material so the sphere stays Red even with lights on
    glEnable(GL_COLOR_MATERIAL);

    // Draw the 3D Sphere
    drawTestObject();

    // 2. DRAW HUD (Score/Lives) on top
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
    switch (key) {
        // --- LIGHTING CONTROLS ---
    case '1':
        currentLevel = 1;
        std::cout << "Level 1: Day (Static Light)\n";
        break;
    case '2':
        currentLevel = 2;
        std::cout << "Level 2: Night (Moving Light)\n";
        break;

        // --- AUDIO & LOGIC CONTROLS ---
    case 'c': // Coin
        game.handleInteraction("coin");
        std::cout << "Action: Collected Coin\n";
        break;
    case 'w': // Wall
        game.handleInteraction("wall");
        std::cout << "Action: Hit Wall\n";
        break;
    case 'v': // Win
        game.handleInteraction("win");
        std::cout << "Action: You Win!\n";
        break;

    case 27: exit(0); break; // ESC to quit
    }
    glutPostRedisplay();
}

void idle() {
    // Update the light animation angle
    game.update();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Member 3 Final Test");

    glEnable(GL_DEPTH_TEST);

    // GREY BACKGROUND (So you can see the sphere clearly)
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    std::cout << "--- MEMBER 3 TEST CONTROLS ---\n";
    std::cout << "[1] Switch to Level 1 (Day)\n";
    std::cout << "[2] Switch to Level 2 (Night + Moving Light)\n";
    std::cout << "[C] Collect Coin\n";
    std::cout << "[W] Hit Wall\n";
    std::cout << "------------------------------\n";

    glutMainLoop();
    return 0;
}