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
    Info: Class definition for material particles
    Code-writter: Ruichen Ni
    Date: 2023.9.10
==============================================================*/
#ifndef _Particle_H_
#define _Particle_H_
#include "../main/MPM3D_MACRO.h"
#include "DynamicProperty.h"
#include "PhysicalProperty.h"

class Particle
{
public:
    //!> Constructor and Deconstructor
    Particle();
    Particle(const Particle& p);
    ~Particle();
private:
    DynamicProperty* _dynamic_property;
    PhysicalProperty* _physical_property;
public:
//!> Various Get/Set Functions
    inline DynamicProperty* GetDynamicPropertyPointer() {return _dynamic_property;}

    inline PhysicalProperty* GetPhysicalPropertyPointer() {return _physical_property;}
};
#endif