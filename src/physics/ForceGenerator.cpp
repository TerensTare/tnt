// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "physics/ForceGenerator.hpp"

void tnt::ForceRegistry::update(float duration)
{
    for (auto it{registrations.begin()}; it != registrations.end(); ++it)
        it->generator->update(it->particle, duration);
}

void tnt::ForceRegistry::add(tnt::Particle *particle, tnt::ForceGenerator *generator)
{
    ForceRegistration registration;
    registration.particle = particle;
    registration.generator = generator;
    registrations.push_back(registration);
}

tnt::GravityForce::GravityForce(Vector const &gravity)
    : value{gravity} {}

void tnt::GravityForce::update(tnt::Particle *particle, float duration)
{
    if (particle->getMass() < .0f)
        return;
    particle->addForce(value * particle->getMass());
}

tnt::DragForce::DragForce(float c1, float c2)
    : k1{c1}, k2{c2} {}

void tnt::DragForce::update(tnt::Particle *particle, float duration)
{
    Vector force{particle->getVelocity()};
    float dcoeff{force.Magnitude()};
    dcoeff = k1 * dcoeff + k2 * dcoeff * dcoeff;

    force.Normalize();
    force *= (-dcoeff);
    particle->addForce(force);
}

tnt::SpringForce::SpringForce(tnt::Particle *other, float c, float rest)
    : o{other}, k{c}, length{rest} {}

void tnt::SpringForce::update(Particle *particle, float duration)
{
    Vector force{particle->getPosition()};
    force -= o->getPosition();
    float mag{force.Magnitude()};
    mag = std::abs(mag - length);
    mag *= k;

    force.Normalize();
    force *= (-mag);
    particle->addForce(force);
}

tnt::AnchoredSpringForce::AnchoredSpringForce(tnt::Vector *anch, float c, float restl)
    : anchor{anch}, springc{c}, length{restl} {}

void tnt::AnchoredSpringForce::update(tnt::Particle *particle, float duration)
{
    Vector force{particle->getPosition()};
    force -= *anchor;

    float mag{force.Magnitude()};
    mag = std::abs(mag - length);
    mag *= springc;

    force.Normalize();
    force *= -mag;
    particle->addForce(force);
}

tnt::BungeeForce::BungeeForce(tnt::Particle *p, float c, float l)
    : other{p}, springc{c}, length{l} {}

void tnt::BungeeForce::update(tnt::Particle *particle, float duration)
{
    Vector force{particle->getPosition()};
    force -= other->getPosition();

    float mag{force.Magnitude()};
    if (mag <= length)
        return;
    mag = springc * (length - mag);

    force.Normalize();
    force *= -mag;
    particle->addForce(force);
}

tnt::BuoyancyForce::BuoyancyForce(float maxDepth, float volume, float waterHeight, float liquidDensity)
    : maxdepth{maxDepth}, vol{volume}, waterheight{waterHeight}, liquiddensity{liquidDensity} {}

void tnt::BuoyancyForce::update(tnt::Particle *particle, float duration)
{
    float depth{particle->getPosition().y};
    if (depth >= waterheight + maxdepth)
        return;
    Vector force{.0f, .0f};

    if (depth <= waterheight - maxdepth)
    {
        force.y = liquiddensity * vol;
        particle->addForce(force);
        return;
    }

    force.y = liquiddensity * vol * (depth - maxdepth - waterheight) / 2 * maxdepth;
    particle->addForce(force);
}

tnt::FakeSpring::FakeSpring(tnt::Vector *anch, float springC, float damp)
    : anchor{anch}, springc{springC}, damping{damp} {}

void tnt::FakeSpring::update(tnt::Particle *particle, float duration)
{
    if (!particle->hasFiniteMass())
        return;
    Vector pos{particle->getPosition()};
    pos -= *anchor;

    float gamma{(float).5 * std::sqrtf(4 * springc - damping * damping)};
    if (gamma == .0f)
        return;

    Vector c{pos * (damping / ((float)2 * gamma)) + particle->getVelocity() * ((float)1 / gamma)};
    Vector target{pos * std::cosf(gamma * duration) + c * std::sinf(gamma * duration)};
    target *= std::expf((float)(-.5) * duration * damping);
    Vector accel{(target - pos) * ((float)1.0 / duration * duration) - particle->getVelocity() * duration};
    particle->addForce(accel * particle->getMass());
}