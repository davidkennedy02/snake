#include "Snake.h"

Snake::Snake() {
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
    Image dead_head_up = LoadImage("snake_assets/dead_head_up.png"); // Load dead head image
    Image dead_head_down = LoadImage("snake_assets/dead_head_down.png"); // Load dead head image
    Image dead_head_left = LoadImage("snake_assets/dead_head_left.png"); // Load dead head image
    Image dead_head_right = LoadImage("snake_assets/dead_head_right.png"); // Load dead head image
    Image dead_body_up = LoadImage("snake_assets/dead_body_up.png"); // Load dead body image
    Image dead_body_down = LoadImage("snake_assets/dead_body_down.png"); // Load dead body image
    Image dead_body_left = LoadImage("snake_assets/dead_body_left.png"); // Load dead body image
    Image dead_body_right = LoadImage("snake_assets/dead_body_right.png"); // Load dead body image
    Image dead_tail_up = LoadImage("snake_assets/dead_tail_up.png"); // Load dead tail image
    Image dead_tail_down = LoadImage("snake_assets/dead_tail_down.png"); // Load dead tail image
    Image dead_tail_left = LoadImage("snake_assets/dead_tail_left.png"); // Load dead tail image
    Image dead_tail_right = LoadImage("snake_assets/dead_tail_right.png"); // Load dead tail image

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
    texture_dead_head_up = LoadTextureFromImage(dead_head_up); // Load texture from image
    texture_dead_head_down = LoadTextureFromImage(dead_head_down); // Load texture from image
    texture_dead_head_left = LoadTextureFromImage(dead_head_left); // Load texture from image
    texture_dead_head_right = LoadTextureFromImage(dead_head_right); // Load texture from image
    texture_dead_body_up = LoadTextureFromImage(dead_body_up); // Load texture from image
    texture_dead_body_down = LoadTextureFromImage(dead_body_down); // Load texture from image
    texture_dead_body_left = LoadTextureFromImage(dead_body_left); // Load texture from image
    texture_dead_body_right = LoadTextureFromImage(dead_body_right); // Load texture from image
    texture_dead_tail_up = LoadTextureFromImage(dead_tail_up); // Load texture from image
    texture_dead_tail_down = LoadTextureFromImage(dead_tail_down); // Load texture from image
    texture_dead_tail_left = LoadTextureFromImage(dead_tail_left); // Load texture from image
    texture_dead_tail_right = LoadTextureFromImage(dead_tail_right); // Load texture from image

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
    UnloadImage(dead_head_up); // Unload image to free memory
    UnloadImage(dead_head_down); // Unload image to free memory
    UnloadImage(dead_head_left); // Unload image to free memory
    UnloadImage(dead_head_right);
    UnloadImage(dead_body_up); // Unload image to free memory
    UnloadImage(dead_body_down); // Unload image to free memory
    UnloadImage(dead_body_left);
    UnloadImage(dead_body_right);
    UnloadImage(dead_tail_up); // Unload image to free memory
    UnloadImage(dead_tail_down); // Unload image to free memory
    UnloadImage(dead_tail_left); // Unload image to free memory
    UnloadImage(dead_tail_right); // Unload image to free memory
}

Snake::~Snake() {
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
    UnloadTexture(texture_dead_head_up); // Unload texture to free memory
    UnloadTexture(texture_dead_head_down); // Unload texture to free memory
    UnloadTexture(texture_dead_head_left); // Unload texture to free memory
    UnloadTexture(texture_dead_head_right); // Unload texture to free memory
    UnloadTexture(texture_dead_body_up); // Unload texture to free memory
    UnloadTexture(texture_dead_body_down); // Unload texture to free memory
    UnloadTexture(texture_dead_body_left); // Unload texture to free memory
    UnloadTexture(texture_dead_body_right); // Unload texture to free memory
    UnloadTexture(texture_dead_tail_up);
    UnloadTexture(texture_dead_tail_down);
    UnloadTexture(texture_dead_tail_left);
    UnloadTexture(texture_dead_tail_right);
}

void Snake::UpdatePosition() { 
    // Apply the buffered direction
    direction = nextDirection;
    
    Vector2 newHead = body.front(); // Get the current head position
    newHead.x += direction.x; // Update head position based on direction
    newHead.y += direction.y;

    body.push_front(newHead); // Add new head to the front of the body
    body.pop_back(); // Remove the last segment of the snake
}

void Snake::CheckInput() { 
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

bool Snake::HasCrashed() { 
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

void Snake::Reset() { 
    body.clear(); // Clear the snake body
    body.push_back(Vector2{3, 3}); // Reset to initial position
    body.push_back(Vector2{2, 3});
    body.push_back(Vector2{1, 3});
    direction = {1, 0}; // Reset direction to right
    nextDirection = {1, 0}; // Reset next direction to right
    isAlive = true; // Set alive status to true
}

void Snake::Draw() { 
    if (isAlive) {
        CheckInput(); // Check for user input every frame
        
        // Only update position when frame counter reaches threshold
        if (frameCounter >= moveDelay) {
            UpdatePosition(); // Update the snake's position
            frameCounter = 0; // Reset frame counter

            if (HasCrashed()) {
                isAlive = false; // Set alive status to false if crashed
            }
        }
    }
    
    for (int i = body.size() - 1; i >= 0; i--) {
        Vector2 segment = body[i]; // Get the current segment position
        float x = segment.x * cellSize + borderSize; // Add border offset to x
        float y = segment.y * cellSize + borderSize; // Add border offset to y
        
        if (i == 0) {
            if (!isAlive) {
                // Draw the dead head based on the direction
                if (direction.x == 1) DrawTexture(texture_dead_head_right, x, y, WHITE); 
                else if (direction.x == -1) DrawTexture(texture_dead_head_left, x, y, WHITE); 
                else if (direction.y == 1) DrawTexture(texture_dead_head_down, x, y, WHITE); 
                else if (direction.y == -1) DrawTexture(texture_dead_head_up, x, y, WHITE); 
            } else {
                // Draw the head based on the direction
                if (direction.x == 1) DrawTexture(texture_head_right, x, y, WHITE); 
                else if (direction.x == -1) DrawTexture(texture_head_left, x, y, WHITE); 
                else if (direction.y == 1) DrawTexture(texture_head_down, x, y, WHITE); 
                else if (direction.y == -1) DrawTexture(texture_head_up, x, y, WHITE); 
            }
        } else if (i == body.size() - 1) { 
            // Draw the tail based on the direction of the second last segment
            if (!isAlive) {
                if (body[i].x == body[i-1].x) {
                    if (body[i].y > body[i-1].y) DrawTexture(texture_dead_tail_up, x, y, WHITE); // Draw dead tail up
                    else DrawTexture(texture_dead_tail_down, x, y, WHITE); // Draw dead tail down
                } else if (body[i].y == body[i-1].y) {
                    if (body[i].x > body[i-1].x) DrawTexture(texture_dead_tail_left, x, y, WHITE); // Draw dead tail left
                    else DrawTexture(texture_dead_tail_right, x, y, WHITE); // Draw dead tail right
                }
            } else {
                if (body[i].x == body[i-1].x) {
                    if (body[i].y > body[i-1].y) DrawTexture(texture_tail_up, x, y, WHITE); // Draw tail up
                    else DrawTexture(texture_tail_down, x, y, WHITE); // Draw tail down
                } else if (body[i].y == body[i-1].y) {
                    if (body[i].x > body[i-1].x) DrawTexture(texture_tail_left, x, y, WHITE); // Draw tail left
                    else DrawTexture(texture_tail_right, x, y, WHITE); // Draw tail right
                }
            }
        } else {
            if (!isAlive) {
                // Draw the dead body based on the direction of the previous segment
                if (body[i].x == body[i-1].x) {
                    if (body[i].y > body[i-1].y) DrawTexture(texture_dead_body_down, x, y, WHITE); // Draw dead body down
                    else DrawTexture(texture_dead_body_up, x, y, WHITE); // Draw dead body up
                } else if (body[i].y == body[i-1].y) {
                    if (body[i].x > body[i-1].x) DrawTexture(texture_dead_body_right, x, y, WHITE); // Draw dead body right
                    else DrawTexture(texture_dead_body_left, x, y, WHITE); // Draw dead body left
                } 
            } else {
                // Draw the body based on the direction of the previous segment
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
}
