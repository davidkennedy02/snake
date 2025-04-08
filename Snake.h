#ifndef SNAKE_H
#define SNAKE_H

#include <raylib.h>
#include <deque>
#include "GameConstants.h"

class Snake { 
public:
    std::deque<Vector2> body = {Vector2{3, 3}, Vector2{2, 3}, Vector2{1, 3}}; // Snake body segments
    Vector2 direction = {1, 0}; // Initial direction (right)
    Vector2 nextDirection = {1, 0}; // Buffer for next direction
    bool isAlive = true; // Snake alive status
    Texture2D texture_head_up, texture_head_down, texture_head_left, texture_head_right;
    Texture2D texture_body_up, texture_body_down, texture_body_left, texture_body_right;
    Texture2D texture_tail_up, texture_tail_down, texture_tail_left, texture_tail_right;
    Texture2D texture_dead_head_up, texture_dead_head_down, texture_dead_head_left, texture_dead_head_right;
    Texture2D texture_dead_body_up, texture_dead_body_down, texture_dead_body_left, texture_dead_body_right;
    Texture2D texture_dead_tail_up, texture_dead_tail_down, texture_dead_tail_left, texture_dead_tail_right;
    
    Snake();
    ~Snake();
    void UpdatePosition();
    void CheckInput();
    bool HasCrashed();
    void Reset();
    void Draw();
};

#endif // SNAKE_H
