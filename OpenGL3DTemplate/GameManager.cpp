#include "GameManager.h"
#include <iostream>
#include <string>
#include <cmath> 

// Windows Audio Libraries
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

GameManager::GameManager() {
    score = 0;
    lives = 5;
    isGameOver = false;
    isGameWon = false;
    lightAngle = 0.0f;
}

// 1. LIGHTING LOGIC
void GameManager::setupLights(int levelNumber) {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // --- LEVEL 2: DARK NIGHT/CAVE WITH MOVING BABY BLUE POINT LIGHT ---
    if (levelNumber == 2) {
        // Point Light (w = 1.0f) - shines from a specific spot, creating a night/cave atmosphere
        glPushMatrix();
        
        // Animation: Sine wave translation (Left <-> Right) to simulate moving moon/mystical light
        float moveX = sin(lightAngle) * 8.0f; // Move 8 units left and right
        glTranslatef(moveX, 5.0f, 0.0f); // At height 5, moving horizontally
        
        GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Point light at current position
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        
        glPopMatrix();

        // Baby blue lighting - soft, light blue colors
        GLfloat ambient[] = { 0.3f, 0.5f, 0.7f, 1.0f }; // Baby blue ambient
        GLfloat diffuse[] = { 0.54f, 0.81f, 0.94f, 1.0f }; // Baby blue diffuse (main baby blue color)
        GLfloat specular[] = { 0.7f, 0.9f, 1.0f, 1.0f }; // Light blue specular highlights
        
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
        
        // Attenuation for point light (makes it fade with distance, enhancing the dark atmosphere)
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f);
    }
    // --- LEVEL 1: BRIGHT DAYLIGHT (Directional Sun) ---
    else {
        // Directional Light (w = 0.0f) - acts like the Sun with parallel rays
        // The direction vector points where the light is coming FROM
        GLfloat lightPos[] = { 1.0f, 1.0f, 0.5f, 0.0f }; // Sun coming from upper right
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        // Bright white daylight - high intensity
        GLfloat ambient[] = { 0.6f, 0.6f, 0.6f, 1.0f }; // Bright ambient (scattered sunlight)
        GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Full white diffuse (direct sunlight)
        GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // White specular (bright highlights)
        
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
        
        // Reset attenuation for directional light (no falloff for sun)
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
    }
}

// 2. LOGIC & SOUND (Jump Sound Removed as requested)
void GameManager::handleInteraction(std::string type) {
    if (isGameOver) return;

    if (type == "coin") {
        score += 10;
        PlaySoundA("Assets/Sounds/collect.wav", NULL, SND_ASYNC);
    }
    else if (type == "wall") {
        lives--;
        PlaySoundA("Assets/Sounds/thud.wav", NULL, SND_ASYNC);

        if (lives <= 0) {
            isGameOver = true;
            isGameWon = false;
            PlaySoundA("Assets/Sounds/lose.wav", NULL, SND_ASYNC);
        }
    }
    else if (type == "win") {
        isGameOver = true;
        isGameWon = true;
        PlaySoundA("Assets/Sounds/win.wav", NULL, SND_ASYNC);
    }
}

void GameManager::update() {
    // Animation Speed - smooth back and forth motion for Level 2 light
    lightAngle += 0.03f; // Adjust speed here (lower = slower)
    if (lightAngle > 6.28f) lightAngle = 0.0f; // Reset after full cycle (2*PI)
}

// 3. UI / HUD RENDERING
// Helper function to draw a heart shape
void GameManager::drawHeart(float x, float y, float size) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    
    // Draw heart shape using triangles and circles
    glBegin(GL_TRIANGLE_FAN);
    // Center bottom point of heart
    glVertex2f(0.0f, -size * 0.5f);
    
    // Right side of heart
    glVertex2f(size * 0.35f, 0.0f);
    glVertex2f(size * 0.5f, size * 0.2f);
    glVertex2f(size * 0.4f, size * 0.4f);
    glVertex2f(size * 0.2f, size * 0.5f);
    glVertex2f(0.0f, size * 0.3f);
    
    // Left side of heart
    glVertex2f(-size * 0.2f, size * 0.5f);
    glVertex2f(-size * 0.4f, size * 0.4f);
    glVertex2f(-size * 0.5f, size * 0.2f);
    glVertex2f(-size * 0.35f, 0.0f);
    glEnd();
    
    glPopMatrix();
}

void GameManager::renderHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Win/Lose Message - Display message and score in black when game is over
    if (isGameOver) {
        glColor3f(0.0f, 0.0f, 0.0f); // Black text
        
        std::string msg = isGameWon ? "YOU WIN!" : "GAME OVER";
        
        // Calculate approximate text width for centering the main message
        int textWidth = msg.length() * 15; // Rough estimate for TIMES_ROMAN_24
        int centerX = (800 - textWidth) / 2;
        
        // Display main message
        glRasterPos2i(centerX, 350);
        for (char c : msg) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
        
        // Display final score below the message (also in black)
        std::string scoreStr = "Final Score: " + std::to_string(score);
        int scoreTextWidth = scoreStr.length() * 9; // Rough estimate for HELVETICA_18
        int scoreCenterX = (800 - scoreTextWidth) / 2;
        
        glRasterPos2i(scoreCenterX, 280);
        for (char c : scoreStr) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    } else {
        // Display Score (Top Left - white text)
        glColor3f(1.0f, 1.0f, 1.0f);
        
        std::string scoreStr = "Score: " + std::to_string(score);

        glRasterPos2i(10, 580);
        for (char c : scoreStr) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        // Display Lives as Hearts (Top Right - red hearts)
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for hearts
        
        float heartSize = 15.0f;
        float heartSpacing = 25.0f;
        float startX = 700.0f;
        float startY = 575.0f;
        
        // Draw hearts for each life
        for (int i = 0; i < lives; i++) {
            drawHeart(startX + (i * heartSpacing), startY, heartSize);
        }
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

bool GameManager::isPlaying() {
    return !isGameOver;
}

bool GameManager::hasWon() {
    return isGameWon;
}

bool GameManager::hasLost() {
    return isGameOver && !isGameWon;
}