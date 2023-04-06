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
    Info: Base class definition for strength behavior of 
        material
    Code-writter: Ruichen Ni
    Date: 2022.9.28
==============================================================*/

#ifndef _STRENGTH_BASE_H
#define _STRENGTH_BASE_H

#include "../../main/MPM3D_MACRO.h"
#include "../../body/PhysicalProperty.h"

class Strength_Base
{
public:
    Strength_Base();
    ~Strength_Base();

    inline string GetName() {return Type;}

    //!> Initial the strength model with parameters' map
    virtual bool Initialize(map<string, MPM_FLOAT> &strength_para, MPM_FLOAT rho0, ofstream& os);

    //!> Write strength model information into file
    virtual void Write(ofstream &os) = 0;

    //!> Update the deviatoric stress of the particle
    //!> the map named transfer is used to tansfer data between Strength/EOS/Failure model
    virtual void UpdateDeviatoricStress(PhysicalProperty* pp, SymTensor& delta_strain, 
        SymTensor& delta_vortex, map<string, MPM_FLOAT>& transfer) = 0;

    //!> Update the pressure of the particle with elastic assumption
    virtual void ElasticPressure(PhysicalProperty* pp, MPM_FLOAT delta_vol,
        map<string, MPM_FLOAT>& transfer) = 0;

    //!> Calculate the squared adabatic sound speed of deviatoric part
    virtual MPM_FLOAT SoundSpeedSquare_Strength(PhysicalProperty* pp) = 0;

    //!> Calculate the squared adabatic sound speed of elastic part
    virtual MPM_FLOAT SoundSpeedSquare_Elastic(PhysicalProperty* pp) = 0;

    //!> Update the temperature of the particle
    virtual void UpdateTemperature(PhysicalProperty* pp, MPM_FLOAT delta_vol,
        map<string, MPM_FLOAT>& transfer) = 0;

    //!> Modify the mean stress with temperature coefficient
    virtual void ModifyPressureByTemperature(PhysicalProperty* pp) = 0;

    //!> Add extra particle properties based on different strength model
    virtual bool AddExtraParticleProperty_Strength(vector<MPM::ExtraParticleProperty> &ExtraProp,
        map<string, MPM_FLOAT>& transfer);
protected:
    string Type;
    MPM_FLOAT _density_0;       //!< initial density

    bool _compute_temperature;  //!< Whether to update temperature or not

    //!> Parameter list for initialization
    map<string, MPM_FLOAT*> ParameterMap_Strength;
};

#endif