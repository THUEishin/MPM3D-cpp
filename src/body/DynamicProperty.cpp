#include "DynamicProperty.h"

DynamicProperty::DynamicProperty()
{
    _position = {0.0, 0.0, 0.0};
    _velocity = {0.0, 0.0, 0.0};
}

DynamicProperty::DynamicProperty(const DynamicProperty& dp)
{
    _position = dp._position;
    _velocity = dp._velocity;
}

DynamicProperty::~DynamicProperty()
{

}

inline void DynamicProperty::UpdatePosition(Array3D& field_vel, MPM_FLOAT dt)
{
    for (size_t i = 0; i < 3; i++)
    {
        _position[i] += field_vel[i]*dt;
    }
}

inline void DynamicProperty::UpdateVelocity(Array3D& field_acc, MPM_FLOAT dt)
{
    for (size_t i = 0; i < 3; i++)
    {
        _velocity[i] += field_acc[i]*dt;
    }
}