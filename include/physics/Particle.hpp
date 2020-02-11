#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vector.hpp"

namespace tnt
{
class Particle
{
public:
    void integrate(real duration);

    void setDamping(real const &value) noexcept;
    real getDamping() const noexcept;

    void setMass(real const &mass) noexcept;
    real getMass() const noexcept;

    void setPosition(Vector const &position) noexcept;
    Vector getPosition() const noexcept;

    void setVelocity(Vector const &vel) noexcept;
    Vector getVelocity() const noexcept;

    void setAcceleration(Vector const &accel) noexcept;
    Vector getAcceleration() const noexcept;

    void addForce(Vector const &force) noexcept;
    void clearAccumulator() noexcept;

protected:
    real damping;
    real inverseMass; // this way mass can't be 0

    Vector pos;
    Vector velocity;
    Vector acceleration;

    Vector forceAccumulator;
};
} // namespace tnt

#endif //!PARTICLE_HPP