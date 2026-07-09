#ifndef CONSTANTS_H
#define CONSTANTS_H

// Current window aspect ratio.
// Declared here and defined in main.cpp so it can be shared globally.
extern float ASPECT_RATIO;

// Target frames per second used throughout the game.
#define FPS 30

// Uniform scale applied to all character sprites.
#define CHARACTER_SCALE 0.25f

// Y-coordinate representing the ground surface.
#define GROUND_LEVEL -0.3f

// Horizontal movement speed of characters.
#define X_VELOCITY 0.025f

// Initial upward velocity applied when jumping.
#define Y_VELOCITY 0.063f

// Downward acceleration applied every frame.
#define GRAVITY    0.004f

#endif