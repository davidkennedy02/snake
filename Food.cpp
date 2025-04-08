#include "Food.h"
#include <math.h>

Food::Food() {
    Image apple = LoadImage("snake_assets/apple.png"); // Load apple image
    Image golden_apple = LoadImage("snake_assets/golden_apple.png"); // Load golden apple image
    Image frozen_apple = LoadImage("snake_assets/frozen_apple.png"); // Load frozen apple image
    texture_apple = LoadTextureFromImage(apple); // Load texture from apple image
    texture_golden_apple = LoadTextureFromImage(golden_apple); // Load texture from golden apple image
    texture_frozen_apple = LoadTextureFromImage(frozen_apple); // Load texture from frozen apple image
    UnloadImage(apple); // Unload image to free memory
    UnloadImage(golden_apple);
    UnloadImage(frozen_apple);
}

Food::~Food() {
    UnloadTexture(texture_apple); // Unload texture to free memory
    UnloadTexture(texture_golden_apple);
    UnloadTexture(texture_frozen_apple);
}

void Food::spawn(Snake& snake) {

    int r = GetRandomValue(1, 100); 
    if (r <= 5) {
        appleType = AppleType::GOLDEN_APPLE; // Spawn golden apple
    } else if (r <= 15) {
        appleType = AppleType::FROZEN_APPLE; // Spawn frozen apple
    } else {
        appleType = AppleType::NORMAL_APPLE; // Spawn normal apple
    }

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

void Food::AddFloatingText(Vector2 pos, const char* text, Color color) {
    FloatingText ft;
    ft.position = pos;
    ft.velocity = {0, -1.5f}; // Move upward
    ft.text = text;
    ft.lifeTime = 1.5f; // Last for 1.5 seconds
    ft.lifeRemaining = ft.lifeTime;
    ft.color = color;
    ft.fontSize = 24; // Size of the text
    
    floatingTexts.push_back(ft);
}

void Food::UpdateFloatingTexts() {
    // Update all floating texts and remove expired ones
    for (int i = floatingTexts.size() - 1; i >= 0; i--) {
        // Update position
        floatingTexts[i].position.x += floatingTexts[i].velocity.x;
        floatingTexts[i].position.y += floatingTexts[i].velocity.y;
        
        // Update lifetime
        floatingTexts[i].lifeRemaining -= GetFrameTime();
        
        // Remove if expired
        if (floatingTexts[i].lifeRemaining <= 0) {
            floatingTexts.erase(floatingTexts.begin() + i);
        }
    }
}

void Food::DrawFloatingTexts() {
    for (auto& ft : floatingTexts) {
        // Calculate alpha based on remaining life
        float alpha = ft.lifeRemaining / ft.lifeTime;
        Color textColor = ColorAlpha(ft.color, alpha);
        
        // Calculate text width for centering
        int textWidth = MeasureText(ft.text, ft.fontSize);
        
        // Draw the text centered at position
        DrawText(ft.text, 
                 ft.position.x - textWidth/2, 
                 ft.position.y, 
                 ft.fontSize, 
                 textColor);
    }
}

void Food::CheckCollision(Snake& snake, Sound& eatSound) { 
    if (position.x == snake.body.front().x && position.y == snake.body.front().y) {
        PlaySound(eatSound); // Play eat sound
        snake.body.push_back(snake.body.back()); // Grow the snake
        score += 10; // Increase score
        
        // Create floating text at the position where food was eaten
        Vector2 textPos = {
            position.x * cellSize + borderSize + cellSize / 2,
            position.y * cellSize + borderSize + cellSize / 2
        };
        
        switch (appleType) {
            case AppleType::NORMAL_APPLE:
                AddFloatingText(textPos, "+10", RED); // Normal apple
                score += 10; // Increase score
                break;
            case AppleType::GOLDEN_APPLE:
                AddFloatingText(textPos, "+50", GOLD); // Golden apple
                score += 50; // Increase score
                break;
            case AppleType::FROZEN_APPLE:
                AddFloatingText(textPos, "+5", BLUE); // Frozen apple
                score += 5;
                moveDelay += 5; // Increase move delay
                if (moveDelay > 15) moveDelay = 15; // Cap the delay to a maximum of 15
                break;
        }
        
        // Increase speed by decreasing move delay
        if (moveDelay > minMoveDelay)
            moveDelay -= 1;
        
        spawn(snake); // Respawn food
    }
}

void Food::Update() {
    float time = GetTime(); // Get current time
    float rotationSpeed = 2.0f;
    float maxRotation = 15.0f;

    rotationAngle = sin(time * rotationSpeed) * maxRotation;
    
    // Update floating texts
    UpdateFloatingTexts();
}

void Food::Draw() {
    Update(); // Update rotation angle

    Texture2D spawnedTexture = texture_apple; // Default texture
    switch (appleType)
    {
    case AppleType::NORMAL_APPLE:
        spawnedTexture = texture_apple; // Normal apple
        break;
    case AppleType::GOLDEN_APPLE:
        spawnedTexture = texture_golden_apple;
        break;
    case AppleType::FROZEN_APPLE:
        spawnedTexture = texture_frozen_apple;
        break;
    }

    Vector2 origin = {
        spawnedTexture.width / 2.0f, 
        spawnedTexture.height / 2.0f
    }; // Set origin for rotation
    
    Rectangle sourceRec = {
        0, 
        0, 
        (float)spawnedTexture.width, 
        (float)spawnedTexture.height
    }; // Source rectangle for texture

    Rectangle destRec = {
        position.x * cellSize + borderSize + cellSize / 2, 
        position.y * cellSize + borderSize + cellSize / 2, 
        (float)spawnedTexture.width, 
        (float)spawnedTexture.height
    }; // Destination rectangle for texture
    
    DrawTexturePro(spawnedTexture, sourceRec, destRec, origin, rotationAngle, WHITE); // Draw food with rotation
    
    // Draw floating texts
    DrawFloatingTexts();
}
