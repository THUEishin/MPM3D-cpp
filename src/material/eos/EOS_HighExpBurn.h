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
    Info: Class definition for Explosive Burning EOS model
    Code-writter: Ruichen Ni
    Date: 2022.10.11
==============================================================*/

#ifndef _EOS_HIGHEXPBURN_H_
#define _EOS_HIGHEXPBURN_H_

#include "EOS_JWL.h"

class EOS_HighExpBurn: public EOS_JWL
{
public:
    EOS_HighExpBurn();
    ~EOS_HighExpBurn();

    //!> Initial the EOS model with parameters' map
    virtual bool Initialize(map<string, MPM_FLOAT> &eos_para, MPM_FLOAT rho0, ofstream& os);

    //!> Write EOS model information into file
    virtual void Write(ofstream &os);

    //!> Update the pressure of the particle
    virtual void UpdatePressure(PhysicalProperty* pp, MPM_FLOAT delta_vol_half, 
        MPM_FLOAT delta_ie, map<string, MPM_FLOAT>& transfer);

    //!> Calculate the squared adabatic sound speed of EOS part
    virtual MPM_FLOAT SoundSpeedSquare_EOS(PhysicalProperty* pp);

    //!> Add extra particle properties based on different failure model
    virtual bool AddExtraParticleProperty_EOS(vector<MPM::ExtraParticleProperty> &ExtraProp,
        map<string, MPM_FLOAT>& transfer);
private:
    MPM_FLOAT _detonation_velocity;
    MPM_FLOAT _F1_coefficient;
    MPM_FLOAT _F2_coefficient;
    MPM_FLOAT _pressure_CJ;
    MPM_FLOAT _character_length;

    bool _beta_burning;         //!< beta burn option
    bool _programed_burning;    //!< programed burning option
private:
    MPM_FLOAT CalculateBurningFraction(PhysicalProperty* pp);
};

#endif