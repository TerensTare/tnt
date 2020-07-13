#ifndef TNT_PCG_PARTICLE_HPP
#define TNT_PCG_PARTICLE_HPP

#include "ecs/RigidBody.hpp"
#include "ecs/Sprite.hpp"

// TODO: better name for ParticleRules and members.
// TODO: destroy when outside the window.

namespace tnt
{
    class Particle
        : virtual public Sprite,
          virtual public RigidBody
    {
    public:
        Particle(Window const *win, uint8_t r, uint8_t g, uint8_t b, uint8_t a,
                 Vector const &speed, float const &radius_,
                 long long time = -1);
        ~Particle() noexcept;

        virtual void Update(long long elapsed) noexcept;
        void Draw(Window const *win) noexcept;

    private:
        bool alive;
        float radius;
        long long lifetime; // in ms
        Vector bounds;
    };
} // namespace tnt

#endif //! TNT_PCG_PARTICLE_HPP