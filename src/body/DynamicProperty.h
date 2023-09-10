/*==============================================================
                            OpenMPM3D
    C-plus-plus code for 3-Dimensional Material Point Method
================================================================
    Copyright (C) 2022 - 

    Computational Dynamics Group
    Department of Engineering Mechanics
    School of Aerospace Engineering
    Tsinghua Univeristy
    Beijing 100084, P. R. China

    Corresponding Author: Xiong Zhang
    E-mail: xzhang@tsinghua.edu.cn
================================================================
    Info: Class definition for dynamic properties
    Code-writter: Ruichen Ni
    Date: 2023.9.10
==============================================================*/
#ifndef _DynamicProperty_H_
#define _DynamicProperty_H_
#include "../main/MPM3D_MACRO.h"

class DynamicProperty
{
public:
    //!> Constructor and Deconstructor
    DynamicProperty();
    DynamicProperty(const DynamicProperty& dp);
    ~DynamicProperty();

    //!> Update the particle position through the filed velocity interpolated from grid node
    inline void UpdatePosition(Array3D& field_vel, MPM_FLOAT dt);

    //!> Update the particle velocity through the filed acceleration interpolated from grid node
    inline void UpdateVelocity(Array3D& field_acc, MPM_FLOAT dt);
private:
    Array3D _position;      //!< Space position
    Array3D _velocity;      //!< particle velocity
public:
    //!> Various Get/Set Function
    inline Array3D GetPosition() { return _position; }
    inline void SetPosition(Array3D& p) { _position = p;}

    inline Array3D GetVelocity() {return _velocity;}
    inline void SetVelocity(Array3D& v) {_velocity = v;}
};
#endif