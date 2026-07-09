#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include "Character.h"
#include "Audio.h"

// Player-controlled character class.
// Extends Character by adding keyboard/mouse input,
// blocking mechanics, and player-specific combat behavior.
class Player : public Character
{
    public:
        // Creates a player at the specified starting position.
        Player(glm::vec3 startPos);

        // Updates player state each frame, including movement,
        // combat input, animations, and physics.
        void Update(GLFWwindow* window, int gameFrame, Audio& audio) override;

        // Applies incoming damage while taking blocking
        // and player-specific mechanics into account.
        void TakeDamage(int damage) override;

        // Returns the player's current hurtbox.
        // Can differ from the base Character hurtbox
        // depending on player state.
        AABB GetHurtbox() const override;

        // Indicates whether the current block is valid
        // for preventing incoming damage.
        bool blockValid;

    protected:
        // Handles player movement input and movement-related
        // state transitions.
        void ApplyMovement(GLFWwindow* window, int gameFrame, Audio& audio) override;

    private:
        // Stores the previous frame's mouse button state.
        // Used to detect fresh mouse clicks.
        bool lastMouseState;

        // Number of hits absorbed while blocking.
        int blockHits;

        // Indicates whether the player's guard has been broken.
        bool blockBroken;

        // Handles attack input and attack initiation.
        void ApplyAttackInput(GLFWwindow* window, Audio& audio);

        // Processes blocking input and block state logic.
        void ApplyBlock(GLFWwindow* window);
};

#endif