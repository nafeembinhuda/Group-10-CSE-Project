// Enables miniaudio's implementation.
// This must appear in exactly one source file before including Audio.h.
#define MINIAUDIO_IMPLEMENTATION
#include "Audio.h"
#include <iostream>
#include <glm/glm.hpp>

Audio::Audio()
{
    // Initialize default volume levels.
    masterVolume = 1.0f;
    musicVolume = 1.0f;
    sfxVolume = 1.0f;

    // No audio files are loaded initially.
    musicLoaded = false;
    pauseLoaded = false;

    memset(&musicSound, 0, sizeof(musicSound));
    memset(&pauseSound, 0, sizeof(pauseSound));

    // Initialize the miniaudio engine.
    ma_result result = ma_engine_init(NULL, &engine);

    // Display an error message if initialization fails.
    if (result != MA_SUCCESS)
        std::cout << "Failed to initialize audio engine\n";
}

Audio::~Audio()
{
    // Stop and unload all audio resources before shutting down.
    StopMusic();
    CleanupSFX();
    StopPauseMusic();

    // Release the audio engine.
    ma_engine_uninit(&engine);
}

void Audio::PlayMusic(const char* path, bool loop)
{
    // Stop and unload any music currently playing.
    if (musicLoaded)
    {
        ma_sound_stop(&musicSound);
        ma_sound_uninit(&musicSound);
        musicLoaded = false;
    }

    // Load the music file as a streamed sound.
    // Streaming reduces memory usage for large audio tracks.
    ma_result result = ma_sound_init_from_file(
        &engine, path,
        MA_SOUND_FLAG_STREAM,
        NULL, NULL, &musicSound
    );

    // Abort if loading failed.
    if (result != MA_SUCCESS)
    {
        std::cout << "Failed to load music: " << path << "\n";
        return;
    }

    // Configure looping behavior.
    ma_sound_set_looping(&musicSound, loop ? MA_TRUE : MA_FALSE);

    // Apply current volume settings.
    ma_sound_set_volume(&musicSound, musicVolume * masterVolume);

    // Start playback.
    ma_sound_start(&musicSound);

    // Mark music as loaded.
    musicLoaded = true;
}

void Audio::StopMusic()
{
    // Stop and unload the current music track if one exists.
    if (musicLoaded)
    {
        ma_sound_stop(&musicSound);
        ma_sound_uninit(&musicSound);
        musicLoaded = false;
    }
}

void Audio::PauseMusic()
{
    // Stop playback without unloading the music resource.
    if (musicLoaded)
        ma_sound_stop(&musicSound);
}

void Audio::ResumeMusic()
{
    // Resume playback from the paused position.
    if (musicLoaded)
        ma_sound_start(&musicSound);
}

void Audio::PlayPauseMusic(const char* path)
{
    // Unload any existing pause-menu music.
    if (pauseLoaded)
    {
        ma_sound_stop(&pauseSound);
        ma_sound_uninit(&pauseSound);
        pauseLoaded = false;
    }

    // Load the pause-menu music track.
    ma_result result = ma_sound_init_from_file(
        &engine, path,
        MA_SOUND_FLAG_STREAM,
        NULL, NULL, &pauseSound
    );

    // Abort if loading fails.
    if (result != MA_SUCCESS)
    {
        std::cout << "Failed to load pause music: " << path << "\n";
        return;
    }

    // Pause music should always loop.
    ma_sound_set_looping(&pauseSound, MA_TRUE);

    // Apply volume settings.
    ma_sound_set_volume(&pauseSound, musicVolume * masterVolume);

    // Start playback.
    ma_sound_start(&pauseSound);

    // Mark pause music as loaded.
    pauseLoaded = true;
}

void Audio::StopPauseMusic()
{
    // Stop and unload pause-menu music if loaded.
    if (pauseLoaded)
    {
        ma_sound_stop(&pauseSound);
        ma_sound_uninit(&pauseSound);
        pauseLoaded = false;
    }
}

void Audio::PlaySFX(const char* path)
{
    // Create a new sound effect instance dynamically.
    ma_sound* sfx = new ma_sound();

    // Load the sound effect asynchronously to avoid game stutters.
    ma_result result = ma_sound_init_from_file(
        &engine, path,
        MA_SOUND_FLAG_ASYNC,
        NULL, NULL, sfx
    );

    // Clean up and exit if loading fails.
    if (result != MA_SUCCESS)
    {
        std::cout << "Failed to load SFX: " << path << "\n";
        delete sfx;
        return;
    }

    // Apply current sound effect volume settings.
    ma_sound_set_volume(sfx, sfxVolume * masterVolume);

    // Begin playback immediately.
    ma_sound_start(sfx);

    // Store the sound instance for later cleanup.
    sfxSounds.push_back(sfx);

    // Remove finished sound effects from memory.
    CleanupSFX();
}

void Audio::SetMasterVolume(float volume)
{
    // Clamp volume to the valid range [0, 1].
    masterVolume = glm::clamp(volume, 0.0f, 1.0f);

    // Apply master volume to the entire audio engine.
    ma_engine_set_volume(&engine, masterVolume);
}

void Audio::SetMusicVolume(float volume)
{
    // Clamp music volume to the valid range [0, 1].
    musicVolume = glm::clamp(volume, 0.0f, 1.0f);

    // Update currently playing music immediately.
    if (musicLoaded)
        ma_sound_set_volume(&musicSound, musicVolume * masterVolume);
}

void Audio::SetSFXVolume(float volume)
{
    // Clamp sound effect volume to the valid range [0, 1].
    sfxVolume = glm::clamp(volume, 0.0f, 1.0f);
}

bool Audio::IsMusicPlaying() const
{
    // No music loaded means nothing can be playing.
    if (!musicLoaded)
        return false;

    // Query miniaudio for playback state.
    return ma_sound_is_playing(&musicSound);
}

void Audio::CleanupSFX()
{
    // Iterate backwards so elements can be safely erased.
    for (int i = (int)sfxSounds.size() - 1; i >= 0; i--)
    {
        // Remove sound effects that have finished playing.
        if (!ma_sound_is_playing(sfxSounds[i]))
        {
            // Release miniaudio resources.
            ma_sound_uninit(sfxSounds[i]);

            // Free dynamically allocated memory.
            delete sfxSounds[i];

            // Remove the pointer from the container.
            sfxSounds.erase(sfxSounds.begin() + i);
        }
    }
}