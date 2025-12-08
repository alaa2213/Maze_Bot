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
    movingLightPos = 0.0f;
}

// 1. LIGHTING LOGIC
void GameManager::setupLights(int levelNumber) {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // --- LEVEL 2: DARK NIGHT WITH MOVING BLUE POINT LIGHT ---
    if (levelNumber == 2) {
        // Point Light (w = 1.0f) means it shines from a specific spot
        glPushMatrix();
        
        // Animation: Sine wave translation (Left <-> Right)
        float moveX = sin(lightAngle) * 8.0f; // Move 8 units left and right
        glTranslatef(moveX, 5.0f, 0.0f); // At height 5, moving horizontally
        
        GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Point light at current position
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        
        glPopMatrix();

        // Color: Dim Blue Night/Cave Light
        GLfloat ambient[] = { 0.05f, 0.05f, 0.15f, 1.0f }; // Very dark blue ambient
        GLfloat diffuse[] = { 0.2f, 0.3f, 0.8f, 1.0f }; // Blue-tinted diffuse
        GLfloat specular[] = { 0.3f, 0.4f, 1.0f, 1.0f }; // Blue specular highlights
        
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
        
        // Attenuation for point light (makes it fade with distance)
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f);
    }
    // --- LEVEL 1: BRIGHT DAYLIGHT (Directional Sun) ---
    else {
        // Directional Light (w = 0.0f) means it acts like the Sun (rays are parallel)
        GLfloat lightPos[] = { 1.0f, 1.0f, 0.5f, 0.0f }; // Direction vector
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        // Color: Bright White Daylight
        GLfloat ambient[] = { 0.6f, 0.6f, 0.6f, 1.0f }; // Bright ambient
        GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Full white diffuse
        GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // White specular
        
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
        
        // Reset attenuation for directional light
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
    }
}

// 2. LOGIC & SOUND
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
    // Animation Speed - smooth back and forth motion
    lightAngle += 0.03f; // Adjust speed here (lower = slower)
    if (lightAngle > 6.28f) lightAngle = 0.0f; // Reset after full cycle (2*PI)
}

// 3. UI / HUD RENDERING
void GameManager::renderHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);

    std::string scoreStr = "Score: " + std::to_string(score);
    std::string livesStr = "Lives: " + std::to_string(lives);

    // Score (Top Left)
    glRasterPos2i(10, 580);
    for (char c : scoreStr) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    // Lives (Top Right)
    glRasterPos2i(700, 580);
    for (char c : livesStr) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    // Win/Lose Message
    if (isGameOver) {
        std::string msg = isGameWon ? "YOU WIN!" : "GAME OVER";
        glRasterPos2i(350, 300);
        for (char c : msg) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

bool GameManager::isPlaying() {
    return !isGameOver;
}