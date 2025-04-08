#ifndef FOOD_H
#define FOOD_H

#include <raylib.h>
#include <vector>
#include "GameConstants.h"
#include "Snake.h"

enum class AppleType {
    NORMAL_APPLE,
    GOLDEN_APPLE,
    FROZEN_APPLE
};

// Structure to represent floating score text
struct FloatingText {
    Vector2 position;    // Position of the text
    Vector2 velocity;    // Velocity/direction of the text
    const char* text;    // The text to display (e.g., "+10")
    float lifeTime;      // Total life time of the text
    float lifeRemaining; // Remaining life of the text
    Color color;         // Color of the text
    int fontSize;        // Font size
};

class Food { 
public:
    Texture2D texture_apple, texture_golden_apple, texture_frozen_apple; // Textures for food
    float rotationAngle = 0.0f; // Rotation angle for animation

    Food();
    ~Food();
    void spawn(Snake& snake);
    void CheckCollision(Snake& snake, Sound& eatSound);
    void Update();
    void Draw();
    
private:
    Vector2 position; // Food position
    std::vector<FloatingText> floatingTexts; // Floating score texts
    AppleType appleType = AppleType::NORMAL_APPLE; // Current apple type
    
    // Helper methods for floating text management
    void AddFloatingText(Vector2 pos, const char* text, Color color);
    void UpdateFloatingTexts();
    void DrawFloatingTexts();
};

#endif // FOOD_H
