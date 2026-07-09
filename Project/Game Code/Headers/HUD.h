#pragma once
#ifndef HUD_H
#define HUD_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shaderClass.h"
#include "Texture.h"
#include <GLFW/glfw3.h>

// Handles all user interface rendering including:
// - Health bar
// - Timer
// - Wave information
// - Score display
// - Main menu
// - Pause menu
// - Settings menu
// - Victory/Game Over screens
// - Credits screen
class HUD {
    public:
        // Initializes HUD resources and rendering data.
        HUD();

        // Releases allocated OpenGL resources and textures.
        ~HUD();

        // Loads the digit sprite sheet used for numbers.
        void LoadDigitTexture(const char* path);

        // Loads all UI label textures.
        void LoadLabelTextures();

        // Draws the in-game HUD.
        //
        // currentHealth   = player's current health
        // maxHealth       = player's maximum health
        // framesRemaining = remaining wave time in frames
        // waveNumber      = current wave number
        // totalWaves      = total number of waves
        // score           = current player score
        void Draw(
            int currentHealth,
            int maxHealth,
            int framesRemaining,
            int waveNumber,
            int totalWaves,
            int score
        );

        // Draws the Game Over screen.
        void DrawGameOver();

        // Draws the Victory screen.
        void DrawVictory();

        // Draws the main menu screen.
        void DrawMenu(GLFWwindow* window);

        // Draws the pause menu screen.
        void DrawPaused(GLFWwindow* window);

        // Buttons available on the pause menu.
        enum class PauseButton {
            NONE,       // No button selected
            RESUME,     // Resume gameplay
            SETTINGS,   // Open settings menu
            QUIT        // Exit to menu or quit game
        };

        // Processes pause menu interaction and returns
        // the selected button action.
        PauseButton UpdatePaused(GLFWwindow* window);

        // Buttons available in the settings menu.
        enum class SettingsButton {
            NONE,
            BACK,
            MASTER_SLIDER,
            MUSIC_SLIDER,
            SFX_SLIDER
        };

        // Draws the settings screen and volume controls.
        void DrawSettings(
            GLFWwindow* window,
            float masterVol,
            float musicVol,
            float sfxVol
        );

        // Processes settings menu input and updates volume values.
        SettingsButton UpdateSettings(
            GLFWwindow* window,
            float& masterVol,
            float& musicVol,
            float& sfxVol
        );

        // Draws the credits screen.
        void DrawCredits();

    private:
        // Vertex Array Object for UI rendering.
        GLuint vao;

        // Vertex Buffer Object for UI geometry.
        GLuint vbo;

        // Element Buffer Object for indexed rendering.
        GLuint ebo;

        // Shader used to render HUD elements.
        Shader* uiShader;

        // Texture containing digits 0–9.
        Texture* digitTex;

        // Main menu label textures.
        Texture* labelPlay;
        Texture* labelSettings;
        Texture* labelCredits;
        Texture* labelQuit;

        // Pause menu label textures.
        Texture* labelResume;
        Texture* labelPaused;

        // End-screen label textures.
        Texture* labelGameOver;
        Texture* labelVictory;

        // Settings menu labels.
        Texture* labelMaster;
        Texture* labelMusic;
        Texture* labelSFX;
        Texture* labelBack;

        // Control instruction labels.
        Texture* labelCtrlMove;
        Texture* labelCtrlJump;
        Texture* labelCtrlAttack;
        Texture* labelCtrlBlock;

        // Miscellaneous menu labels.
        Texture* labelClickContinue;
        Texture* labelCreditsScreen;

        // Shader uniform locations.
        GLint uniPos;
        GLint uniSize;
        GLint uniColor;

        // UV mapping uniforms used for digit rendering.
        GLint uniUVOffset;
        GLint uniUVWidth;

        // Texture rendering controls.
        GLint uniUseTexture;
        GLint uniTex;

        // Draws a colored rectangular UI element.
        void DrawQuad(float x, float y, float w, float h, glm::vec4 color);

        // Draws a single numerical digit.
        void DrawDigit(int digit, float x, float y, float h);

        // Draws a colon character for timer display.
        void DrawColon(float x, float y, float h);

        // Draws the player's health bar.
        void DrawHealthBar(int current, int max);

        // Draws the wave countdown timer.
        void DrawTimer(int framesRemaining);

        // Draws current wave progress.
        void DrawWaveNumber(int waveNumber, int totalWaves);

        // Draws the current score.
        void DrawScore(int score);

        // Draws a textured label at the specified position and size.
        void DrawLabel(Texture* label, float btnX, float btnY, float btnW, float btnH);
};

#endif