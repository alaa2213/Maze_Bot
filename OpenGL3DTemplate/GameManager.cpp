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

    glPushMatrix();

    // --- LEVEL 2: ANIMATION (Point Light Moving) ---
    if (levelNumber == 2) {
        // Point Light (w = 1.0f) means it shines from a specific spot
        GLfloat lightPos[] = { 0.0f, 10.0f, 0.0f, 1.0f };

        // Animation: Sine wave translation (Left <-> Right)
        float moveX = sin(lightAngle) * 10.0f;
        glTranslatef(moveX, 0.0f, 0.0f);

        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        // Color: Dim Blue Cave Light
        GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
        GLfloat diffuse[] = { 0.0f, 0.0f, 0.8f, 1.0f };
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    }
    // --- LEVEL 1: STATIC (Directional Sun) ---
    else {
        // Directional Light (w = 0.0f) means it acts like the Sun (rays are parallel)
        // Position doesn't matter as much as direction, but we define it anyway
        GLfloat lightPos[] = { 0.0f, 10.0f, 0.0f, 0.0f };

        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        // Color: Bright White Daylight
        GLfloat ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    }

    glPopMatrix();
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
    // Animation Speed
    lightAngle += 0.05f;
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