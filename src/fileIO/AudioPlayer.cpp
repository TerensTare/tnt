#include "core/AudioPlayer.hpp"
#include "fileIO/AssetManager.hpp"
#include <iostream>

tnt::AudioPlayer::AudioPlayer(int frequency, unsigned short format, int channels, int chunksize)
{
    if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0)
        std::cout << "Couldn't initialize SDL_Mixer!!\nError: " << Mix_GetError() << std::endl;
}

void tnt::AudioPlayer::PlayMusic(std::string_view filename, int loops)
{
    if (AssetManager::This().Music(filename) == nullptr) {
        AssetManager::This().AddMusic(filename);
        if (AssetManager::This().Music(filename) == nullptr) {
            std::cout << "Couldn't load " << filename << "\n.Error: " << Mix_GetError() << std::endl;
            return;
        }
    }

    Mix_PlayMusic(AssetManager::This().Music(filename), loops);
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
    if (AssetManager::This().Sfx(filename) == nullptr) {
        AssetManager::This().AddSfx(filename);
        if (AssetManager::This().Sfx(filename) != nullptr) {
            std::cout << "Couldn't load " << filename << "\nError: " << Mix_GetError() << std::endl;
            return;
        }
    }

    Mix_PlayChannel(channel, AssetManager::This().Sfx(filename), loops);
}