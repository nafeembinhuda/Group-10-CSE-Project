#ifndef CHARACTER_CLASS_H
#define CHARACTER_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "Audio.h"

// Represents all possible animation states a character can be in.
enum class AnimState {
    IDLE,    // Standing still
    WALK,    // Moving horizontally
    JUMP,    // Airborne state
    ATTACK,  // Performing an attack
    BLOCK,   // Defending against attacks
    HIT,     // Temporarily stunned after taking damage
    KO       // Knocked out / defeated
};

// Axis-Aligned Bounding Box used for collision detection.
struct AABB {
    float x, y, width, height;

    // Returns true if this bounding box overlaps another.
    bool Intersects(const AABB& other) const {
        return x < other.x + other.width &&
            x + width > other.x &&
            y < other.y + other.height &&
            y + height > other.y;
    }
};

// Abstract base class for all game characters.
// Provides shared functionality for player and enemy classes.
class Character {
    public:
        // World position of the character.
        glm::vec3 position;

        // Current movement velocity.
        glm::vec3 velocity;

        // Determines sprite orientation.
        // true = facing right, false = facing left.
        bool facingRight;

        // Current animation/state of the character.
        AnimState state;

        // Current health value.
        int health;

        // Prevents multiple hits from registering during one attack.
        bool hasHit;

        // Creates a character at the given position with specified health.
        Character(glm::vec3 startPos, int maxHealth);

        // Loads a texture corresponding to a specific animation state.
        void LoadTexture(AnimState state, const char* imagePath);

        // Updates character logic every frame.
        // Must be implemented by derived classes.
        virtual void Update(GLFWwindow* window, int gameFrame, Audio& audio) = 0;

        // Renders the character using the currently active animation.
        void Draw(Shader& shader, GLuint uniModel, GLuint uniOffset, GLuint uniNumFrames);

        // Returns the character's hurtbox (damage-receiving area).
        virtual AABB GetHurtbox() const;

        // Returns the active hitbox (damage-dealing area).
        AABB GetHitbox() const;

        // Returns true if an attack is currently capable of hitting.
        bool IsAttackActive() const;

        // Applies incoming damage to the character.
        virtual void TakeDamage(int damage);

        // Returns true if the character still has health remaining.
        bool IsAlive() { return health > 0; }

        // Returns the remaining invincibility frames.
        int GetInvincibilityTimer() const { return invincibilityTimer; }

        // Returns true when the KO animation has fully finished.
        bool IsKOComplete() const;

        // Updates animation frame progression.
        void UpdateAnimation();

    protected:
        // Animation textures indexed by AnimState.
        Texture* textures[7];

        // Number of frames in each animation.
        int frameCounts[7];

        // Current animation frame index.
        int animFrame;

        // Controls animation playback speed.
        int frameSpeed;

        // Counts frames until animation advances.
        int frameTimer;

        // Tracks attack timing.
        int attackTimer;

        // Cooldown timer for player attacks.
        int playerAttackCooldown;

        // Cooldown timer for enemy attacks.
        int enemyAttackCooldown;

        // Duration of an attack animation/action.
        int attackDuration;

        // Index used to identify attack stages or attack type.
        int attackIndex;

        // Duration of hit-stun after taking damage.
        int stunTimer;

        // Stores the previous animation state.
        AnimState lastState;

        // Number of invincibility frames remaining.
        int invincibilityTimer;

        // Handles movement and state changes.
        // Must be implemented by derived classes.
        virtual void ApplyMovement(GLFWwindow* window, int gameFrame, Audio& audio) = 0;

        // Processes attack logic and timing.
        void ApplyAttack();
};

#endif