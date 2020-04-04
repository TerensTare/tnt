#ifndef TNT_PCG_PARTICLE_HPP
#define TNT_PCG_PARTICLE_HPP

#include "math/Rectangle.hpp"
#include "ecs/Sprite.hpp"
#include "ecs/RigidBody.hpp"

// TODO: better name for ParticleRules and members.

// TODO(maybe):
// derive Particle from Object.

namespace tnt
{

class Particle
    : virtual public Sprite,
      virtual public RigidBody
{
public:
    Particle(
        Window const *win, std::string_view filename,
        Vector const &speed, float &radius_,
        Rectangle const &area,
        long long time = 0);
    ~Particle() noexcept;

    virtual void Update(long long elapsed) noexcept;

private:
    bool alive;
    float radius;
    long long lifetime; // in ms
    Rectangle rect;
};
} // namespace tnt

#endif //!TNT_PCG_PARTICLE_HPP