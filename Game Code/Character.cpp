#include "Character.h"
#include "Constants.h"

// Creates a character with default movement, animation,
// combat, and state values.
Character::Character(glm::vec3 startPos, int maxHealth)
{
    // Set initial world position.
    position = startPos;

    // Character starts stationary.
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);

    // Default orientation is facing right.
    facingRight = true;

    // Start in idle state.
    state = AnimState::IDLE;
    lastState = AnimState::IDLE;

    // Begin animation from first frame.
    animFrame = 0;

    // Animation speed based on game FPS.
    frameSpeed = FPS / 4;

    // Set initial health.
    health = maxHealth;

    // Initialize all animation texture pointers.
    for (int i = 0; i < 7; i++)
        textures[i] = nullptr;

    // Number of frames contained in each animation sprite sheet.
    frameCounts[(int)AnimState::IDLE] = 4;
    frameCounts[(int)AnimState::WALK] = 7;
    frameCounts[(int)AnimState::JUMP] = 5;
    frameCounts[(int)AnimState::ATTACK] = 6;
    frameCounts[(int)AnimState::BLOCK] = 1;
    frameCounts[(int)AnimState::HIT] = 2;
    frameCounts[(int)AnimState::KO] = 2;

    // Initialize combat-related timers and counters.
    attackTimer = 0;
    playerAttackCooldown = 4;
    enemyAttackCooldown = 8;
    attackDuration = 8;
    attackIndex = 0;
    stunTimer = 0;

    // No attack has connected yet.
    hasHit = false;

    // Animation timing starts at zero.
    frameTimer = 0;

    // No invincibility frames initially.
    invincibilityTimer = 0;
}

// Loads a texture corresponding to a particular animation state.
void Character::LoadTexture(AnimState animState, const char* imagePath)
{
    int index = (int)animState;

    // Remove any existing texture before replacing it.
    if (textures[index] != nullptr)
    {
        textures[index]->Delete();
        delete textures[index];
    }

    // Create and load the new texture.
    textures[index] = new Texture(imagePath, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
}

// Updates attack cooldown timing.
void Character::ApplyAttack()
{
    // Count down active attack timer.
    if (attackTimer > 0)
        attackTimer--;
    else
        // Once attack expires, allow future hits.
        hasHit = false;
}

// Returns the character's hurtbox,
// which represents the area that can receive damage.
AABB Character::GetHurtbox() const
{
    float w = CHARACTER_SCALE * 0.8f;
    float h = CHARACTER_SCALE * 1.0f;

    return {
        position.x - w / 2.0f,
        position.y - h / 2.0f,
        w,
        h
    };
}

// Returns the attack hitbox if an attack is active.
AABB Character::GetHitbox() const
{
    // No hitbox exists if not attacking.
    if (state != AnimState::ATTACK || !IsAttackActive())
        return { 0.0f, 0.0f, 0.0f, 0.0f };

    // Dimensions of the attack hitbox.
    float w = CHARACTER_SCALE * 0.6f;
    float h = CHARACTER_SCALE * 0.4f;

    // Width of the hurtbox used for positioning.
    float hurtW = CHARACTER_SCALE * 0.8f;

    // Place hitbox in front of the character depending on facing direction.
    float hitboxX = facingRight
        ? position.x + (hurtW / 2.0f)
        : position.x - (hurtW / 2.0f) - w;

    return {
        hitboxX,
        position.y - h / 2.0f,
        w,
        h
    };
}

// Determines whether the attack hitbox should be active.
bool Character::IsAttackActive() const
{
    // Entire attack state is treated as active.
    return state == AnimState::ATTACK;
}

// Applies incoming damage and updates state accordingly.
void Character::TakeDamage(int damage)
{
    // Ignore damage once already knocked out.
    if (state == AnimState::KO)
        return;

    // Reduce health.
    health -= damage;

    // Handle defeat condition.
    if (health <= 0)
    {
        health = 0;
        state = AnimState::KO;
    }
    else
    {
        // Enter hit-stun state.
        state = AnimState::HIT;
        stunTimer = 4;
    }
}

// Returns true once the KO animation has reached its final frame.
bool Character::IsKOComplete() const
{
    return state == AnimState::KO &&
        animFrame >= frameCounts[(int)AnimState::KO] - 1;
}

// Updates animation timing and frame progression.
void Character::UpdateAnimation()
{
    // Handle hit-stun duration.
    if (stunTimer > 0)
    {
        stunTimer--;

        // Return to idle after stun ends.
        if (stunTimer == 0 && state == AnimState::HIT)
            state = AnimState::IDLE;
    }

    // Detect animation state changes.
    if (state != lastState)
    {
        // Restart animation when changing state,
        // except for attack chains.
        if (state != AnimState::ATTACK)
            animFrame = 0;

        lastState = state;
        frameTimer = 0;
    }

    // KO animation plays more slowly for dramatic effect.
    int currentFrameSpeed =
        (state == AnimState::KO) ? frameSpeed * 4 : frameSpeed;

    frameTimer++;

    // Advance animation when enough frames have elapsed.
    if (frameTimer >= currentFrameSpeed)
    {
        frameTimer = 0;

        // KO animation does not loop.
        if (state == AnimState::KO)
        {
            int frames = frameCounts[(int)AnimState::KO];

            if (animFrame < frames - 1)
                animFrame++;

            return;
        }

        // Attack animation cycles within the currently selected attack pair.
        if (state == AnimState::ATTACK)
        {
            int attackFrameOffset = attackIndex * 2;

            animFrame =
                attackFrameOffset +
                ((animFrame - attackFrameOffset + 1) % 2);
        }
        else
        {
            // Normal animations loop continuously.
            int frames = frameCounts[(int)state];
            animFrame = (animFrame + 1) % frames;
        }
    }
}

// Draws the character using the current animation frame.
void Character::Draw(Shader& shader, GLuint uniModel, GLuint uniOffset, GLuint uniNumFrames)
{
    // Create model transformation matrix.
    glm::mat4 model = glm::mat4(1.0f);

    // Move character into world position.
    model = glm::translate(model, position);

    // Apply character scaling.
    model = glm::scale(model, glm::vec3(CHARACTER_SCALE, CHARACTER_SCALE, 1.0f));

    // Mirror sprite when facing left.
    if (!facingRight)
        model = glm::scale(model, glm::vec3(-1.0f, 1.0f, 1.0f));

    // Send transformation matrix to shader.
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

    // Retrieve animation information for current state.
    int frames = frameCounts[(int)state];
    Texture* tex = textures[(int)state];

    // Fallback to idle texture if current animation texture is missing.
    if (tex == nullptr)
    {
        tex = textures[(int)AnimState::IDLE];

        // Use matching idle frame count.
        frames = frameCounts[(int)AnimState::IDLE];
    }

    // Select the correct frame from the sprite sheet.
    glUniform1f(uniOffset, animFrame * (1.0f / frames));

    // Inform shader how many frames exist in the sheet.
    glUniform1i(uniNumFrames, frames);

    // Bind texture before drawing.
    if (tex != nullptr)
    {
        tex->Bind();
        tex->texUnit(shader, "sprite", 0);
    }

    // Render the sprite quad.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}