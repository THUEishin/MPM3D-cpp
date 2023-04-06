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
    Info: Class definition for Polynomial EOS model
    Code-writter: Ruichen Ni
    Date: 2022.10.5
==============================================================*/

#ifndef _EOS_POLYNOMIAL_H_
#define _EOS_POLYNOMIAL_H_

#include "EOS_Base.h"

class EOS_Polynomial: public EOS_Base
{
public:
    EOS_Polynomial();
    ~EOS_Polynomial();

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
    //!> parameters for polynomial EOS
    //!> p = c0 + c1*mu + c2*mu^2 + c3*mu^3 + (c4 + c5*mu + c6*mu^2)*E
    MPM_FLOAT _c0, _c1, _c2, _c3, _c4, _c5, _c6;
};

#endif