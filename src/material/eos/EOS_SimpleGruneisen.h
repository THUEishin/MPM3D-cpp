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
    Info: Class definition for Simplified Gruneisen EOS model
    Code-writter: Ruichen Ni
    Date: 2022.10.5
==============================================================*/

#ifndef _EOS_SIMPLEGRUNEISEN_H
#define _EOS_SIMPLEGRUNEISEN_H

#include "EOS_Base.h"

class EOS_SimpleGruneisen: public EOS_Base
{
public:
    EOS_SimpleGruneisen();
    ~EOS_SimpleGruneisen();

    //!> Initial the EOS model with parameters' map
    virtual bool Initialize(map<string, MPM_FLOAT> &eos_para, MPM_FLOAT rho0, ofstream& os);

    //!> Write EOS model information into file
    virtual void Write(ofstream &os);

    //!> Update the pressure of the particle
    virtual void UpdatePressure(PhysicalProperty* pp, MPM_FLOAT delta_vol_half, 
        MPM_FLOAT delta_ie, map<string, MPM_FLOAT>& transfer);

    //!> Calculate the squared adabatic sound speed of EOS part
    virtual MPM_FLOAT SoundSpeedSquare_EOS(PhysicalProperty* pp);
private:
    //!> parameters for Gruneisen EOS
    //!> p = rho0*c0^2*(mu + (2*s - 1)*mu^2 + (s - 1)*(3*s - 1)*mu^3)
    MPM_FLOAT _s;       //!< Slope in shock velocity and particle velocity: u_s = c_0 + s*u_p
    MPM_FLOAT _gamma0;  //!< Initial Gruneisen gamma: _gamma0 ~ 2*_s - 1

    MPM_FLOAT _c1, _c2, _c3;
};

#endif