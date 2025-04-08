#include <raylib.h>
#include <iostream>
#include <deque>
#include <math.h>
#include <fstream>

const int cellSize = 45;  // Changed from 30 to 45 to match the new assets
const int screenWidth = 900;
const int screenHeight = 900;
const int borderSize = 50; // Border size
const int windowWidth = screenWidth + 2 * borderSize; // Total window width including borders
const int windowHeight = screenHeight + 2 * borderSize; // Total window height including borders
const int rows = screenHeight / cellSize;  // Now equals 20 (900/45)
const int cols = screenWidth / cellSize;   // Now equals 20 (900/45)

int score = 0; // Initialize score
int highscore = 0; // Initialize high score
int frameCounter = 0; // Counter for frames since last movement
int moveDelay = 15; // Initial delay - snake moves four times every 60 frames
int minMoveDelay = 5; // Minimum delay between movements
float musicTimer = 0.0f;

class Snake { 
    public:
        std::deque<Vector2> body = {Vector2{3, 3}, Vector2{2, 3}, Vector2{1, 3}}; // Snake body segments
        Vector2 direction = {1, 0}; // Initial direction (right)
        Vector2 nextDirection = {1, 0}; // Buffer for next direction
        bool isAlive = true; // Snake alive status
        Texture2D texture_head_up, texture_head_down, texture_head_left, texture_head_right;
        Texture2D texture_body_up, texture_body_down, texture_body_left, texture_body_right;
        Texture2D texture_tail_up, texture_tail_down, texture_tail_left, texture_tail_right;
        
        Snake() {
            Image head_up = LoadImage("snake_assets/head_up.png"); // Load head image
            Image head_down = LoadImage("snake_assets/head_down.png"); // Load head image
            Image head_left = LoadImage("snake_assets/head_left.png"); // Load head image
            Image head_right = LoadImage("snake_assets/head_right.png"); // Load head image
            Image body_up = LoadImage("snake_assets/body_up.png"); // Load body image
            Image body_down = LoadImage("snake_assets/body_down.png"); // Load body image
            Image body_left = LoadImage("snake_assets/body_left.png"); // Load body image
            Image body_right = LoadImage("snake_assets/body_right.png"); // Load body image
            Image tail_up = LoadImage("snake_assets/tail_up.png"); // Load tail image
            Image tail_down = LoadImage("snake_assets/tail_down.png"); // Load tail image
            Image tail_left = LoadImage("snake_assets/tail_left.png"); // Load tail image
            Image tail_right = LoadImage("snake_assets/tail_right.png"); // Load tail image

            texture_head_up = LoadTextureFromImage(head_up); // Load texture from image
            texture_head_down = LoadTextureFromImage(head_down); // Load texture from image
            texture_head_left = LoadTextureFromImage(head_left);
            texture_head_right = LoadTextureFromImage(head_right);
            texture_body_up = LoadTextureFromImage(body_up);
            texture_body_down = LoadTextureFromImage(body_down);
            texture_body_left = LoadTextureFromImage(body_left);
            texture_body_right = LoadTextureFromImage(body_right);
            texture_tail_up = LoadTextureFromImage(tail_up);
            texture_tail_down = LoadTextureFromImage(tail_down);
            texture_tail_left = LoadTextureFromImage(tail_left);
            texture_tail_right = LoadTextureFromImage(tail_right);

            UnloadImage(head_up); // Unload image to free memory
            UnloadImage(head_down); // Unload image to free memory
            UnloadImage(head_left); // Unload image to free memory
            UnloadImage(head_right); // Unload image to free memory
            UnloadImage(body_up); // Unload image to free memory
            UnloadImage(body_down); // Unload image to free memory
            UnloadImage(body_left);
            UnloadImage(body_right);
            UnloadImage(tail_up); // Unload image to free memory
            UnloadImage(tail_down); // Unload image to free memory
            UnloadImage(tail_left); // Unload image to free memory
            UnloadImage(tail_right); // Unload image to free memory
        }

        ~Snake() {
            UnloadTexture(texture_head_up); // Unload texture to free memory
            UnloadTexture(texture_head_down); // Unload texture to free memory
            UnloadTexture(texture_head_left); // Unload texture to free memory
            UnloadTexture(texture_head_right); // Unload texture to free memory
            UnloadTexture(texture_body_up); // Unload texture to free memory
            UnloadTexture(texture_body_down); // Unload texture to free memory
            UnloadTexture(texture_body_left); // Unload texture to free memory
            UnloadTexture(texture_body_right); // Unload texture to free memory
            UnloadTexture(texture_tail_up); // Unload texture to free memory
            UnloadTexture(texture_tail_down); // Unload texture to free memory
            UnloadTexture(texture_tail_left); // Unload texture to free memory
            UnloadTexture(texture_tail_right); // Unload texture to free memory
        }

        void UpdatePosition() { 
            // Apply the buffered direction
            direction = nextDirection;
            
            Vector2 newHead = body.front(); // Get the current head position
            newHead.x += direction.x; // Update head position based on direction
            newHead.y += direction.y;

            body.push_front(newHead); // Add new head to the front of the body
            body.pop_back(); // Remove the last segment of the snake
        }

        void CheckInput() { 
            if (IsKeyPressed(KEY_UP) && direction.y == 0) {
                nextDirection = {0, -1}; // Move up
            } else if (IsKeyPressed(KEY_DOWN) && direction.y == 0) {
                nextDirection = {0, 1}; // Move down
            } else if (IsKeyPressed(KEY_LEFT) && direction.x == 0) {
                nextDirection = {-1, 0}; // Move left
            } else if (IsKeyPressed(KEY_RIGHT) && direction.x == 0) {
                nextDirection = {1, 0}; // Move right
            }
        }

        bool HasCrashed() { 
            // Check if the snake has crashed into itself or the walls
            if (body.front().x < 0 || body.front().x >= cols || 
                body.front().y < 0 || body.front().y >= rows) {
                return true; // Snake has crashed into the wall
            }
            for (size_t i = 1; i < body.size(); ++i) {
                if (body.front().x == body[i].x && body.front().y == body[i].y) {
                    return true; // Snake has crashed into itself
                }
            }
            return false; // No crash detected
        }

        void Reset() { 
            body.clear(); // Clear the snake body
            body.push_back(Vector2{3, 3}); // Reset to initial position
            body.push_back(Vector2{2, 3});
            body.push_back(Vector2{1, 3});
            direction = {1, 0}; // Reset direction to right
            nextDirection = {1, 0}; // Reset next direction to right
            isAlive = true; // Set alive status to true
        }

        void Draw() { 
            if (isAlive) {
                CheckInput(); // Check for user input every frame
                
                // Only update position when frame counter reaches threshold
                if (frameCounter >= moveDelay) {
                    UpdatePosition(); // Update the snake's position
                    frameCounter = 0; // Reset frame counter
                }
            }
            
            for (size_t i = 0; i < body.size(); ++i) {
                Vector2 segment = body[i]; // Get the current segment position
                float x = segment.x * cellSize + borderSize; // Add border offset to x
                float y = segment.y * cellSize + borderSize; // Add border offset to y
                
                if (i == 0) {
                    // Draw the head based on the direction
                    if (direction.x == 1) DrawTexture(texture_head_right, x, y, WHITE); 
                    else if (direction.x == -1) DrawTexture(texture_head_left, x, y, WHITE); 
                    else if (direction.y == 1) DrawTexture(texture_head_down, x, y, WHITE); 
                    else if (direction.y == -1) DrawTexture(texture_head_up, x, y, WHITE); 
                } else if (i == body.size() - 1) { 
                    // Draw the tail based on the direction of the second last segment
                    if (body[i].x == body[i-1].x) {
                        if (body[i].y > body[i-1].y) DrawTexture(texture_tail_up, x, y, WHITE); // Draw tail up
                        else DrawTexture(texture_tail_down, x, y, WHITE); // Draw tail down
                    } else if (body[i].y == body[i-1].y) {
                        if (body[i].x > body[i-1].x) DrawTexture(texture_tail_left, x, y, WHITE); // Draw tail left
                        else DrawTexture(texture_tail_right, x, y, WHITE); // Draw tail right
                    }
                } else {
                    if (body[i].x == body[i-1].x) {
                        if (body[i].y > body[i-1].y) DrawTexture(texture_body_down, x, y, WHITE); // Draw body down
                        else DrawTexture(texture_body_up, x, y, WHITE); // Draw body up
                    } else if (body[i].y == body[i-1].y) {
                        if (body[i].x > body[i-1].x) DrawTexture(texture_body_right, x, y, WHITE); // Draw body right
                        else DrawTexture(texture_body_left, x, y, WHITE); // Draw body left
                    } 
                }
            }
        }
};


class Food { 
    public:
        Texture2D texture_apple; // Food texture
        float rotationAngle = 0.0f; // Rotation angle for animation

        Food() {
            Image apple = LoadImage("snake_assets/apple.png"); // Load apple image
            texture_apple = LoadTextureFromImage(apple); // Load texture from image
            UnloadImage(apple); // Unload image to free memory
        }

        ~Food() {
            UnloadTexture(texture_apple); // Unload texture to free memory
        }

        void spawn(Snake& snake) {
            // Check if the food spawns on the snake's body
            bool spawnOnSnake = false;
            do {
                spawnOnSnake = false; // Reset flag for each spawn attempt
                // Randomly generate food position
                position.x = GetRandomValue(0, cols-1); // Random x position
                position.y = GetRandomValue(0, rows-1); // Random y position

                // Check if the food spawns on the snake's body
                for (size_t i = 0; i < snake.body.size(); ++i) {
                    if (position.x == snake.body[i].x && position.y == snake.body[i].y) {
                        spawnOnSnake = true; // Food spawns on snake's body
                        break; // Exit loop
                    }
                }
            } while (spawnOnSnake); // Repeat until food spawns outside the snake's body
        }

        void CheckCollision(Snake& snake, Sound& eatSound) { 
            if (position.x == snake.body.front().x && position.y == snake.body.front().y) {
                PlaySound(eatSound); // Play eat sound
                snake.body.push_back(snake.body.back()); // Grow the snake
                score += 10; // Increase score
                
                // Increase speed by decreasing move delay
                if (moveDelay > minMoveDelay)
                    moveDelay -= 1;
                
                spawn(snake); // Respawn food
            }
        }

        void Update() {
            float time = GetTime(); // Get current time
            float rotationSpeed = 2.0f;
            float maxRotation = 15.0f;

            rotationAngle = sin(time * rotationSpeed) * maxRotation;
        }

        void Draw() {
            Update(); // Update rotation angle
            Vector2 origin = {
                texture_apple.width / 2.0f, 
                texture_apple.height / 2.0f
            }; // Set origin for rotation
            
            Rectangle sourceRec = {
                0, 
                0, 
                (float)texture_apple.width, 
                (float)texture_apple.height
            }; // Source rectangle for texture

            Rectangle destRec = {
                position.x * cellSize + borderSize + cellSize / 2, 
                position.y * cellSize + borderSize + cellSize / 2, 
                (float)texture_apple.width, 
                (float)texture_apple.height
            }; // Destination rectangle for texture
            
            DrawTexturePro(texture_apple, sourceRec, destRec, origin, rotationAngle, WHITE); // Draw food with rotation
        }
    
    private:
        Vector2 position; // Food position
};


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

// New function to display game over screen
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

// Modified function to display title screen
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

int main() {

    InitWindow(windowWidth, windowHeight, "Snake Game"); // Use the new window dimensions
    SetTargetFPS(60); // Set constant 60 FPS

    LoadHighscore(); // Load high score from file

    Image background = LoadImage("snake_assets/background.png"); // Load background image
    Texture2D texture_background = LoadTextureFromImage(background); // Load texture from image
    UnloadImage(background);

    InitAudioDevice(); // Initialize audio device
    Sound eatSound = LoadSound("sounds/eat_sound.mp3"); // Load eat sound
    Sound gameOverSound = LoadSound("sounds/game_over.mp3"); // Load game over sound
    SetSoundVolume(eatSound, 1.0f); // Set eat sound volume
    SetSoundVolume(gameOverSound, 1.0f); // Set game over sound volume

    Music music = LoadMusicStream("sounds/soundtrack.mp3"); // Load background music
    SetMusicVolume(music, 0.5f); // Set music volume

    Snake snake = Snake();
    Food food = Food(); 
    food.spawn(snake); // Spawn the food
    
    // Add a title screen flag
    bool inTitleScreen = true;
    
    // Main game loop
    while (!WindowShouldClose()) {
        
        UpdateMusicStream(music); // Update the music stream
        
        BeginDrawing(); // Always begin drawing at start of loop
        
        // Handle title screen
        if (inTitleScreen) {
            if (DisplayTitleScreen(texture_background)) {
                inTitleScreen = false; // Exit title screen and start the game
                PlayMusicStream(music); // Start playing music when game begins
            }
            EndDrawing(); // End drawing before continuing
            continue; // Skip rest of loop while in title screen
        }
        
        // Increment frame counter each frame
        frameCounter++;

        ClearBackground(WHITE);
        
        // Draw the game border
        DrawRectangle(borderSize, borderSize, screenWidth, screenHeight, WHITE);
        DrawTexture(texture_background, 0, 0, WHITE); // Draw background texture
        
        DisplayScores(); // Display the scores

        snake.Draw(); // Draw the snake

        food.CheckCollision(snake, eatSound); // Check for food collision
        food.Draw(); // Draw the food

        if (snake.HasCrashed()) { // Check for collisions
            if (snake.isAlive) {
                PlaySound(gameOverSound); // Play game over sound
                StopMusicStream(music); // Stop the music
            }
            snake.isAlive = false; 
            DisplayGameOverScreen(); // Use the new game over screen
        }

        if (!snake.isAlive && IsKeyPressed(KEY_R)) {
            snake.Reset(); // Reset the snake
            food.spawn(snake); // Respawn food
            score = 0; // Reset score
            moveDelay = 15; // Reset movement delay
            SeekMusicStream(music, 0.0f); // Restart the music
            PlayMusicStream(music); // Play background music
        }

        EndDrawing();
    }

    SaveHighscore(); // Save high score to file

    UnloadTexture(texture_background); // Unload background texture to free memory
    UnloadMusicStream(music); // Unload music stream to free memory
    UnloadSound(eatSound); // Unload sound to free memory
    UnloadSound(gameOverSound); // Unload sound to free memory
    CloseAudioDevice(); // Close audio device

    CloseWindow();
    return 0;
}