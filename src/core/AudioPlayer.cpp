#include "core/AudioPlayer.hpp"

tnt::AudioPlayer::AudioPlayer(int frequency, Uint16 format, int channels, int chunksize)
{
    if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0)
        std::cout << "Couldn't initialize SDL_Mixer!!\nError: " << Mix_GetError() << std::endl;
}

void tnt::AudioPlayer::PlayMusic(std::string filename, int loops)
{
    if (AssetManager::This()->music[filename] == nullptr)
    {
        AssetManager::This()->music[filename] = Mix_LoadMUS(filename.c_str());
        if (AssetManager::This()->music[filename] == nullptr)
        {
            std::cout << "Couldn't load " << filename.c_str() << "\n.Error: " << Mix_GetError() << std::endl;
            return;
        }
    }

    Mix_PlayMusic(AssetManager::This()->music[filename], loops);
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

void tnt::AudioPlayer::PlaySFX(std::string filename, int channel, int loops)
{
    if (AssetManager::This()->sfx[filename] == nullptr)
    {
        AssetManager::This()->sfx[filename] = Mix_LoadWAV(filename.c_str());
        if (AssetManager::This()->sfx[filename] != nullptr)
        {
            std::cout << "Couldn't load " << filename.c_str() << "\nError: " << Mix_GetError() << std::endl;
            return;
        }
    }

    Mix_PlayChannel(channel, AssetManager::This()->sfx[filename], loops);
}