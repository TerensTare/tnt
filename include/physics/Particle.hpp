#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "physics/Vector.hpp"

namespace tnt
{
class Particle
{
public:
    void integrate(float duration);

    void setDamping(float const &value) noexcept;
    float getDamping() const noexcept;

    void setMass(float const &mass) noexcept;
    float getMass() const noexcept;

    void setPosition(Vector const &position) noexcept;
    Vector getPosition() const noexcept;

    void setVelocity(Vector const &vel) noexcept;
    Vector getVelocity() const noexcept;

    void setAcceleration(Vector const &accel) noexcept;
    Vector getAcceleration() const noexcept;

    bool hasFiniteMass() const noexcept;

    void addForce(Vector const &force) noexcept;
    void clearAccumulator() noexcept;

protected:
    float damping;
    float inverseMass; // this way mass can't be 0

    Vector pos;
    Vector velocity;
    Vector acceleration;

    Vector forceAccumulator;
};
} // namespace tnt

#endif //!PARTICLE_HPP