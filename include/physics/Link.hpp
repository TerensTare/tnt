#ifndef LINK_HPP
#define LINK_HPP

#include "physics/Contact.hpp" // TODO(maybe): include this only on the source file.

namespace tnt
{
class BodiesLink
{
protected:
    float getLength() const noexcept;

public:
    virtual unsigned fillContact(Contact *contact, unsigned limit) const = 0;

    Particle *particles[2];
};

class CableLink : public BodiesLink
{
public:
    virtual unsigned fillContact(Contact *contact, unsigned limit) const override;

    float maxLength;
    float restitution;
};

class RodLink : public BodiesLink
{
public:
    float getLength() const noexcept;
    virtual unsigned fillContact(Contact *contact, unsigned limit) const override;

    float length;
};
} // namespace tnt

#endif //!LINK_HPP