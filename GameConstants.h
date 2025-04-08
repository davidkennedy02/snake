#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <raylib.h>

// Game dimensions and layout
extern const int cellSize;
extern const int screenWidth;
extern const int screenHeight;
extern const int borderSize;
extern const int windowWidth;
extern const int windowHeight;
extern const int rows;
extern const int cols;

// Game state variables
extern int score;
extern int highscore;
extern int frameCounter;
extern int moveDelay;
extern int minMoveDelay;
extern bool playedGameOverSound;

#endif // GAME_CONSTANTS_H
