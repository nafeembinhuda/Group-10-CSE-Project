#include "Player.h"
#include "Constants.h"
#include "Audio.h"

// Initializes player-specific combat and blocking variables.
Player::Player(glm::vec3 startPos) : Character(startPos, 100)
{
    lastMouseState = false;
    attackIndex = 0;
    blockHits = 0;
    blockValid = true;
    blockBroken = false;
}

// Handles incoming damage, including block mechanics,
// block breaking, invincibility frames, and KO logic.
void Player::TakeDamage(int damage)
{
    // Ignore damage while temporarily invincible.
    if (invincibilityTimer > 0) return;

    // Ignore damage if already defeated.
    if (state == AnimState::KO) return;

    // Blocking can negate several hits before breaking.
    if (state == AnimState::BLOCK && blockValid)
    {
        blockHits++;

        // First three blocked hits deal no damage.
        if (blockHits < 4)
        {
            return;
        }
        else
        {
            // Fourth blocked hit breaks the guard.
            blockBroken = true;
            state = AnimState::HIT;
            health -= damage;
            stunTimer = attackDuration;
        }
    }
    else
    {
        // Use default damage behaviour.
        Character::TakeDamage(damage);
    }

    // Ensure health never becomes negative.
    if (health <= 0)
    {
        health = 0;
        state = AnimState::KO;
    }
}

// Reduces the player's hurtbox while jumping,
// making aerial movement slightly safer.
AABB Player::GetHurtbox() const
{
    if (state == AnimState::JUMP)
    {
        float w = CHARACTER_SCALE * 0.8f;
        float h = CHARACTER_SCALE * 0.5f;

        return {
            position.x - w / 2.0f,
            position.y,
            w,
            h
        };
    }

    return Character::GetHurtbox();
}

// Handles entering, maintaining, and exiting the block state.
void Player::ApplyBlock(GLFWwindow* window)
{
    // Reset block break status once the hit reaction finishes.
    if (blockBroken && state != AnimState::HIT)
        blockBroken = false;

    bool blocking =
        glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    // Enter blocking state if allowed.
    if (blocking && state != AnimState::JUMP && !blockBroken)
    {
        state = AnimState::BLOCK;
    }
    // Return to movement or idle when block is released.
    else if (state == AnimState::BLOCK && !blocking)
    {
        blockHits = 0;

        bool moving =
            glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;

        state = moving ? AnimState::WALK : AnimState::IDLE;
    }
}

// Handles player movement, jumping, gravity,
// landing behaviour, and movement sound effects.
void Player::ApplyMovement(GLFWwindow* window, int gameFrame, Audio& audio)
{
    bool movingRight =
        glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;

    bool movingLeft =
        glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;

    // Move right.
    if (movingRight)
    {
        position.x += X_VELOCITY;
        facingRight = true;

        if (state != AnimState::JUMP &&
            state != AnimState::ATTACK &&
            state != AnimState::BLOCK)
        {
            state = AnimState::WALK;
        }
    }
    // Move left.
    else if (movingLeft)
    {
        position.x -= X_VELOCITY;
        facingRight = false;

        if (state != AnimState::JUMP &&
            state != AnimState::ATTACK &&
            state != AnimState::BLOCK)
        {
            state = AnimState::WALK;
        }
    }
    else
    {
        if (state == AnimState::WALK)
            state = AnimState::IDLE;
    }

    // Prevent the player from leaving the visible arena.
    position.x = glm::clamp(
        position.x,
        -ASPECT_RATIO + 0.1f,
        ASPECT_RATIO - 0.1f
    );

    // Play walking sound periodically while grounded.
    if (state == AnimState::WALK &&
        position.y <= GROUND_LEVEL &&
        gameFrame % 10 == 0)
    {
        audio.PlaySFX("Audio/walk.wav");
    }

    // Jump only when standing on the ground.
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS &&
        position.y <= GROUND_LEVEL &&
        state != AnimState::BLOCK)
    {
        velocity.y = Y_VELOCITY;
        state = AnimState::JUMP;

        audio.PlaySFX("Audio/jump.wav");
    }

    // Apply gravity while airborne.
    if (position.y > GROUND_LEVEL || velocity.y > 0.0f)
    {
        velocity.y -= GRAVITY;
        position.y += velocity.y;
    }

    // Landing logic.
    if (position.y <= GROUND_LEVEL)
    {
        position.y = GROUND_LEVEL;
        velocity.y = 0.0f;

        if (state == AnimState::JUMP)
        {
            state = AnimState::IDLE;

            // Brief invulnerability after landing.
            invincibilityTimer = 8;
        }
    }
}

// Handles mouse-click attack input,
// attack combo progression, and attack recovery.
void Player::ApplyAttackInput(GLFWwindow* window, Audio& audio)
{
    bool currentMouseState =
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    bool freshClick = currentMouseState && !lastMouseState;
    lastMouseState = currentMouseState;

    // Reduce attack cooldown timer.
    if (attackTimer > 0)
        attackTimer--;

    // Start a new attack if the cooldown has finished.
    if (freshClick &&
        attackTimer == 0 &&
        state != AnimState::BLOCK)
    {
        state = AnimState::ATTACK;

        // Cycle through the three attack animations.
        attackIndex = (attackIndex + 1) % 3;

        // Each attack occupies two frames in the attack sprite sheet.
        animFrame = attackIndex * 2;

        attackTimer = attackDuration + playerAttackCooldown;
        hasHit = false;
    }

    // End attack once recovery frames have completed.
    if (state == AnimState::ATTACK &&
        attackTimer <= playerAttackCooldown)
    {
        if (position.y > GROUND_LEVEL)
        {
            state = AnimState::JUMP;
        }
        else
        {
            bool moving =
                glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;

            state = moving ? AnimState::WALK : AnimState::IDLE;
        }
    }
}

// Main per-frame update for the player.
void Player::Update(GLFWwindow* window, int gameFrame, Audio& audio)
{
    // Update temporary invulnerability timer.
    if (invincibilityTimer > 0)
        invincibilityTimer--;

    ApplyAttackInput(window, audio);
    ApplyBlock(window);
    ApplyMovement(window, gameFrame, audio);

    // Advance animation state.
    UpdateAnimation();
}