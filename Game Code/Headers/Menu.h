#ifndef MENU_H
#define MENU_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shaderClass.h"
#include "Texture.h"
#include "Audio.h"

// Represents the possible menu actions that can be selected.
enum class MenuButton 
{
    NONE,       // No button selected
    PLAY,       // Start the game
    SETTINGS,   // Open settings menu
    CREDITS,    // Open credits screen
    QUIT        // Exit the game
};

// Handles the main menu interface, including button rendering,
// hover detection, and mouse input processing.
class Menu 
{
    public:
        // Creates and initializes the menu.
        Menu();

        // Releases menu resources.
        ~Menu();

        // Loads textures used for the menu buttons.
        void LoadButtonTextures(const char* play, const char* settings,
            const char* credits, const char* quit);

        // Processes menu input and returns the button that was clicked.
        // Returns NONE if no button was selected.
        MenuButton Update(GLFWwindow* window);

        // Draws the menu and all menu buttons.
        void Draw(GLFWwindow* window, Shader& shader, GLuint uniModel,
            GLuint uniOffset, GLuint uniNumFrames, GLuint uniProjection,
            glm::mat4& projection);

    private:

        // Textures corresponding to menu buttons.
        Texture* buttonTextures[4];

        // Stores the mouse button state from the previous frame.
        // Used to detect fresh clicks instead of continuous presses.
        bool lastMouseState;

        // Represents a clickable menu button.
        struct Button {
            // Position and size in normalized device coordinates.
            float x, y, w, h;

            // Default button color.
            glm::vec4 color;

            // Color displayed while hovering.
            glm::vec4 hoverColor;

            // Action associated with this button.
            MenuButton type;
        };

        // Array containing all menu buttons.
        Button buttons[4];

        // Returns true if the mouse cursor is currently over the button.
        bool IsHovered(const Button& btn, double mouseX, double mouseY,
            GLFWwindow* window) const;

        // Initializes button positions, colors, and actions.
        void InitButtons();
};

#endif