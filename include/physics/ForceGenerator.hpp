#ifndef FORCE_GENERATOR_HPP
#define FORCE_GENERATOR_HPP

#include <vector>
#include "physics/Particle.hpp"

// TODO: take the same approach with easing functions.
namespace tnt
{
// TODO: this works only on Particle-s.
class ForceGenerator
{
public:
    virtual ~ForceGenerator() noexcept {}

    virtual void update(Particle *particle, real duration) = 0;
};

class ForceRegistry
{
public:
    void add(Particle *p, ForceGenerator *gen);
    void remove(Particle *p, ForceGenerator *gen);
    void clear();

    void update(real duration);

protected:
    struct ForceRegistration
    {
        Particle *particle;
        ForceGenerator *generator;
    };

    using Registry = std::vector<ForceRegistration>;
    Registry registrations;
};

class GravityForce : public ForceGenerator
{
public:
    GravityForce(Vector const &gravity);

    virtual void update(Particle *particle, real duration) override;

private:
    Vector value;
};

class DragForce : public ForceGenerator
{
public:
    DragForce(real c1, real c2);

    virtual void update(Particle *particle, real duration) override;

private:
    real k1, k2;
};

class SpringForce : public ForceGenerator
{
public:
    SpringForce(Particle *other, real c, real rest);

    virtual void update(Particle *particle, real duration) override;

private:
    Particle *o;
    real k;
    real length;
};

class AnchoredSpringForce : public ForceGenerator
{
public:
    AnchoredSpringForce(Vector *anch, real c, real restl);

    virtual void update(Particle *particle, real duration) override;

private:
    Vector *anchor;
    real springc;
    real length;
};

class BungeeForce : public ForceGenerator
{
public:
    BungeeForce(Particle *p, real c, real l);

    virtual void update(Particle *particle, real duration) override;

private:
    Particle *other;
    real springc;
    real length;
};

class BuoyancyForce : public ForceGenerator
{
public:
    BuoyancyForce(real maxDepth, real volume, real waterHeight,
                  real liquidDensity = (real)1000.0);

    virtual void update(Particle *particle, real duration) override;

private:
    real maxdepth;
    real vol;
    real waterheight;
    real liquiddensity;
};

class FakeSpring : public ForceGenerator
{
public:
    FakeSpring(Vector *anch, real springC, real damp);

    virtual void update(Particle *particle, real duration) override;

private:
    Vector *anchor;
    real springc;
    real damping;
};

} // namespace tnt

#endif //!FORCE_GENERATOR_HPP