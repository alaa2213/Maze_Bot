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