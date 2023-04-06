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
    Info: Base class definition for Equation-Of-State(EOS)
    Code-writter: Ruichen Ni
    Date: 2022.10.4
==============================================================*/

#ifndef _EOS_BASE_H_
#define _EOS_BASE_H_

#include "../../main/MPM3D_MACRO.h"
#include "../../body/PhysicalProperty.h"
class EOS_Base
{
public:
    EOS_Base();
    ~EOS_Base();

    inline string GetName() {return Type;}

    //!> Initial the EOS model with parameters' map
    virtual bool Initialize(map<string, MPM_FLOAT> &eos_para, MPM_FLOAT rho0, ofstream& os);

    //!> Write EOS model information into file
    virtual void Write(ofstream &os) = 0;

    //!> Update the pressure of the particle
    virtual void UpdatePressure(PhysicalProperty* pp, MPM_FLOAT delta_vol_half, 
        MPM_FLOAT delta_ie, map<string, MPM_FLOAT>& transfer) = 0;

    //!> Calculate the squared adabatic sound speed of EOS part
    virtual MPM_FLOAT SoundSpeedSquare_EOS(PhysicalProperty* pp) = 0;

    //!> Add extra particle properties based on different failure model
    virtual bool AddExtraParticleProperty_EOS(vector<MPM::ExtraParticleProperty> &ExtraProp,
        map<string, MPM_FLOAT>& transfer);
protected:
    string Type;
    MPM_FLOAT _density_0;           //!< initial density
    MPM_FLOAT _sound_speed_0;       //!< initial sound speed
    MPM_FLOAT _internal_energy_0;   //!< initial internal energy per reference specific volume

    //!> EOS parameters for initialization
    map<string, MPM_FLOAT*> ParameterMap_EOS;
};

#endif