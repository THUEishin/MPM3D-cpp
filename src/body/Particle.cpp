#include "Particle.h"

Particle::Particle()
{
    _dynamic_property = new DynamicProperty();
    _physical_property = new PhysicalProperty();
}

Particle::Particle(const Particle& p)
{
    _dynamic_property = new DynamicProperty(*p._dynamic_property);
    _physical_property = new PhysicalProperty(*p._physical_property);
}

Particle::~Particle()
{
    if (_dynamic_property)
        delete _dynamic_property;
    if (_physical_property)
        delete _physical_property;
}