#ifndef TNT_DOO_ECS_CAMERAS_SYSTEM_HPP
#define TNT_DOO_ECS_CAMERAS_SYSTEM_HPP

#include <span>
#include <nlohmann/json_fwd.hpp>

#include "core/Window.hpp"
#include "doo_ecs/Base.hpp"
#include "math/Vector.hpp"
#include "types/SparseSet.hpp"

// example of a json chunk containing the cameras.
// [
//     {
//         "angle": 10,
//         "pos": { "x" : 0, "y" : 0 },
//         "w": 100,
//         "h": 200,
//         "speed": 5
//     }
// ]

// TODO:
// Axis locking (move camera only on x-axis/y-axis/both).
// Keep a minimal offset from the object the camera is following.
// Define maximal distance that the camera can have from the origin, by knowing the world and camera size.
// Add ability to draw camera and objects shrinked. (ex. on split screen games)
// Add ability to zoom objects on the camera with lerp().
// Pre-made aspect ratios (like 16:9, 4:3)
// Scriptable cameras

// TODO(maybe):
// initialize the system with the size of the world ??
// move angle, scale and pos to objects_sys ??

namespace tnt::doo
{
    struct camera_comp final
    {
        /// @brief Create a new camera.
        /// @param angle_ The angle of the camera.
        /// @param pos_ The position of the camera.
        /// @param w_ The width of the camera.
        /// @param h_ The height of the camera.
        /// @param speed_ The speed the camera follows its target (if any). 5 by default.
        /// @param shaking_ The amplitude of shaking. .7f by default.
        /// @param decrease_ The amount of units the shaking factors decreases each second. 1.f by default.
        /// @param scale_ The scale of the objects drawed inside the camera. {1.f, 1.f} by default.
        constexpr camera_comp(float angle_, Vector const &pos_,
                              float w_, float h_, float speed_ = 5.f,
                              float shaking_ = .7f, float decrease_ = 1.f,
                              Vector const &scale_ = Vector{1.f, 1.f}) noexcept
            : angle{angle_}, w{w_}, h{h_}, speed{speed_}, pos{pos_},
              shaking{shaking_}, shakeLoss{decrease_}, scale{scale_} {}

        float angle;     /// < The angle of the camera.
        float w;         /// < The width of the camera.
        float h;         /// < The height of the camera.
        float speed;     /// < The speed of the camera.
        float shaking;   /// < The amplitude of shaking.
        float shakeLoss; /// < The amount of shaking decrease for second.
        Vector pos;      /// < The position of the camera.
        Vector scale;    /// < The scale of the objects on the camera.
    };

    inline struct cameras_sys final
    {
        // cameras_sys() = default;

        // cameras_sys(cameras_sys const &) = delete;
        // cameras_sys &operator=(cameras_sys const &) = delete;

        /// @brief Add a new camera to the system.
        /// @param data The data of the camera component.
        void add_object(camera_comp const &data);

        /// @brief Load camera data from a json chunk.
        /// @param j The json chunk.
        void from_json(nlohmann::json const &j);

        /// @brief Store camera data of a specific object to a json chunk.
        /// @param id The id of the camera to serialize to json.
        /// @param j The json chunk where the data will be saved.
        void to_json(camera const &id, nlohmann::json &j);

        /// @brief Draw widgets on the given window to modify the datas of the system.
        /// @param id The id of the desired camera.
        /// @param win The window where the widgets will be drawed.
        void draw_imgui(camera const &id, Window const &win) noexcept;

        /// @brief Center the given object to the center of the camera. Maintain
        /// @param cam The id of the camera.
        /// @param id The id of the object that should be centered.
        /// @param time_ The elapsed time since the last frame.
        /// @param off The offset that the object can have from the center of the camera. Defaulted to the center.
        void follow(camera const &cam, object const &id, float time_, Vector const &off = Vector{0.f, 0.f}) noexcept;

        /// @brief Shake the camera for the given amount of time.
        /// @param cam The id of the camera that should be shaked.
        /// @todo Return to the original posiition after shaking.
        /// @todo Shake for a fixed amount of time with shake_for().
        /// @note Based on https://gist.github.com/ftvs/5822103
        void shake(camera const &cam) noexcept;

        /// @brief Set the camera zoom to fit all the given objects.
        /// @param cam The id of the camera to zoom.
        /// @param objs The id-s of the objects that should fit on the camera.
        void zoom_to_fit(camera const &cam, std::span<object> objs);

        /// @brief Remove the given object from the camera system.
        /// @param id The id of the object you want to remove.
        void remove(camera const &id) noexcept;

        /// @brief Remove all the objects from the camera system.
        void clear() noexcept;

        sparse_set<camera> active; /// < The id-s of all the active cameras. Not related to the id-s of the objects.

        std::vector<float> angle;      /// < The angles of all the cameras.
        std::vector<float> width;      /// < The width of all the cameras.
        std::vector<float> height;     /// < The height of all the cameras.
        std::vector<float> speed;      /// < The speed the camera will approach to the player.
        std::vector<float> shaking;    /// < The amount of shaking for each camera.
        std::vector<float> shakeLoss;  /// < The amount of decreasing on the shaking of each camera per second.
        std::vector<float> shake_time; /// < The amount of time in milliseconds a camera should shake, if it is shaking.

        std::vector<object> target; /// < The id-s of the object each camera is centered to. -1 if none.
        std::vector<Vector> pos;    /// < The position of each camera.
        std::vector<Vector> scale;  /// < The scale of the objects drawed on each camera.
        std::vector<Vector> offset; /// < The offset that the object can have from the center of the camera.
    } cameras;                      /// < An instance of cameras_sys.
} // namespace tnt::doo

#endif //!TNT_DOO_ECS_CAMERAS_SYSTEM_HPP