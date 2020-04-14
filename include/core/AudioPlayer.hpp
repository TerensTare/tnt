#ifndef AUDIO_PLAYER_HPP
#define AUDIO_PLAYER_HPP

#include <string_view>

// Consider making this a singleton.
namespace tnt
{
    class AudioPlayer
    {
      public:
        AudioPlayer(int frequency, unsigned short format, int channels,
                    int chunksize);

        void PlayMusic(std::string_view filename, int loops);
        void PauseMusic();
        void ResumeMusic();

        void PlaySFX(std::string_view filename, int channel, int loops);
    };
} // namespace tnt

#endif //! AUDIO_PLAYER_HPP