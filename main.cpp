#include <raylib.h>
#include <iostream>
#include <deque>

const int cellSize = 30;
const int screenWidth = 900;
const int screenHeight = 900;
const int borderSize = 50; // Border size
const int windowWidth = screenWidth + 2 * borderSize; // Total window width including borders
const int windowHeight = screenHeight + 2 * borderSize; // Total window height including borders
const int rows = screenHeight / cellSize;
const int cols = screenWidth / cellSize;

int score = 0; // Initialize score
int highscore = 0; // Initialize high score
int current_FPS = 5; // Initialize current FPS
int max_FPS = 20; // Initialize max FPS
float musicTimer = 0.0f;

class Snake { 
    public:
        std::deque<Vector2> body = {Vector2{3, 3}, Vector2{2, 3}, Vector2{1, 3}}; // Snake body segments
        Vector2 direction = {1, 0}; // Initial direction (right)
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
            Vector2 newHead = body.front(); // Get the current head position
            newHead.x += direction.x; // Update head position based on direction
            newHead.y += direction.y;

            body.push_front(newHead); // Add new head to the front of the body
            body.pop_back(); // Remove the last segment of the snake
        }

        void CheckInput() { 
            if (IsKeyPressed(KEY_UP) && direction.y == 0) {
                direction = {0, -1}; // Move up
            } else if (IsKeyPressed(KEY_DOWN) && direction.y == 0) {
                direction = {0, 1}; // Move down
            } else if (IsKeyPressed(KEY_LEFT) && direction.x == 0) {
                direction = {-1, 0}; // Move left
            } else if (IsKeyPressed(KEY_RIGHT) && direction.x == 0) {
                direction = {1, 0}; // Move right
            }
        }

        void Reset() { 
            body.clear(); // Clear the snake body
            body.push_back(Vector2{3, 3}); // Reset to initial position
            body.push_back(Vector2{2, 3});
            body.push_back(Vector2{1, 3});
            direction = {1, 0}; // Reset direction to right
            isAlive = true; // Set alive status to true
        }

        void Draw() { 
            if (isAlive) {
                CheckInput(); // Check for user input
                UpdatePosition(); // Update the snake's position
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

        Food() {
            Image apple = LoadImage("snake_assets/apple.png"); // Load apple image
            texture_apple = LoadTextureFromImage(apple); // Load texture from image
            UnloadImage(apple); // Unload image to free memory
        }

        ~Food() {
            UnloadTexture(texture_apple); // Unload texture to free memory
        }

        void spawn() {
            position.x = GetRandomValue(0, rows-1); // Random x position
            position.y = GetRandomValue(0, cols-1); // Random y position
        }

        void CheckCollision(Snake& snake, Sound& eatSound) { 
            if (position.x == snake.body.front().x && position.y == snake.body.front().y) {
                PlaySound(eatSound); // Play eat sound
                snake.body.push_back(snake.body.back()); // Grow the snake
                score += 10; // Increase score
                current_FPS += 1; // Increase FPS
                if (current_FPS > max_FPS) current_FPS = max_FPS; // Cap the FPS
                SetTargetFPS(current_FPS); // Increase FPS based on score
                spawn(); // Respawn food
            }
        }

        void Draw() {
            DrawTexture(texture_apple, position.x * cellSize + borderSize, position.y * cellSize + borderSize, WHITE); // Draw food with border offset
        }
    
    private:
        Vector2 position; // Food position
};

void DisplayScores() { 
    // Display the scores in the border area
    DrawText(TextFormat("Score: %i", score), borderSize, borderSize / 2, 20, BLACK); // Display score at top border
    DrawText(TextFormat("High Score: %i", highscore), windowWidth - borderSize - MeasureText(TextFormat("High Score: %i", highscore), 20), borderSize / 2, 20, BLACK); // Display high score
    if (score > highscore) {
        highscore = score; // Update high score
    }
}

int main() {

    InitWindow(windowWidth, windowHeight, "Snake Game"); // Use the new window dimensions
    SetTargetFPS(current_FPS); // Set the target frames-per-second

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
    food.spawn(); // Spawn the food

    PlayMusicStream(music); // Play background music
    // SetMusicLooping(music, true); // Loop the music

    while (!WindowShouldClose()) {

        UpdateMusicStream(music); // Update the music stream

        BeginDrawing();
        
        ClearBackground(WHITE);
        
        // Draw the game border
        DrawRectangle(borderSize, borderSize, screenWidth, screenHeight, WHITE);
        DrawTexture(texture_background, 0, 0, WHITE); // Draw background texture
        
        DisplayScores(); // Display the scores

        snake.Draw(); // Draw the snake

        if (snake.body.front().x < 0 || snake.body.front().x >= rows || 
            snake.body.front().y < 0 || snake.body.front().y >= cols) {
            if (snake.isAlive) {
                PlaySound(gameOverSound); // Play game over sound
            }
            snake.isAlive = false; 
            DrawRectangle(windowWidth / 2 - (MeasureText("Hold R to restart", 20) / 2) - 25, windowHeight / 2 - 30, 230, 100, WHITE); // Draw game over rectangle
            DrawText("Game Over!", windowWidth / 2 - MeasureText("Game Over!", 20) / 2, windowHeight / 2 - 10, 20, RED); // Use window dimensions
            DrawText("Hold R to restart", windowWidth / 2 - MeasureText("Hold R to restart", 20) / 2, windowHeight / 2 + 20, 20, RED); // Use window dimensions
            StopMusicStream(music); // Stop the music
        }

        for (size_t i = 1; i < snake.body.size(); ++i) {
            if (snake.body.front().x == snake.body[i].x && snake.body.front().y == snake.body[i].y) {
                if (snake.isAlive) {
                    PlaySound(gameOverSound); // Play game over sound
                }
                snake.isAlive = false; // Check for self-collision
                DrawRectangle(windowWidth / 2 - (MeasureText("Hold R to restart", 20) / 2) - 25, windowHeight / 2 - 30, 230, 100, WHITE); // Draw game over rectangle
                DrawText("Game Over!", windowWidth / 2 - MeasureText("Game Over!", 20) / 2, windowHeight / 2 - 10, 20, RED); // Use window dimensions
                DrawText("Hold R to restart", windowWidth / 2 - MeasureText("Hold R to restart", 20) / 2, windowHeight / 2 + 20, 20, RED); // Use window dimensions
                StopMusicStream(music); // Stop the music
            }
        }

        food.CheckCollision(snake, eatSound); // Check for food collision
        food.Draw(); // Draw the food

        if (!snake.isAlive && IsKeyPressed(KEY_R)) {
            snake.Reset(); // Reset the snake
            food.spawn(); // Respawn food
            score = 0; // Reset score
            current_FPS = 5; // Reset FPS
            SetTargetFPS(current_FPS); // Set target FPS
            SeekMusicStream(music, 0.0f); // Restart the music
            PlayMusicStream(music); // Play background music
        }

        EndDrawing();
    }

    UnloadTexture(texture_background); // Unload background texture to free memory
    UnloadMusicStream(music); // Unload music stream to free memory
    UnloadSound(eatSound); // Unload sound to free memory
    UnloadSound(gameOverSound); // Unload sound to free memory
    CloseAudioDevice(); // Close audio device

    CloseWindow();
    return 0;
}