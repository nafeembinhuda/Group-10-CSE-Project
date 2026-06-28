#include "HUD.h"
#include "Constants.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

HUD::HUD() : digitTex(nullptr), labelPlay(nullptr), labelSettings(nullptr),
labelCredits(nullptr), labelQuit(nullptr), labelResume(nullptr),
labelPaused(nullptr), labelGameOver(nullptr), labelVictory(nullptr),
labelMaster(nullptr), labelMusic(nullptr), labelSFX(nullptr),
labelBack(nullptr), labelCtrlMove(nullptr), labelCtrlJump(nullptr),
labelCtrlAttack(nullptr), labelCtrlBlock(nullptr), labelClickContinue(nullptr),
labelCreditsScreen(nullptr)
{
    // ------------------------------------------------------------------
    // Create a unit quad (0,0 → 1,1) that will be reused for all HUD
    // elements. Position and size are controlled through shader uniforms.
    // ------------------------------------------------------------------
    float vertices[] = {
        0.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 0.0f,  1.0f, 0.0f,
        1.0f, 1.0f,  1.0f, 1.0f,
        0.0f, 1.0f,  0.0f, 1.0f
    };

    GLuint indices[] = { 0, 1, 2, 0, 2, 3 };

    // Generate OpenGL objects
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Upload vertex and index data
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW);

    // Vertex position attribute
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(float),
        (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Load HUD shader
    uiShader = new Shader(
        "Resources/UI Shaders/ui.vert",
        "Resources/UI Shaders/ui.frag");

    // Cache uniform locations for faster access
    uniPos = glGetUniformLocation(uiShader->ID, "uPos");
    uniSize = glGetUniformLocation(uiShader->ID, "uSize");
    uniColor = glGetUniformLocation(uiShader->ID, "uColor");
    uniUVOffset = glGetUniformLocation(uiShader->ID, "uUVOffset");
    uniUVWidth = glGetUniformLocation(uiShader->ID, "uUVWidth");
    uniUseTexture = glGetUniformLocation(uiShader->ID, "uUseTexture");
    uniTex = glGetUniformLocation(uiShader->ID, "uTex");
}

HUD::~HUD()
{
    // Release OpenGL buffers
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    // Release shader
    if (uiShader) {
        uiShader->Delete();
        delete uiShader;
    }

    // Release all loaded textures
    if (digitTex) { digitTex->Delete(); delete digitTex; }

    if (labelPlay) { labelPlay->Delete(); delete labelPlay; }
    if (labelSettings) { labelSettings->Delete(); delete labelSettings; }
    if (labelCredits) { labelCredits->Delete(); delete labelCredits; }
    if (labelQuit) { labelQuit->Delete(); delete labelQuit; }

    if (labelResume) { labelResume->Delete(); delete labelResume; }
    if (labelPaused) { labelPaused->Delete(); delete labelPaused; }
    if (labelGameOver) { labelGameOver->Delete(); delete labelGameOver; }
    if (labelVictory) { labelVictory->Delete(); delete labelVictory; }

    if (labelMaster) { labelMaster->Delete(); delete labelMaster; }
    if (labelMusic) { labelMusic->Delete(); delete labelMusic; }
    if (labelSFX) { labelSFX->Delete(); delete labelSFX; }
    if (labelBack) { labelBack->Delete(); delete labelBack; }

    if (labelCtrlMove) { labelCtrlMove->Delete(); delete labelCtrlMove; }
    if (labelCtrlJump) { labelCtrlJump->Delete(); delete labelCtrlJump; }
    if (labelCtrlAttack) { labelCtrlAttack->Delete(); delete labelCtrlAttack; }
    if (labelCtrlBlock) { labelCtrlBlock->Delete(); delete labelCtrlBlock; }

    if (labelCreditsScreen) {
        labelCreditsScreen->Delete();
        delete labelCreditsScreen;
    }
}

void HUD::LoadDigitTexture(const char* path)
{
    // Texture containing digits 0-9 arranged horizontally
    digitTex = new Texture(
        path,
        GL_TEXTURE_2D,
        GL_TEXTURE0,
        GL_RGBA,
        GL_UNSIGNED_BYTE);
}

void HUD::LoadLabelTextures()
{
    // Menu labels
    labelPlay = new Texture(
        "Resources/Textures/Menu/play.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    labelSettings = new Texture(
        "Resources/Textures/Menu/settings.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    labelCredits = new Texture(
        "Resources/Textures/Menu/credits.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    labelQuit = new Texture(
        "Resources/Textures/Menu/quit.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    // Pause screen labels
    labelResume = new Texture(
        "Resources/Textures/Menu/continue.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    labelPaused = new Texture(
        "Resources/Textures/Menu/paused.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    // End screen labels
    labelGameOver = new Texture(
        "Resources/Textures/Menu/gameOver.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    labelVictory = new Texture(
        "Resources/Textures/Menu/victory.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    // Settings menu labels
    labelMaster = new Texture(
        "Resources/Textures/UI/Labels/label_master.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    labelMusic = new Texture(
        "Resources/Textures/UI/Labels/label_music.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    labelSFX = new Texture(
        "Resources/Textures/UI/Labels/label_sfx.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    labelBack = new Texture(
        "Resources/Textures/UI/Labels/label_back.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    // Control descriptions
    labelCtrlMove = new Texture(
        "Resources/Textures/UI/Labels/label_ctrl_move.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    labelCtrlJump = new Texture(
        "Resources/Textures/UI/Labels/label_ctrl_jump.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    labelCtrlAttack = new Texture(
        "Resources/Textures/UI/Labels/label_ctrl_attack.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    labelCtrlBlock = new Texture(
        "Resources/Textures/UI/Labels/label_ctrl_block.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    // Miscellaneous labels
    labelClickContinue = new Texture(
        "Resources/Textures/UI/Labels/label_click_continue.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);

    labelCreditsScreen = new Texture(
        "Resources/Textures/UI/Labels/credits.png",
        GL_TEXTURE_2D, GL_TEXTURE0,
        GL_RGBA, GL_UNSIGNED_BYTE);
}

void HUD::DrawQuad(float x, float y, float w, float h, glm::vec4 color)
{
    // Set screen position and size of the quad
    glUniform2f(uniPos, x, y);
    glUniform2f(uniSize, w, h);

    // Set solid colour
    glUniform4f(
        uniColor,
        color.r,
        color.g,
        color.b,
        color.a
    );

    // Disable texture sampling
    glUniform1i(uniUseTexture, 0);

    // Use full UV range
    glUniform1f(uniUVOffset, 0.0f);
    glUniform1f(uniUVWidth, 1.0f);

    // Render quad
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void HUD::DrawDigit(int digit, float x, float y, float h)
{
    // Ignore invalid requests
    if (!digitTex || digit < 0 || digit > 9)
        return;

    // Each digit occupies 1/10th of the texture
    float uvW = 1.0f / 10.0f;

    // Maintain digit aspect ratio
    float w = (h * 0.5f) / ASPECT_RATIO;

    digitTex->Bind();

    glUniform2f(uniPos, x, y);
    glUniform2f(uniSize, w, h);

    // Select the appropriate digit region
    glUniform1f(uniUVOffset, digit * uvW);
    glUniform1f(uniUVWidth, uvW);

    glUniform1i(uniUseTexture, 1);
    glUniform1i(uniTex, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void HUD::DrawColon(float x, float y, float h)
{
    // Draw two small dots to represent ':'
    float dotW = (h * 0.12f) / ASPECT_RATIO;
    float dotH = h * 0.12f;

    float cx = x + (h * 0.1f) / ASPECT_RATIO;

    DrawQuad(
        cx,
        y + h * 0.55f,
        dotW,
        dotH,
        glm::vec4(1.0f)
    );

    DrawQuad(
        cx,
        y + h * 0.25f,
        dotW,
        dotH,
        glm::vec4(1.0f)
    );
}

void HUD::DrawHealthBar(int current, int max)
{
    // Health bar position and dimensions
    const float barX = -0.95f;
    const float barY = 0.82f;
    const float barW = 0.50f;
    const float barH = 0.06f;
    const float pad = 0.007f;

    // Black border
    DrawQuad(
        barX - pad,
        barY - pad,
        barW + pad * 2,
        barH + pad * 2,
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    // Background
    DrawQuad(
        barX,
        barY,
        barW,
        barH,
        glm::vec4(0.25f, 0.0f, 0.0f, 1.0f)
    );

    // Remaining health percentage
    float pct = glm::clamp(
        (float)current / max,
        0.0f,
        1.0f
    );

    // Smooth colour transition:
    // Green → Yellow → Red
    glm::vec4 fill;

    if (pct > 0.5f)
    {
        fill = glm::mix(
            glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
            glm::vec4(0.0f, 0.9f, 0.1f, 1.0f),
            (pct - 0.5f) * 2.0f
        );
    }
    else
    {
        fill = glm::mix(
            glm::vec4(0.9f, 0.0f, 0.0f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
            pct * 2.0f
        );
    }

    // Draw current health
    DrawQuad(
        barX,
        barY,
        barW * pct,
        barH,
        fill
    );
}

void HUD::DrawTimer(int framesRemaining)
{
    // Convert frame count into seconds
    int total = std::max(framesRemaining, 0) / FPS;

    int minutes = total / 60;
    int seconds = total % 60;

    // Draw digital timer when digit texture exists
    if (digitTex)
    {
        const float h = 0.14f;

        const float w =
            (h * 0.5f) / ASPECT_RATIO;

        const float gap = w + 0.015f;

        const float colonW =
            (h * 0.12f) / ASPECT_RATIO;

        const float colonGap =
            colonW + 0.015f;

        const float y = 0.85f;

        float totalWidth =
            gap * 4 + colonGap;

        float startX =
            -(totalWidth / 2.0f);

        float d0X = startX;
        float d1X = startX + gap;
        float colX = startX + gap * 2.0f;
        float d2X = colX + colonGap;
        float d3X = colX + colonGap + gap;

        DrawDigit(minutes / 10, d0X, y, h);
        DrawDigit(minutes % 10, d1X, y, h);

        DrawColon(colX, y, h);

        DrawDigit(seconds / 10, d2X, y, h);
        DrawDigit(seconds % 10, d3X, y, h);
    }
    else
    {
        // Fallback progress bar timer
        const float waveDuration =
            2.0f * 60.0f * FPS;

        float pct = glm::clamp(
            (float)framesRemaining / waveDuration,
            0.0f,
            1.0f
        );

        const float barW = 0.60f;
        const float barX = -(barW / 2.0f);
        const float barY = 0.90f;
        const float barH = 0.04f;
        const float pad = 0.005f;

        DrawQuad(
            barX - pad,
            barY - pad,
            barW + pad * 2,
            barH + pad * 2,
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
        );

        DrawQuad(
            barX,
            barY,
            barW,
            barH,
            glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)
        );

        // Colour changes when time becomes low
        glm::vec4 tColor =
            pct > 0.4f
            ? glm::vec4(0.2f, 0.8f, 1.0f, 1.0f)
            : glm::vec4(1.0f, 0.4f, 0.0f, 1.0f);

        DrawQuad(
            barX,
            barY,
            barW * pct,
            barH,
            tColor
        );
    }
}

void HUD::DrawLabel(
    Texture* label,
    float btnX,
    float btnY,
    float btnW,
    float btnH)
{
    // Ignore missing textures
    if (!label)
        return;

    label->Bind();

    glUniform2f(uniPos, btnX, btnY);
    glUniform2f(uniSize, btnW, btnH);

    glUniform1f(uniUVOffset, 0.0f);
    glUniform1f(uniUVWidth, 1.0f);

    glUniform1i(uniUseTexture, 1);
    glUniform1i(uniTex, 0);

    glDrawElements(
        GL_TRIANGLES,
        6,
        GL_UNSIGNED_INT,
        0
    );
}

void HUD::DrawGameOver()
{
    uiShader->Activate();
    glBindVertexArray(vao);

    // Dark red overlay
    DrawQuad(
        -1.0f, -1.0f,
        2.0f, 2.0f,
        glm::vec4(
            0.4f, 0.0f,
            0.0f, 0.7f)
    );

    // Game over title
    DrawLabel(
        labelGameOver,
        -0.5f, 0.05f,
        1.0f, 0.2f
    );

    // Continue prompt
    DrawLabel(
        labelClickContinue,
        -0.4f, -0.25f,
        0.8f, 0.1f
    );

    glBindVertexArray(0);
}

void HUD::DrawVictory()
{
    uiShader->Activate();
    glBindVertexArray(vao);

    // Green victory overlay
    DrawQuad(
        -1.0f, -1.0f,
        2.0f, 2.0f,
        glm::vec4(
            0.0f, 0.3f,
            0.0f, 0.7f)
    );

    // Victory title
    DrawLabel(
        labelVictory,
        -0.5f, 0.05f,
        1.0f, 0.2f
    );

    // Continue prompt
    DrawLabel(
        labelClickContinue,
        -0.4f, -0.25f,
        0.8f, 0.1f
    );

    glBindVertexArray(0);
}

void HUD::DrawSettings(GLFWwindow* window, float masterVol, float musicVol, float sfxVol)
{
    // Activate UI shader and bind quad geometry
    uiShader->Activate();
    glBindVertexArray(vao);

    // Draw dark translucent background overlay
    DrawQuad(-1.0f, -1.0f, 2.0f, 2.0f,
        glm::vec4(0.0f, 0.0f, 0.0f, 0.75f));

    // Draw settings title banner
    DrawQuad(-0.3f, 0.7f, 0.6f, 0.12f,
        glm::vec4(0.1f, 0.1f, 0.4f, 1.0f));
    DrawLabel(labelSettings, -0.3f, 0.7f, 0.6f, 0.12f);

    // Slider dimensions shared by all volume controls
    float sliderX = -0.4f;
    float sliderW = 0.8f;
    float sliderH = 0.05f;

    // Label dimensions
    float labelW = 0.35f;
    float labelH = 0.06f;

    // =========================
    // Master Volume Slider
    // =========================
    DrawQuad(-0.7f, 0.45f, labelW, labelH,
        glm::vec4(0.15f, 0.15f, 0.3f, 1.0f));
    DrawLabel(labelMaster, -0.7f, 0.45f, labelW, labelH);

    // Slider background
    DrawQuad(sliderX, 0.47f, sliderW, sliderH,
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

    // Filled portion based on current volume
    DrawQuad(sliderX, 0.47f, sliderW * masterVol, sliderH,
        glm::vec4(0.3f, 0.6f, 1.0f, 1.0f));

    // =========================
    // Music Volume Slider
    // =========================
    DrawQuad(-0.7f, 0.28f, labelW, labelH,
        glm::vec4(0.15f, 0.15f, 0.3f, 1.0f));
    DrawLabel(labelMusic, -0.7f, 0.28f, labelW, labelH);

    DrawQuad(sliderX, 0.30f, sliderW, sliderH,
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

    DrawQuad(sliderX, 0.30f, sliderW * musicVol, sliderH,
        glm::vec4(0.3f, 0.6f, 1.0f, 1.0f));

    // =========================
    // SFX Volume Slider
    // =========================
    DrawQuad(-0.7f, 0.11f, labelW, labelH,
        glm::vec4(0.15f, 0.15f, 0.3f, 1.0f));
    DrawLabel(labelSFX, -0.7f, 0.11f, labelW, labelH);

    DrawQuad(sliderX, 0.13f, sliderW, sliderH,
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

    DrawQuad(sliderX, 0.13f, sliderW * sfxVol, sliderH,
        glm::vec4(0.3f, 0.6f, 1.0f, 1.0f));

    // =========================
    // Controls Information
    // =========================

    float ctrlX = -0.7f;
    float ctrlY = -0.05f;
    float ctrlW = 1.4f;
    float ctrlH = 0.09f;
    float ctrlGap = 0.13f;

    // Movement controls
    DrawQuad(ctrlX, ctrlY, ctrlW, ctrlH,
        glm::vec4(0.1f, 0.1f, 0.2f, 0.9f));
    DrawLabel(labelCtrlMove, ctrlX, ctrlY, ctrlW, ctrlH);

    // Jump controls
    DrawQuad(ctrlX, ctrlY - ctrlGap, ctrlW, ctrlH,
        glm::vec4(0.1f, 0.1f, 0.2f, 0.9f));
    DrawLabel(labelCtrlJump, ctrlX, ctrlY - ctrlGap, ctrlW, ctrlH);

    // Attack controls
    DrawQuad(ctrlX, ctrlY - ctrlGap * 2, ctrlW, ctrlH,
        glm::vec4(0.1f, 0.1f, 0.2f, 0.9f));
    DrawLabel(labelCtrlAttack, ctrlX, ctrlY - ctrlGap * 2, ctrlW, ctrlH);

    // Block controls
    DrawQuad(ctrlX, ctrlY - ctrlGap * 3, ctrlW, ctrlH,
        glm::vec4(0.1f, 0.1f, 0.2f, 0.9f));
    DrawLabel(labelCtrlBlock, ctrlX, ctrlY - ctrlGap * 3, ctrlW, ctrlH);

    // Get mouse position for hover effect
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int winW, winH;
    glfwGetWindowSize(window, &winW, &winH);

    // Convert mouse coordinates to NDC
    float ndcX = (float)(mouseX / winW) * 2.0f - 1.0f;
    float ndcY = -(float)(mouseY / winH) * 2.0f + 1.0f;

    // Helper function for button hover detection
    auto isHovered = [&](float x, float y, float w, float h)
        {
            return ndcX >= x && ndcX <= x + w &&
                ndcY >= y && ndcY <= y + h;
        };

    // Back button dimensions
    float btnW = 0.3f;
    float btnH = 0.08f;
    float btnX = -(btnW / 2.0f);
    float btnY = -0.88f;

    // Change colour when hovered
    glm::vec4 backColor = isHovered(btnX, btnY, btnW, btnH)
        ? glm::vec4(0.4f, 0.8f, 1.0f, 1.0f)
        : glm::vec4(0.2f, 0.6f, 1.0f, 0.9f);

    DrawQuad(btnX, btnY, btnW, btnH, backColor);
    DrawLabel(labelBack, btnX, btnY, btnW, btnH);

    glBindVertexArray(0);
}

HUD::SettingsButton HUD::UpdateSettings(
    GLFWwindow* window,
    float& masterVol,
    float& musicVol,
    float& sfxVol)
{
    // Get current mouse position
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int winW, winH;
    glfwGetWindowSize(window, &winW, &winH);

    // Convert screen coordinates to NDC
    float ndcX = (float)(mouseX / winW) * 2.0f - 1.0f;
    float ndcY = -(float)(mouseY / winH) * 2.0f + 1.0f;

    // Check if left mouse button is held
    bool held =
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)
        == GLFW_PRESS;

    // Slider dimensions
    float sliderX = -0.4f;
    float sliderW = 0.8f;
    float sliderH = 0.05f;

    // =========================
    // Handle Slider Dragging
    // =========================
    if (held)
    {
        // Updates slider value based on mouse position
        auto dragSlider = [&](float y, float& vol) -> bool
            {
                if (ndcX >= sliderX &&
                    ndcX <= sliderX + sliderW &&
                    ndcY >= y &&
                    ndcY <= y + sliderH)
                {
                    // Convert slider position to value between 0 and 1
                    vol = glm::clamp(
                        (ndcX - sliderX) / sliderW,
                        0.0f,
                        1.0f);

                    return true;
                }
                return false;
            };

        // Master volume slider
        if (dragSlider(0.47f, masterVol))
            return HUD::SettingsButton::MASTER_SLIDER;

        // Music volume slider
        if (dragSlider(0.30f, musicVol))
            return HUD::SettingsButton::MUSIC_SLIDER;

        // SFX volume slider
        if (dragSlider(0.13f, sfxVol))
            return HUD::SettingsButton::SFX_SLIDER;
    }

    // =========================
    // Back Button Detection
    // =========================

    // Detect fresh click only
    static bool lastClick = false;

    bool freshClick = held && !lastClick;
    lastClick = held;

    float btnW = 0.3f;
    float btnH = 0.08f;
    float btnX = -(btnW / 2.0f);
    float btnY = -0.85f;

    // Return BACK if button clicked
    if (freshClick &&
        ndcX >= btnX && ndcX <= btnX + btnW &&
        ndcY >= btnY && ndcY <= btnY + btnH)
    {
        return HUD::SettingsButton::BACK;
    }

    // No interaction detected
    return HUD::SettingsButton::NONE;
}

void HUD::DrawPaused(GLFWwindow* window)
{
    uiShader->Activate();
    glBindVertexArray(vao);

    // Semi-transparent dark overlay
    DrawQuad(
        -1.0f, -1.0f,
        2.0f, 2.0f,
        glm::vec4(0.0f, 0.0f, 0.0f, 0.6f)
    );

    // Pause title background
    DrawQuad(
        -0.25f, 0.5f,
        0.5f, 0.12f,
        glm::vec4(0.1f, 0.1f, 0.4f, 1.0f)
    );

    DrawLabel(
        labelPaused,
        -0.25f, 0.5f,
        0.5f, 0.12f
    );

    // Get mouse position in NDC coordinates
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int winW, winH;
    glfwGetWindowSize(window, &winW, &winH);

    float ndcX =
        (float)(mouseX / winW) * 2.0f - 1.0f;

    float ndcY =
        -(float)(mouseY / winH) * 2.0f + 1.0f;

    // Helper lambda for hover detection
    auto isHovered =
        [&](float x, float y, float w, float h)
        {
            return ndcX >= x &&
                ndcX <= x + w &&
                ndcY >= y &&
                ndcY <= y + h;
        };

    float btnW = 0.4f;
    float btnH = 0.1f;
    float btnX = -(btnW / 2.0f);

    // ---------------- Resume Button ----------------

    glm::vec4 resumeColor =
        isHovered(btnX, 0.2f, btnW, btnH)
        ? glm::vec4(0.4f, 0.8f, 1.0f, 1.0f)
        : glm::vec4(0.2f, 0.6f, 1.0f, 0.9f);

    DrawQuad(btnX, 0.2f, btnW, btnH, resumeColor);

    DrawLabel(
        labelResume,
        btnX, 0.2f,
        btnW, btnH
    );

    // ---------------- Settings Button ----------------

    glm::vec4 settingsColor =
        isHovered(btnX, 0.0f, btnW, btnH)
        ? glm::vec4(0.3f, 0.6f, 1.0f, 1.0f)
        : glm::vec4(0.2f, 0.4f, 0.8f, 0.9f);

    DrawQuad(btnX, 0.0f, btnW, btnH, settingsColor);

    DrawLabel(
        labelSettings,
        btnX, 0.0f,
        btnW, btnH
    );

    // ---------------- Quit Button ----------------

    glm::vec4 quitColor =
        isHovered(btnX, -0.2f, btnW, btnH)
        ? glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)
        : glm::vec4(0.6f, 0.1f, 0.1f, 0.9f);

    DrawQuad(btnX, -0.2f, btnW, btnH, quitColor);

    DrawLabel(
        labelQuit,
        btnX, -0.2f,
        btnW, btnH
    );

    glBindVertexArray(0);
}

HUD::PauseButton HUD::UpdatePaused(GLFWwindow* window)
{
    // Convert mouse position to NDC coordinates
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int winW, winH;
    glfwGetWindowSize(window, &winW, &winH);

    float ndcX =
        (float)(mouseX / winW) * 2.0f - 1.0f;

    float ndcY =
        -(float)(mouseY / winH) * 2.0f + 1.0f;

    bool clicked =
        glfwGetMouseButton(
            window,
            GLFW_MOUSE_BUTTON_LEFT
        ) == GLFW_PRESS;

    auto isHovered =
        [&](float x, float y, float w, float h)
        {
            return ndcX >= x &&
                ndcX <= x + w &&
                ndcY >= y &&
                ndcY <= y + h;
        };

    float btnW = 0.4f;
    float btnH = 0.1f;
    float btnX = -(btnW / 2.0f);

    if (clicked)
    {
        if (isHovered(btnX, 0.2f, btnW, btnH))
            return HUD::PauseButton::RESUME;

        if (isHovered(btnX, 0.0f, btnW, btnH))
            return HUD::PauseButton::SETTINGS;

        if (isHovered(btnX, -0.2f, btnW, btnH))
            return HUD::PauseButton::QUIT;
    }

    return HUD::PauseButton::NONE;
}

void HUD::DrawCredits()
{
    uiShader->Activate();
    glBindVertexArray(vao);

    // Dark blue background overlay
    DrawQuad(
        -1.0f, -1.0f,
        2.0f, 2.0f,
        glm::vec4(
            0.0f, 0.0f,
            0.05f, 0.92f)
    );

    // Render full credits image
    if (labelCreditsScreen)
    {
        labelCreditsScreen->Bind();

        glUniform2f(uniPos, -0.6f, -0.9f);
        glUniform2f(uniSize, 1.2f, 1.8f);

        glUniform1f(uniUVOffset, 0.0f);
        glUniform1f(uniUVWidth, 1.0f);

        glUniform1i(uniUseTexture, 1);
        glUniform1i(uniTex, 0);

        glDrawElements(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            0
        );
    }

    glBindVertexArray(0);
}

void HUD::DrawWaveNumber(int waveNumber, int totalWaves)
{
    // Wave progress bar
    const float barW = 0.3f;
    const float barH = 0.06f;
    const float barX = 0.65f;
    const float barY = 0.82f;
    const float pad = 0.007f;

    // Border
    DrawQuad(
        barX - pad,
        barY - pad,
        barW + pad * 2,
        barH + pad * 2,
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    // Background
    DrawQuad(
        barX,
        barY,
        barW,
        barH,
        glm::vec4(0.1f, 0.1f, 0.4f, 1.0f)
    );

    // Wave completion percentage
    float pct =
        (float)waveNumber / totalWaves;

    DrawQuad(
        barX,
        barY,
        barW * pct,
        barH,
        glm::vec4(0.3f, 0.4f, 1.0f, 1.0f)
    );

    // Draw current wave number
    if (digitTex)
    {
        const float h = 0.07f;

        const float gap =
            (h * 0.5f / ASPECT_RATIO) + 0.01f;

        float x = barX + 0.02f;
        float y = barY - 0.005f;

        int w1 = waveNumber / 10;
        int w2 = waveNumber % 10;

        if (w1 > 0)
            DrawDigit(w1, x, y, h);

        DrawDigit(
            w2,
            x + (w1 > 0 ? gap : 0),
            y,
            h
        );
    }
}

void HUD::DrawScore(int score)
{
    // Score display bar
    const float barX = 0.65f;
    const float barY = 0.72f;
    const float barW = 0.3f;
    const float barH = 0.05f;
    const float pad = 0.007f;

    DrawQuad(
        barX - pad,
        barY - pad,
        barW + pad * 2,
        barH + pad * 2,
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    DrawQuad(
        barX,
        barY,
        barW,
        barH,
        glm::vec4(0.15f, 0.1f, 0.0f, 1.0f)
    );

    DrawQuad(
        barX,
        barY,
        barW,
        barH,
        glm::vec4(0.6f, 0.5f, 0.0f, 1.0f)
    );

    // Draw score digits
    if (digitTex)
    {
        const float h = 0.06f;

        const float gap =
            (h * 0.5f / ASPECT_RATIO) + 0.008f;

        float x = barX + 0.02f;
        float y = barY - 0.003f;

        int digits[6];
        int temp = score;

        // Extract individual digits
        for (int i = 5; i >= 0; i--)
        {
            digits[i] = temp % 10;
            temp /= 10;
        }

        // Skip leading zeros
        bool leading = true;

        for (int i = 0; i < 6; i++)
        {
            if (leading &&
                digits[i] == 0 &&
                i < 5)
                continue;

            leading = false;

            DrawDigit(
                digits[i],
                x,
                y,
                h
            );

            x += gap;
        }
    }
}

void HUD::Draw(
    int currentHealth,
    int maxHealth,
    int framesRemaining,
    int waveNumber,
    int totalWaves,
    int score)
{
    // Enable alpha blending for HUD transparency
    glEnable(GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    uiShader->Activate();
    glBindVertexArray(vao);

    // Draw gameplay HUD elements
    DrawHealthBar(
        currentHealth,
        maxHealth
    );

    DrawTimer(framesRemaining);

    DrawWaveNumber(
        waveNumber,
        totalWaves
    );

    DrawScore(score);

    glBindVertexArray(0);
}