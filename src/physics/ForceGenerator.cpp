// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "physics/ForceGenerator.hpp"

void tnt::ForceRegistry::update(real duration)
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

void tnt::GravityForce::update(tnt::Particle *particle, real duration)
{
    if (particle->getMass() < real0)
        return;
    particle->addForce(value * particle->getMass());
}

tnt::DragForce::DragForce(real c1, real c2)
    : k1{c1}, k2{c2} {}

void tnt::DragForce::update(tnt::Particle *particle, real duration)
{
    Vector force{particle->getVelocity()};
    real dcoeff{force.Magnitude()};
    dcoeff = k1 * dcoeff + k2 * dcoeff * dcoeff;

    force.Normalize();
    force *= (-dcoeff);
    particle->addForce(force);
}

tnt::SpringForce::SpringForce(tnt::Particle *other, real c, real rest)
    : o{other}, k{c}, length{rest} {}

void tnt::SpringForce::update(Particle *particle, real duration)
{
    Vector force{particle->getPosition()};
    force -= o->getPosition();
    real mag{force.Magnitude()};
    mag = std::abs(mag - length);
    mag *= k;

    force.Normalize();
    force *= (-mag);
    particle->addForce(force);
}

tnt::AnchoredSpringForce::AnchoredSpringForce(tnt::Vector *anch, real c, real restl)
    : anchor{anch}, springc{c}, length{restl} {}

void tnt::AnchoredSpringForce::update(tnt::Particle *particle, real duration)
{
    Vector force{particle->getPosition()};
    force -= *anchor;

    real mag{force.Magnitude()};
    mag = std::abs(mag - length);
    mag *= springc;

    force.Normalize();
    force *= -mag;
    particle->addForce(force);
}

tnt::BungeeForce::BungeeForce(tnt::Particle *p, real c, real l)
    : other{p}, springc{c}, length{l} {}

void tnt::BungeeForce::update(tnt::Particle *particle, real duration)
{
    Vector force{particle->getPosition()};
    force -= other->getPosition();

    real mag{force.Magnitude()};
    if (mag <= length)
        return;
    mag = springc * (length - mag);

    force.Normalize();
    force *= -mag;
    particle->addForce(force);
}

tnt::BuoyancyForce::BuoyancyForce(real maxDepth, real volume, real waterHeight, real liquidDensity)
    : maxdepth{maxDepth}, vol{volume}, waterheight{waterHeight}, liquiddensity{liquidDensity} {}

void tnt::BuoyancyForce::update(tnt::Particle *particle, real duration)
{
    real depth{particle->getPosition().y};
    if (depth >= waterheight + maxdepth)
        return;
    Vector force{real0, real0};

    if (depth <= waterheight - maxdepth)
    {
        force.y = liquiddensity * vol;
        particle->addForce(force);
        return;
    }

    force.y = liquiddensity * vol * (depth - maxdepth - waterheight) / 2 * maxdepth;
    particle->addForce(force);
}