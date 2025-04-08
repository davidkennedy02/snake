#include <raylib.h>
#include <iostream>
#include <deque>
#include <math.h>
#include <fstream>
#include "GameConstants.h"
#include "Snake.h"
#include "Food.h"

// Define all the global constants
const int cellSize = 45;  // Changed from 30 to 45 to match the new assets
const int screenWidth = 900;
const int screenHeight = 900;
const int borderSize = 50; // Border size
const int windowWidth = screenWidth + 2 * borderSize; // Total window width including borders
const int windowHeight = screenHeight + 2 * borderSize; // Total window height including borders
const int rows = screenHeight / cellSize;  // Now equals 20 (900/45)
const int cols = screenWidth / cellSize;   // Now equals 20 (900/45)

// Define the global variables
int score = 0; // Initialize score
int highscore = 0; // Initialize high score
int frameCounter = 0; // Counter for frames since last movement
int moveDelay = 15; // Initial delay - snake moves four times every 60 frames
int minMoveDelay = 5; // Minimum delay between movements
bool playedGameOverSound = false; // Flag to check if game over sound has been played

bool LoadHighscore() {
    std::ifstream file("highscore.dat");
    if (!file.is_open()) {
        std::cout << "Highscore file not found, creating new file..." << std::endl;
        
        // Create the file with default highscore of 0
        std::ofstream newFile("highscore.dat");
        if (!newFile.is_open()) {
            std::cerr << "Failed to create highscore file." << std::endl;
            return false;
        }
        
        newFile << 0;
        newFile.close();
        
        // Set default highscore
        highscore = 0;
        return true;
    }

    int savedScore = 0;
    if (file >> savedScore) {
        if (savedScore >= 0) {
            highscore = savedScore; // Load high score from file
            file.close();
            return true;
        } 
    }

    file.close();
    
    // Recreate file with default value
    std::ofstream resetFile("highscore.dat");
    if (resetFile.is_open()) {
        resetFile << 0;
        resetFile.close();
        highscore = 0;
        return true;
    }
    
    return false;
}

bool SaveHighscore() {
    std::ofstream file("highscore.dat"); // Open file in truncate mode
    if (!file.is_open()) {
        std::cerr << "Error opening highscore file for writing." << std::endl;
        return false; // Failed to open file
    }

    file << highscore; // Save high score to file

    if (file.fail()) {
        std::cerr << "Error writing highscore to file." << std::endl;
        file.close();
        return false; // Failed to write high score
    }

    file.close();
    return true; // Successfully saved high score
}

void DisplayScores() { 
    // Define colors to match the game's aesthetic
    Color scoreColor = {30, 130, 30, 255}; // Green matching the snake theme
    Color shadowColor = {20, 70, 20, 200}; // Darker green for shadow
    Color panelColor = ColorAlpha(WHITE, 0.8f); // Semi-transparent white background
    
    // Create background panels for scores
    DrawRectangle(borderSize, borderSize / 4, 150, 30, panelColor);
    DrawRectangleLines(borderSize, borderSize / 4, 150, 30, scoreColor);
    
    DrawRectangle(windowWidth - borderSize - 200, borderSize / 4, 200, 30, panelColor);
    DrawRectangleLines(windowWidth - borderSize - 200, borderSize / 4, 200, 30, scoreColor);
    
    // Draw score with shadow effect
    DrawText(TextFormat("Score: %i", score), 
             borderSize + 5 + 1, borderSize / 4 + 8 + 1, 20, shadowColor);
    DrawText(TextFormat("Score: %i", score), 
             borderSize + 5, borderSize / 4 + 8, 20, scoreColor);
    
    // Draw high score with shadow effect
    const char* highScoreText = TextFormat("High Score: %i", highscore);
    DrawText(highScoreText, 
             windowWidth - borderSize - 195 + 1, 
             borderSize / 4 + 8 + 1, 20, shadowColor);
    DrawText(highScoreText, 
             windowWidth - borderSize - 195, 
             borderSize / 4 + 8, 20, scoreColor);
    
    // Update high score
    if (score > highscore) {
        highscore = score;
    }
}

void DisplayGameOverScreen() {
    static float textAlpha = 0.0f;
    static float animTimer = 0.0f;
    
    // Update animation
    animTimer += GetFrameTime();
    textAlpha = sin(animTimer * 2) * 0.5f + 0.5f; // Pulsating effect
    
    // Define colors to match title screen
    Color titleColor = {30, 130, 30, 255}; // Green title matching the snake theme
    Color shadowColor = {20, 70, 20, 200}; // Darker green for shadow
    
    // Draw semi-transparent panel background
    DrawRectangle(windowWidth/2 - 200, windowHeight/2 - 150, 400, 300, ColorAlpha(WHITE, 0.9f));
    DrawRectangleLines(windowWidth/2 - 200, windowHeight/2 - 150, 400, 300, titleColor);
    
    // Draw title with shadow
    const char* gameOverText = "GAME OVER";
    int titleFontSize = 60;
    int titleWidth = MeasureText(gameOverText, titleFontSize);
    
    // Draw shadow
    DrawText(gameOverText, windowWidth/2 - titleWidth/2 + 3, windowHeight/2 - 120 + 3, titleFontSize, shadowColor);
    // Draw main text
    DrawText(gameOverText, windowWidth/2 - titleWidth/2, windowHeight/2 - 120, titleFontSize, titleColor);
    
    // Draw score and highscore
    DrawText(TextFormat("Score: %i", score), 
             windowWidth/2 - 150, windowHeight/2 - 30, 30, titleColor);
    DrawText(TextFormat("High Score: %i", highscore), 
             windowWidth/2 - 150, windowHeight/2 + 10, 30, titleColor);
    
    // Draw "Press R to restart" with pulsating effect
    const char* restartText = "Press R to restart";
    int restartFontSize = 30;
    int restartWidth = MeasureText(restartText, restartFontSize);
    
    Color startColor = {40, 100, 40, (unsigned char)(textAlpha * 255)};
    DrawText(restartText, windowWidth/2 - restartWidth/2, windowHeight/2 + 80, 
             restartFontSize, startColor);
    
    // Draw decorative element
    DrawRectangle(windowWidth/2 - 150, windowHeight/2 + 130, 300, 3, titleColor);
}

bool DisplayTitleScreen(Texture2D& backgroundTexture) {
    static float titleAlpha = 0.0f;
    static bool fadeIn = true;
    static float animTimer = 0.0f;
    
    // Update animation
    animTimer += GetFrameTime();
    if (fadeIn) {
        titleAlpha = sin(animTimer * 2) * 0.5f + 0.5f; // Pulsating effect
    }
    
    // Draw the title screen (without BeginDrawing/EndDrawing)
    ClearBackground(WHITE);
    DrawTexture(backgroundTexture, 0, 0, WHITE); // Draw background
    
    // Title with shadow effect
    Color titleColor = {30, 130, 30, 255}; // Green title matching the snake theme
    Color shadowColor = {20, 70, 20, 200}; // Darker green for shadow
    
    // Draw title
    const char* titleText = "RETRO SNAKE";
    int fontSize = 80;
    int titleWidth = MeasureText(titleText, fontSize);
    
    // Draw shadow
    DrawText(titleText, windowWidth/2 - titleWidth/2 + 4, windowHeight/3 + 4, fontSize, shadowColor);
    // Draw main text
    DrawText(titleText, windowWidth/2 - titleWidth/2, windowHeight/3, fontSize, titleColor);
    
    // Draw "Press any key to start" with pulsating effect
    const char* startText = "PRESS ANY KEY TO START";
    int startFontSize = 30;
    int startTextWidth = MeasureText(startText, startFontSize);
    
    Color startColor = {40, 100, 40, (unsigned char)(titleAlpha * 255)};
    DrawText(startText, windowWidth/2 - startTextWidth/2, windowHeight/2 + 100, startFontSize, startColor);
    
    // Draw game decoration - snake silhouette
    DrawRectangle(windowWidth/2 - 100, windowHeight/2 + 180, 200, 20, titleColor);
    
    // Check for key press
    if (GetKeyPressed() != 0) {
        return true; // User pressed a key, exit title screen
    }
    
    return false; // Stay on title screen
}

void InitializeGame(Snake& snake, Food& food, Texture2D& backgroundTexture, 
                    Sound& eatSound, Sound& gameOverSound, Music& music) {
    LoadHighscore(); // Load high score from file

    // Load background
    Image background = LoadImage("snake_assets/background.png");
    backgroundTexture = LoadTextureFromImage(background);
    UnloadImage(background);

    // Initialize audio
    InitAudioDevice();
    eatSound = LoadSound("sounds/eat_sound.mp3");
    gameOverSound = LoadSound("sounds/game_over.mp3");
    SetSoundVolume(eatSound, 1.0f);
    SetSoundVolume(gameOverSound, 1.0f);
    
    music = LoadMusicStream("sounds/soundtrack.mp3");
    SetMusicVolume(music, 0.5f);
    
    // Initialize game objects
    food.spawn(snake);
}

void UpdateGame(Snake& snake, Food& food, Sound& eatSound, Sound& gameOverSound, 
                Music& music, bool& inTitleScreen) {
    // Update music stream
    UpdateMusicStream(music);
    
    // Skip the rest if in title screen
    if (inTitleScreen) {
        return;
    }
    
    // Increment frame counter
    frameCounter++;
    
    // Check for food collision
    food.CheckCollision(snake, eatSound);
    
    // Check for game over condition
    if (!snake.isAlive && !playedGameOverSound) {
        PlaySound(gameOverSound);
        StopMusicStream(music);
        playedGameOverSound = true;
    } 
    
    // Handle restart
    if (!snake.isAlive && IsKeyPressed(KEY_R)) {
        snake.Reset();
        food.spawn(snake);
        score = 0;
        moveDelay = 15;
        SeekMusicStream(music, 0.0f);
        PlayMusicStream(music);
        playedGameOverSound = false; // Reset game over sound flag
    }
}

void RenderGame(Snake& snake, Food& food, Texture2D& backgroundTexture, 
                bool& inTitleScreen, Music& music) {
    BeginDrawing();
    
    // Handle title screen
    if (inTitleScreen) {
        if (DisplayTitleScreen(backgroundTexture)) {
            inTitleScreen = false;
            PlayMusicStream(music);
        }
        EndDrawing();
        return;
    }
    
    // Draw game elements
    ClearBackground(WHITE);
    DrawRectangle(borderSize, borderSize, screenWidth, screenHeight, WHITE);
    DrawTexture(backgroundTexture, 0, 0, WHITE);
    
    DisplayScores();
    snake.Draw();
    food.Draw();
    
    // Show game over screen if needed
    if (!snake.isAlive) {
        DisplayGameOverScreen();
    }
    
    EndDrawing();
}

void CleanupGame(Texture2D& backgroundTexture, Sound& eatSound, 
                 Sound& gameOverSound, Music& music) {
    SaveHighscore();
    
    UnloadTexture(backgroundTexture);
    UnloadMusicStream(music);
    UnloadSound(eatSound);
    UnloadSound(gameOverSound);
    CloseAudioDevice();
    
    CloseWindow();
}

int main() {
    // Initialize window
    InitWindow(windowWidth, windowHeight, "Snake Game");
    SetTargetFPS(60);

    // Create game objects and resources
    Snake snake = Snake();
    Food food = Food();
    Texture2D texture_background;
    Sound eatSound;
    Sound gameOverSound;
    Music music;
    bool inTitleScreen = true;
    
    // Initialize game
    InitializeGame(snake, food, texture_background, eatSound, gameOverSound, music);
    
    // Main game loop
    while (!WindowShouldClose()) {
        UpdateGame(snake, food, eatSound, gameOverSound, music, inTitleScreen);
        RenderGame(snake, food, texture_background, inTitleScreen, music);
    }
    
    // Cleanup before exit
    CleanupGame(texture_background, eatSound, gameOverSound, music);
    
    return 0;
}