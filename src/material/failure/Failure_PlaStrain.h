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
    Info: Class definition for failure behavior of 'Effective
        Plastic Strain'
    Code-writter: Ruichen Ni
    Date: 2022.9.29
==============================================================*/

#ifndef _FAILURE_PLASTRAIN_H_
#define _FAILURE_PLASTRAIN_H_

#include "Failure_Base.h"
class Failure_PlaStrain: public Failure_Base
{
public:
    Failure_PlaStrain();
    ~Failure_PlaStrain();

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
    //!> Threshold of effective plastic strain
    MPM_FLOAT _epmax;
};

#endif