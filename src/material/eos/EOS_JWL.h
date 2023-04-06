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
    Info: Class definition for Jones-Wilkins-Lee EOS model
    Code-writter: Ruichen Ni
    Date: 2022.10.5
==============================================================*/

#ifndef _EOS_JWL_H
#define _EOS_JWL_H

#include "EOS_Base.h"

class EOS_JWL: public EOS_Base
{
public:
    EOS_JWL();
    ~EOS_JWL();

    //!> Initial the EOS model with parameters' map
    virtual bool Initialize(map<string, MPM_FLOAT> &eos_para, MPM_FLOAT rho0, ofstream& os);

    //!> Write EOS model information into file
    virtual void Write(ofstream &os);

    //!> Update the pressure of the particle
    virtual void UpdatePressure(PhysicalProperty* pp, MPM_FLOAT delta_vol_half, 
        MPM_FLOAT delta_ie, map<string, MPM_FLOAT>& transfer);

    //!> Calculate the squared adabatic sound speed of EOS part
    virtual MPM_FLOAT SoundSpeedSquare_EOS(PhysicalProperty* pp);
protected:
    //!> parameters for Jones-Wilkins-Lee EOS
    //!> p = A*(1 - w/R1/V)*exp(-R1*V) + B*(1 - w/R2/V)*exp(-R2*V) + wE/V
    MPM_FLOAT _A, _B, _R1, _R2, _w;
};

#endif