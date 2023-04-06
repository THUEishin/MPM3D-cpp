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
    Info: Class definition for failure behavior of 'Maximum 
        Principle Strain'
    Code-writter: Ruichen Ni
    Date: 2022.9.29
==============================================================*/

#ifndef _FAILURE_PRISTRAIN_H_
#define _FAILURE_PRISTRAIN_H_

#include "Failure_Base.h"

class Failure_PriStrain: public Failure_Base
{
public:
    Failure_PriStrain();
    ~Failure_PriStrain();

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
    //!> Threshold of principle strain
    MPM_FLOAT _min_principle_strain;
    MPM_FLOAT _max_principle_strain;
    MPM_FLOAT _max_shear_strain;
};

#endif