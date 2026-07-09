#ifndef WAVE_MANAGER_H
#define WAVE_MANAGER_H

#include <vector>
#include <glm/glm.hpp>
#include "Enemy.h"
#include "Audio.h"

// Defines the properties of a wave.
// Used to determine enemy count and enemy strength
// for a particular wave number.
struct WaveConfig {
    int enemyCount;
    int enemyBaseHealth;
    int enemyBaseDamage;
};

// Stores enemies that have not yet entered the game world.
// Each enemy becomes active once its spawn frame is reached.
struct PendingEnemy {
    Enemy* enemy;

    // Wave-relative frame at which the enemy should spawn.
    int spawnFrame;
};

// Manages wave progression, enemy spawning,
// wave completion, and enemy lifecycle management.
class WaveManager {
    public:
        // Creates a wave manager with the specified number of waves.
        WaveManager(int totalWaves = 15);

        // Creates all enemies for a wave and schedules their spawns.
        void SpawnWave(int waveNumber);

        // Updates active enemies and handles pending spawns.
        void Update(GLFWwindow* window, int gameFrame, Audio& audio);

        // Draws all currently active enemies.
        void Draw(Shader& shader, GLuint uniModel, GLuint uniOffset, GLuint uniNumFrames);

        // ----- Getters -----

        // Returns true when all enemies in the current wave
        // have been defeated and no pending spawns remain.
        bool IsWaveComplete() const;

        // Returns true when the final wave has been completed.
        bool IsGameComplete() const;

        // Returns true if there are enemies waiting to spawn.
        bool HasPendingEnemies() const { return !pendingEnemies.empty(); }

        // Returns the current wave number.
        int GetCurrentWave() const { return currentWave; }

        // Returns the total number of waves in the game.
        int GetTotalWaves() const { return totalWaves; }

        // Returns the number of active enemies remaining.
        int GetEnemiesRemaining() const;

        // ----- Setters -----

        // Updates all enemies with the player's current position
        // so their AI can react accordingly.
        void SetPlayerPos(glm::vec3 pos);

        // Provides direct access to active enemies for
        // collision detection and combat logic.
        std::vector<Enemy*>& GetEnemies() { return enemies; }

    private:
        // Currently active enemies.
        std::vector<Enemy*> enemies;

        // Enemies scheduled to spawn later in the wave.
        std::vector<PendingEnemy> pendingEnemies;

        // Current wave number.
        int currentWave;

        // Total number of waves in the game.
        int totalWaves;

        // Global frame when the current wave began.
        int waveStartFrame;

        // Number of frames elapsed since the current wave started.
        int waveFrame;

        // Returns configuration values for the specified wave.
        WaveConfig GetWaveConfig(int waveNumber);

        // Calculates an appropriate spawn position for an enemy.
        glm::vec3 GetSpawnPosition(int enemyIndex, int totalEnemies);

        // Removes defeated enemies and frees their resources.
        void CleanupDeadEnemies();

        // Moves enemies from the pending list into the active list
        // once their spawn frame is reached.
        void CheckPendingSpawns();

        // Creates and initializes an enemy at the specified position.
        Enemy* CreateEnemy(glm::vec3 pos);
};

#endif