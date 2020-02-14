#ifndef CONTACT_HPP
#define CONTACT_HPP

#include "physics/Particle.hpp"

namespace tnt
{
// this works only on particles, momentally...
class Contact
{
protected:
    void resolve(real duration);
    real calcSeparatingVelocity() const;

    Particle *particles[2];
    Vector contactNormal;
    real restitution; // coefficient

private:
    void resolveVel(real duration);
};
} // namespace tnt

#endif //!CONTACT_HPP