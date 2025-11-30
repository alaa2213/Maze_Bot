#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <string>
// Include glut/freeglut based on your setup
#include "glut.h"
#include <windows.h> // Required for PlaySound in Windows

class GameManager {
private:
    int score;
    int lives;
    bool isGameOver;
    bool isGameWon;

    // For Light Animation
    float lightAngle;

public:
    // Constructor
    GameManager();

    // Game Logic
    void update(); // Called every frame to animate lights
    void handleInteraction(std::string type); // Handles 'coin', 'wall', 'win'

    // Visuals & Effects
    void setupLights(int levelNumber); // Sets Day (Lvl 1) or Night (Lvl 2)
    void renderHUD(); // Draws Score, Lives, and Win/Loss messages

    // Getters if needed by other members
    bool isPlaying();
};

#endif