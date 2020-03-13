#ifndef AUDIO_PLAYER_HPP
#define AUDIO_PLAYER_HPP

#include "AssetManager.hpp"

// Consider making this a singleton.
namespace tnt
{
class AudioPlayer
{
public:
    AudioPlayer(int frequency, Uint16 format, int channels, int chunksize);

    void PlayMusic(std::string filename, int loops);
    void PauseMusic();
    void ResumeMusic();

    void PlaySFX(std::string filename, int channel, int loops);
};
} // namespace tnt

#endif //!AUDIO_PLAYER_HPP