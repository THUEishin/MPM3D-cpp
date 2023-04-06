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
    Info: Base class definition for failure behavior of 
        Accumulated Damage Type
    Code-writter: Ruichen Ni
    Date: 2022.9.28
==============================================================*/

#ifndef _FAILURE_DAMAGE_JHONSONCOOK_H
#define _FAILURE_DAMAGE_JHONSONCOOK_H

#include "Failure_Base.h"

class Failure_Damage_JohnsonCook: public Failure_Base
{
public:
    Failure_Damage_JohnsonCook();
    ~Failure_Damage_JohnsonCook();

    //!> Identify if the particle fails according to its physical property
    virtual bool CheckFailure(PhysicalProperty* pp, map<string, MPM_FLOAT>& transfer);

    //!> Write failure model information into file
    virtual void Write(ofstream &os);

    //!> Initial the Failure model with parameters' map
    virtual bool Initialize(map<string, MPM_FLOAT> &failure_para, ofstream& os);

    //!> Add extra particle properties based on different failure model
    virtual bool AddExtraParticleProperty_Failure(vector<MPM::ExtraParticleProperty> &ExtraProp,
        map<string, MPM_FLOAT>& transfer);
private:
    //!> Parameters for damage update
    MPM_FLOAT _D1, _D2, _D3, _D4, _D5;
};

#endif