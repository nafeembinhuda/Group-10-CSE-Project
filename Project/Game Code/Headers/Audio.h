#ifndef AUDIO_CLASS_H
#define AUDIO_CLASS_H

// Disable PulseAudio and JACK backends in miniaudio.
// This forces miniaudio to use other available audio backends.
#define MA_NO_PULSEAUDIO
#define MA_NO_JACK
#include "miniaudio.h"

#include <string>
#include <vector>

// Handles all audio-related functionality including:
// - Background music
// - Pause menu music
// - Sound effects (SFX)
// - Volume management
class Audio {
    public:
        // Initializes the audio engine and default settings.
        Audio();

        // Cleans up audio resources and shuts down the audio engine.
        ~Audio();

        // =========================
        // Music Controls
        // =========================

        // Loads and plays background music.
        // If loop is true, the track repeats indefinitely.
        void PlayMusic(const char* path, bool loop = true);

        // Stops the currently playing background music.
        void StopMusic();

        // Pauses the currently playing background music.
        void PauseMusic();

        // Resumes paused background music.
        void ResumeMusic();

        // Plays pause-menu music or toggles its playback state.
        void PlayPauseMusic(const char* path);

        // Stops pause-menu music playback.
        void StopPauseMusic();

        // =========================
        // Sound Effect Controls
        // =========================

        // Plays a one-shot sound effect.
        void PlaySFX(const char* path);

        // =========================
        // Volume Controls
        // =========================

        // Sets the overall volume applied to all audio.
        void SetMasterVolume(float volume);

        // Sets the volume level for music tracks.
        void SetMusicVolume(float volume);

        // Sets the volume level for sound effects.
        void SetSFXVolume(float volume);

        // =========================
        // Getters
        // =========================

        // Returns the current master volume.
        float GetMasterVolume() const { return masterVolume; }

        // Returns the current music volume.
        float GetMusicVolume()  const { return musicVolume; }

        // Returns the current sound effect volume.
        float GetSFXVolume()    const { return sfxVolume; }

        // Returns true if background music is currently playing.
        bool IsMusicPlaying() const;

    private:
        // Main miniaudio engine responsible for audio playback.
        ma_engine engine;

        // Background music sound object.
        ma_sound  musicSound;

        // Indicates whether a music file is currently loaded.
        bool      musicLoaded;

        // Separate sound object used for pause-menu music.
        ma_sound pauseSound;

        // Indicates whether pause music has been loaded.
        bool     pauseLoaded;

        // Global volume multiplier applied to all audio.
        float masterVolume;

        // Volume multiplier for music playback.
        float musicVolume;

        // Volume multiplier for sound effects.
        float sfxVolume;

        // Stores dynamically created sound effect instances.
        // Used so they can be cleaned up after playback.
        std::vector<ma_sound*> sfxSounds;

        // Removes and releases finished sound effect instances
        // to prevent memory leaks.
        void CleanupSFX();
};

#endif