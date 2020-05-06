#ifndef AUDIO_PLAYER_HPP
#define AUDIO_PLAYER_HPP

#include <string_view>

// TODO: Consider making this a singleton or a namespace.
// TODO: Consider making a shorter constructor.

namespace tnt
{
/// @brief The class used to handle audio playing.
class AudioPlayer
{
public:
  /// @brief Creates a new @em AudioPlayer for a certain audio format.
  /// @param frequency The frequency the audio should be played.
  /// @param format The format of the audio.
  /// @param channels The number of the channels.
  /// @param chunksize The size of the chunk.
  AudioPlayer(int frequency, unsigned short format, int channels,
              int chunksize);

  /// @brief Plays the music of @em filename @em loops many times.
  /// If loops is -1, it plays the music ~65000 times.
  /// @param filename The name of the file to play.
  /// @param loops The number of times the music should be played.
  void PlayMusic(std::string_view filename, int loops);

  /// @brief Pauses the player if a music being played.
  void PauseMusic();

  /// @brief Resumes the player if it was paused.
  void ResumeMusic();

  /// @brief Play a .sfx file on the given @em channel @em loops many times.
  /// If loops is -1, it plays the music ~65000 times.
  /// @param filename The name of the sfx file.
  /// @param channel The id of the channel where the music should be played.
  /// @param loops The number of times the sfx should be played.
  void PlaySFX(std::string_view filename, int channel, int loops);
};
} // namespace tnt

#endif //! AUDIO_PLAYER_HPP