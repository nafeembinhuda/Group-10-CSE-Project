#include "Menu.h"
#include "Constants.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Menu::Menu()
{
    // No mouse button was pressed previously
    lastMouseState = false;

    // Initialize texture pointers to nullptr
    for (int i = 0; i < 4; i++)
        buttonTextures[i] = nullptr;

    // Create menu button definitions
    InitButtons();
}

Menu::~Menu()
{
    // Release all loaded button textures
    for (int i = 0; i < 4; i++)
    {
        if (buttonTextures[i]) {
            buttonTextures[i]->Delete();
            delete buttonTextures[i];
        }
    }
}

void Menu::LoadButtonTextures(const char* play, const char* settings,
                              const char* credits, const char* quit)
{
    // Array of texture file paths
    const char* paths[4] = { play, settings, credits, quit };

    // Load each button texture
    for (int i = 0; i < 4; i++)
        buttonTextures[i] = new Texture(
            paths[i],
            GL_TEXTURE_2D,
            GL_TEXTURE0,
            GL_RGBA,
            GL_UNSIGNED_BYTE
        );
}

void Menu::InitButtons()
{
    // Common button dimensions
    float btnW = 0.4f;
    float btnH = 0.1f;

    // Centre buttons horizontally
    float btnX = -(btnW / 2.0f);

    // Starting vertical position
    float startY = 0.3f;

    // Vertical spacing between buttons
    float gap = 0.18f;

    // Play button
    buttons[0] = {
        btnX, startY,
        btnW, btnH,
        glm::vec4(0.2f, 0.6f, 1.0f, 0.9f),
        glm::vec4(0.4f, 0.8f, 1.0f, 1.0f),
        MenuButton::PLAY
    };

    // Settings button
    buttons[1] = {
        btnX, startY - gap,
        btnW, btnH,
        glm::vec4(0.2f, 0.4f, 0.8f, 0.9f),
        glm::vec4(0.3f, 0.6f, 1.0f, 1.0f),
        MenuButton::SETTINGS
    };

    // Credits button
    buttons[2] = {
        btnX, startY - gap * 2,
        btnW, btnH,
        glm::vec4(0.2f, 0.4f, 0.8f, 0.9f),
        glm::vec4(0.3f, 0.6f, 1.0f, 1.0f),
        MenuButton::CREDITS
    };

    // Quit button
    buttons[3] = {
        btnX, startY - gap * 3,
        btnW, btnH,
        glm::vec4(0.6f, 0.1f, 0.1f, 0.9f),
        glm::vec4(0.8f, 0.2f, 0.2f, 1.0f),
        MenuButton::QUIT
    };
}

bool Menu::IsHovered(const Button& btn,
                     double mouseX,
                     double mouseY,
                     GLFWwindow* window) const
{
    // Obtain current window size
    int winW, winH;
    glfwGetWindowSize(window, &winW, &winH);

    // Convert mouse position from screen coordinates
    // to OpenGL Normalized Device Coordinates (NDC)
    float ndcX = (float)(mouseX / winW) * 2.0f * ASPECT_RATIO - ASPECT_RATIO;
    float ndcY = -(float)(mouseY / winH) * 2.0f + 1.0f;

    // Check whether cursor lies inside button bounds
    return ndcX >= btn.x &&
        ndcX <= btn.x + btn.w &&
        ndcY >= btn.y &&
        ndcY <= btn.y + btn.h;
}

MenuButton Menu::Update(GLFWwindow* window)
{
    // Get current mouse position
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Detect mouse button state
    bool currentMouseState =
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    // Detect a fresh click (pressed this frame only)
    bool freshClick = currentMouseState && !lastMouseState;

    // Store current state for next frame
    lastMouseState = currentMouseState;

    // Check button selection only on a fresh click
    if (freshClick)
    {
        for (int i = 0; i < 4; i++)
        {
            if (IsHovered(buttons[i], mouseX, mouseY, window))
                return buttons[i].type;
        }
    }

    return MenuButton::NONE;
}

void Menu::Draw(GLFWwindow* window,
                Shader& shader,
                GLuint uniModel,
                GLuint uniOffset,
                GLuint uniNumFrames,
                GLuint uniProjection,
                glm::mat4& projection)
{
    // Activate menu shader
    shader.Activate();

    // Upload projection matrix
    glUniformMatrix4fv(
        uniProjection,
        1,
        GL_FALSE,
        glm::value_ptr(projection)
    );

    // Menu textures use a single frame
    glUniform1f(uniOffset, 0.0f);
    glUniform1i(uniNumFrames, 1);

    // Get current mouse position
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Draw each menu button
    for (int i = 0; i < 4; i++)
    {
        if (!buttonTextures[i])
            continue;

        // Check if mouse is currently hovering over button
        bool hovered =
            IsHovered(buttons[i], mouseX, mouseY, window);

        // Build transformation matrix
        glm::mat4 model = glm::mat4(1.0f);

        // Move quad to button centre
        model = glm::translate(
            model,
            glm::vec3(
                buttons[i].x + buttons[i].w / 2.0f,
                buttons[i].y + buttons[i].h / 2.0f,
                0.0f
            )
        );

        // Slightly enlarge button when hovered
        model = glm::scale(
            model,
            glm::vec3(
                buttons[i].w * (hovered ? 1.05f : 1.0f),
                buttons[i].h * (hovered ? 1.05f : 1.0f),
                1.0f
            )
        );

        // Upload model matrix
        glUniformMatrix4fv(
            uniModel,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        // Bind button texture
        buttonTextures[i]->Bind();

        // Render textured quad
        glDrawElements(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            0
        );
    }
}