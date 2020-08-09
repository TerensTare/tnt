#include "fileIO/AudioPlayer.hpp"
#include "fileIO/AssetCache.hpp"

#include "utils/Logger.hpp"

inline tnt::small_music_cache *music{tnt::default_music_cache()};
inline tnt::small_sfx_cache *sfx{tnt::default_sfx_cache()};

tnt::AudioPlayer::AudioPlayer(int frequency, unsigned short format, int channels, int chunksize) noexcept
{
    if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0)
        logger::error("Couldn't initialize SDL_Mixer!! Error: {}", Mix_GetError());
}

tnt::AudioPlayer::~AudioPlayer() noexcept
{
    Mix_CloseAudio();
}

void tnt::AudioPlayer::PlayMusic(std::string_view filename, int loops)
{
    if (Mix_Music * mus{music->get(filename)}; mus != nullptr)
        Mix_PlayMusic(mus, loops);
    else
        logger::info("Couldn't load {}\n!!Error: {}", Mix_GetError());
}

void tnt::AudioPlayer::PauseMusic()
{
    if (Mix_PlayingMusic() != 0)
        Mix_PauseMusic();
}

void tnt::AudioPlayer::ResumeMusic()
{
    if (Mix_PausedMusic() != 0)
        Mix_ResumeMusic();
}

void tnt::AudioPlayer::PlaySFX(std::string_view filename, int channel, int loops)
{
    if (Mix_Chunk * chunk{sfx->get(filename)}; chunk != nullptr)
        Mix_PlayChannel(channel, chunk, loops);
    else
        logger::info("Couldn't load {}\n!!Error: {}", Mix_GetError());
}