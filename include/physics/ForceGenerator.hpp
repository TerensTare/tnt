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

    virtual void update(Particle *particle, float duration) = 0;
};

class ForceRegistry
{
public:
    void add(Particle *p, ForceGenerator *gen);
    void remove(Particle *p, ForceGenerator *gen);
    void clear();

    void update(float duration);

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

    virtual void update(Particle *particle, float duration) override;

private:
    Vector value;
};

class DragForce : public ForceGenerator
{
public:
    DragForce(float c1, float c2);

    virtual void update(Particle *particle, float duration) override;

private:
    float k1, k2;
};

class SpringForce : public ForceGenerator
{
public:
    SpringForce(Particle *other, float c, float rest);

    virtual void update(Particle *particle, float duration) override;

private:
    Particle *o;
    float k;
    float length;
};

class AnchoredSpringForce : public ForceGenerator
{
public:
    AnchoredSpringForce(Vector *anch, float c, float restl);

    virtual void update(Particle *particle, float duration) override;

private:
    Vector *anchor;
    float springc;
    float length;
};

class BungeeForce : public ForceGenerator
{
public:
    BungeeForce(Particle *p, float c, float l);

    virtual void update(Particle *particle, float duration) override;

private:
    Particle *other;
    float springc;
    float length;
};

class BuoyancyForce : public ForceGenerator
{
public:
    BuoyancyForce(float maxDepth, float volume, float waterHeight,
                  float liquidDensity = (float)1000.0);

    virtual void update(Particle *particle, float duration) override;

private:
    float maxdepth;
    float vol;
    float waterheight;
    float liquiddensity;
};

class FakeSpring : public ForceGenerator
{
public:
    FakeSpring(Vector *anch, float springC, float damp);

    virtual void update(Particle *particle, float duration) override;

private:
    Vector *anchor;
    float springc;
    float damping;
};

} // namespace tnt

#endif //!FORCE_GENERATOR_HPP