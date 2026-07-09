#include "WaveManager.h"
#include "Constants.h"
#include <algorithm>
#include <cstdlib>

// Initializes wave tracking variables.
WaveManager::WaveManager(int totalWaves)
    : currentWave(0), totalWaves(totalWaves), waveStartFrame(0), waveFrame(0)
{
}

// Returns configuration values for a given wave.
// Currently controls enemy count and provides placeholders
// for future health/damage scaling.
WaveConfig WaveManager::GetWaveConfig(int waveNumber)
{
    float difficultyMultiplier = 1.0f + (waveNumber * 0.15f);

    WaveConfig config;
    config.enemyBaseHealth = 25;
    config.enemyBaseDamage = 4;

    // Waves 0-3 (displayed as waves 1-4)
    if (waveNumber <= 3)
        config.enemyCount = 16;

    // Waves 4-6 (displayed as waves 5-7)
    else if (waveNumber <= 6)
        config.enemyCount = 20;

    // Waves 7+ (displayed as waves 8-15)
    else
        config.enemyCount = 25;

    return config;
}

// Generates a random spawn position near either edge of the arena.
glm::vec3 WaveManager::GetSpawnPosition(int enemyIndex, int totalEnemies)
{
    // Randomly choose left or right side.
    bool spawnRight = rand() % 2 == 0;

    float xPos;
    if (spawnRight)
        xPos = ASPECT_RATIO - 0.1f - ((float)rand() / RAND_MAX) * 0.3f;
    else
        xPos = -ASPECT_RATIO + 0.1f + ((float)rand() / RAND_MAX) * 0.3f;

    return glm::vec3(xPos, GROUND_LEVEL, 0.0f);
}

// Creates an enemy and loads all required animation textures.
Enemy* WaveManager::CreateEnemy(glm::vec3 pos)
{
    Enemy* newEnemy = new Enemy(pos);

    newEnemy->LoadTexture(AnimState::IDLE, "Resources/Textures/Enemy/enemyIdle.png");
    newEnemy->LoadTexture(AnimState::WALK, "Resources/Textures/Enemy/enemyWalk.png");
    newEnemy->LoadTexture(AnimState::ATTACK, "Resources/Textures/Enemy/enemyAttack.png");
    newEnemy->LoadTexture(AnimState::HIT, "Resources/Textures/Enemy/enemyHit.png");
    newEnemy->LoadTexture(AnimState::KO, "Resources/Textures/Enemy/enemyKO.png");

    return newEnemy;
}

// Clears the previous wave and schedules enemies for the new wave.
void WaveManager::SpawnWave(int waveNumber)
{
    // Remove active enemies from previous wave.
    for (auto enemy : enemies)
    {
        delete enemy;
    }

    // Remove enemies waiting to spawn.
    for (auto& pe : pendingEnemies)
    {
        delete pe.enemy;
    }

    enemies.clear();
    pendingEnemies.clear();

    currentWave = waveNumber;
    waveFrame = 0;

    WaveConfig config = GetWaveConfig(waveNumber);

    // Spawn enemies over the first 90 seconds of the wave.
    const int spawnWindow = 90 * FPS;

    // Minimum spacing between enemy spawns.
    const int minGap = 4 * FPS;

    // First wave begins with two enemies already active.
    int immediateCount = (waveNumber == 0) ? 2 : 0;

    // Stores spawn frames in ascending order.
    std::vector<int> spawnTimes;

    // Generate spawn times while enforcing minimum spacing.
    for (int i = immediateCount; i < config.enemyCount; i++)
    {
        int earliest = spawnTimes.empty() ? 0 : spawnTimes.back() + minGap;
        int latest = spawnWindow - (config.enemyCount - i - 1) * minGap;

        // Prevent invalid random ranges.
        if (earliest > latest)
            earliest = latest;

        int frame = earliest +
            (latest > earliest ? rand() % (latest - earliest) : 0);

        spawnTimes.push_back(frame);
    }

    // Create all enemies for the wave.
    for (int i = 0; i < config.enemyCount; i++)
    {
        glm::vec3 spawnPos = GetSpawnPosition(i, config.enemyCount);
        Enemy* newEnemy = CreateEnemy(spawnPos);

        // Spawn the first two enemies immediately in wave one.
        if (waveNumber == 0 && i < 2)
        {
            enemies.push_back(newEnemy);
        }
        else
        {
            PendingEnemy pe;
            pe.enemy = newEnemy;
            pe.spawnFrame = spawnTimes[i - immediateCount];

            pendingEnemies.push_back(pe);
        }
    }
}

// Moves scheduled enemies into the active enemy list
// once their spawn time is reached.
void WaveManager::CheckPendingSpawns()
{
    for (int i = (int)pendingEnemies.size() - 1; i >= 0; i--)
    {
        if (waveFrame >= pendingEnemies[i].spawnFrame)
        {
            enemies.push_back(pendingEnemies[i].enemy);
            pendingEnemies.erase(pendingEnemies.begin() + i);
        }
    }
}

// Removes enemies whose KO animation has finished.
void WaveManager::CleanupDeadEnemies()
{
    for (int i = (int)enemies.size() - 1; i >= 0; i--)
    {
        if (enemies[i]->IsKOComplete())
        {
            delete enemies[i];
            enemies.erase(enemies.begin() + i);
        }
    }
}

// Updates wave timing, handles spawning,
// removes defeated enemies, and updates all active enemies.
void WaveManager::Update(GLFWwindow* window, int gameFrame, Audio& audio)
{
    waveFrame++;

    CheckPendingSpawns();
    CleanupDeadEnemies();

    for (auto enemy : enemies)
    {
        enemy->Update(window, gameFrame, audio);
    }
}

// Draws all active enemies.
void WaveManager::Draw(Shader& shader, GLuint uniModel, GLuint uniOffset, GLuint uniNumFrames)
{
    for (auto enemy : enemies)
    {
        enemy->Draw(shader, uniModel, uniOffset, uniNumFrames);
    }
}

// Updates the player position used by enemy AI.
void WaveManager::SetPlayerPos(glm::vec3 pos)
{
    for (auto enemy : enemies)
    {
        enemy->SetPlayerPos(pos);
    }
}

// A wave is complete when there are no active enemies
// and no enemies waiting to spawn.
bool WaveManager::IsWaveComplete() const
{
    return pendingEnemies.empty() && enemies.empty();
}

// The game is complete when the final wave has finished.
bool WaveManager::IsGameComplete() const
{
    return currentWave >= totalWaves && IsWaveComplete();
}

// Returns the total number of enemies still remaining
// in the current wave, including pending spawns.
int WaveManager::GetEnemiesRemaining() const
{
    return (int)enemies.size() + (int)pendingEnemies.size();
}