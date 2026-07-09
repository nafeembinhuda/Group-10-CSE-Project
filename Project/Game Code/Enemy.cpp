#include "Enemy.h"
#include "Constants.h"

// Creates an enemy character with predefined AI parameters.
Enemy::Enemy(glm::vec3 startPos) : Character(startPos, 20)
{
    // Maximum distance at which the enemy notices the player.
    detectionRange = 2.0f;

    // Distance required before an attack can be triggered.
    attackRange = CHARACTER_SCALE * 1.0f;

    // Initialize player position to the origin.
    playerPos = glm::vec3(0.0f);

    // Enemy uses the first attack animation set by default.
    attackIndex = 0;

    // Delay before the enemy becomes active.
    // Useful for giving the player time at the start of a round.
    aiDelay = 60;
}

// Handles enemy movement and basic chase behavior.
void Enemy::ApplyMovement(GLFWwindow* window, int gameFrame, Audio& audio)
{
    // Enemy cannot move while defeated, stunned, or attacking.
    if (state == AnimState::KO ||
        state == AnimState::HIT ||
        state == AnimState::ATTACK)
        return;

    // Horizontal distance to the player.
    float dist = playerPos.x - position.x;

    // Absolute distance regardless of direction.
    float absDist = glm::abs(dist);

    // Move toward the player if detected but not yet in attack range.
    if (absDist < detectionRange && absDist > attackRange)
    {
        if (dist > 0)
        {
            position.x += 0.005f;
            facingRight = true;
        }
        else
        {
            position.x -= 0.005f;
            facingRight = false;
        }

        state = AnimState::WALK;
    }
    // Remain idle if the player is outside detection range.
    else if (absDist >= detectionRange)
    {
        state = AnimState::IDLE;
    }

    // Prevent the enemy from moving beyond screen boundaries.
    position.x = glm::clamp(
        position.x,
        -ASPECT_RATIO + 0.1f,
        ASPECT_RATIO - 0.1f
    );
}

// Handles attack decision-making and attack state transitions.
void Enemy::ApplyAttackAI()
{
    // Cannot attack while defeated or stunned.
    if (state == AnimState::KO || state == AnimState::HIT)
        return;

    // Distance to the player.
    float dist = glm::abs(playerPos.x - position.x);

    // Start an attack if:
    // - Player is within attack range
    // - Attack cooldown has expired
    // - Player is on or near the ground
    if (dist <= attackRange &&
        attackTimer == 0 &&
        playerPos.y <= GROUND_LEVEL + 0.01f)
    {
        state = AnimState::ATTACK;

        // Restart attack animation.
        animFrame = 0;

        // Set attack duration plus cooldown.
        attackTimer = attackDuration + enemyAttackCooldown;

        // Allow this attack to register a hit.
        hasHit = false;
    }

    // Once the attack animation has effectively finished,
    // return to movement behavior.
    if (state == AnimState::ATTACK &&
        attackTimer <= enemyAttackCooldown)
    {
        float absDist = glm::abs(playerPos.x - position.x);

        state = (absDist < detectionRange)
            ? AnimState::WALK
            : AnimState::IDLE;
    }

    // Update attack timer and hit state.
    ApplyAttack();
}

// Main enemy update function called every frame.
void Enemy::Update(GLFWwindow* window, int gameFrame, Audio& audio)
{
    // Wait for the initial AI delay before activating.
    if (aiDelay > 0)
    {
        aiDelay--;
        return;
    }

    // Update movement behavior.
    ApplyMovement(window, gameFrame, audio);

    // Update attack behavior.
    ApplyAttackAI();

    // Advance animations.
    UpdateAnimation();
}