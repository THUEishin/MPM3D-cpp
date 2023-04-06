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
    Info: Class definition for "Null" strength behavior of 
        material(used for fluid material of MPM)
    Code-writter: Ruichen Ni
    Date: 2022.10.11
==============================================================*/

#ifndef _STRENGTH_NULL_H_
#define _STRENGTH_NULL_H_

#include "Strength_Base.h"

class Strength_Null: public Strength_Base
{
public:
    Strength_Null();
    ~Strength_Null();

    //!> Initial the strength model with parameters' map
    virtual bool Initialize(map<string, MPM_FLOAT> &strength_para, MPM_FLOAT rho0, ofstream& os);

    //!> Write strength model information into file
    virtual void Write(ofstream &os);

    //!> Update the deviatoric stress of the particle
    //!> the map named transfer is used to tansfer data between Strength/EOS/Failure model
    virtual void UpdateDeviatoricStress(PhysicalProperty* pp, SymTensor& delta_strain, 
        SymTensor& delta_vortex, map<string, MPM_FLOAT>& transfer);

    //!> Update the pressure of the particle with elastic assumption
    virtual void ElasticPressure(PhysicalProperty* pp, MPM_FLOAT delta_vol,
        map<string, MPM_FLOAT>& transfer);

    //!> Calculate the squared adabatic sound speed of deviatoric part
    virtual MPM_FLOAT SoundSpeedSquare_Strength(PhysicalProperty* pp);

    //!> Calculate the squared adabatic sound speed of elastic part
    virtual MPM_FLOAT SoundSpeedSquare_Elastic(PhysicalProperty* pp);

    //!> Update the temperature of the particle
    virtual void UpdateTemperature(PhysicalProperty* pp, MPM_FLOAT delta_vol,
        map<string, MPM_FLOAT>& transfer);

    //!> Modify the mean stress with temperature coefficient
    virtual void ModifyPressureByTemperature(PhysicalProperty* pp);
private:
    MPM_FLOAT _mu;  //!< coefficient of deviatoric viscosity
    MPM_FLOAT _ck;  //!< consistency for non-Newtom fluid
    MPM_FLOAT _nn;  //!< exponent for non-Newton fluid
};

#endif