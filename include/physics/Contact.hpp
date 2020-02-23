#ifndef CONTACT_HPP
#define CONTACT_HPP

#include "physics/Particle.hpp"

namespace tnt
{
// this works only on particles, momentally...
class Contact
{
public:
    float restitution; // coefficient
    float penetration;
    Vector contactNormal;
    Particle *particles[2];

protected:
    void resolve(float duration);
    float calcSeparatingVelocity() const;

private:
    void resolveVel(float duration);
    void resolveInterpenetration(float duration);

    friend class ContactResolver;
};

class ContactResolver
{
public:
    ContactResolver(unsigned it);

    void setIterations(unsigned it) noexcept;
    void resolve(Contact *contacts, unsigned number, float duration);

protected:
    unsigned iterations;
    unsigned iterationsUsed;
};

class ContactGenerator
{
public:
    virtual unsigned addContact(Contact *contact, unsigned limit) const = 0;
};
} // namespace tnt

#endif //!CONTACT_HPP