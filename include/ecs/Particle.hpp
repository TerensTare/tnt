#ifndef TNT_PCG_PARTICLE_HPP
#define TNT_PCG_PARTICLE_HPP

#include "ecs/RigidBody.hpp"
#include "ecs/Sprite.hpp"

// TODO: better name for ParticleRules and members.

namespace tnt
{
    class Particle
        : virtual public Sprite,
          virtual public RigidBody
    {
    public:
        Particle(Window const *win, std::string_view filename,
                 Vector const &speed, float &radius_, Rectangle const &area,
                 long long time = -1);
        ~Particle() noexcept;

        virtual void Update(long long elapsed) noexcept;

    private:
        bool alive;
        float radius;
        long long lifetime; // in ms
        Rectangle rect;
    };
} // namespace tnt

#endif //! TNT_PCG_PARTICLE_HPP