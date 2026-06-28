#ifndef ENEMY_CLASS_H
#define ENEMY_CLASS_H

#include "Character.h"
#include "Audio.h"

// Enemy character controlled by simple AI behavior.
// Inherits all movement, animation, combat, and rendering
// functionality from the Character base class.
class Enemy : public Character {
    public:
        // Creates an enemy at the specified starting position.
        Enemy(glm::vec3 startPos);

        // Updates enemy logic each frame, including movement,
        // AI decisions, attacks, and animation state changes.
        void Update(GLFWwindow* window, int gameFrame, Audio& audio) override;

        // Updates the enemy's knowledge of the player's position.
        // Used by the AI to determine movement and attack behavior.
        void SetPlayerPos(glm::vec3 pos) { playerPos = pos; }

    protected:
        // Handles AI-controlled movement and state transitions.
        void ApplyMovement(GLFWwindow* window, int gameFrame, Audio& audio) override;

    private:
        // Maximum distance at which the enemy begins tracking the player.
        float detectionRange;

        // Distance within which the enemy can perform attacks.
        float attackRange;

        // Last known player position used for AI calculations.
        glm::vec3 playerPos;

        // Delay timer used to slow AI reaction speed
        // and make behavior appear more natural.
        int aiDelay;

        // Handles attack decision-making and attack initiation.
        void ApplyAttackAI();
};

#endif