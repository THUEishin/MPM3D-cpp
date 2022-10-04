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
        material
    Code-writter: Ruichen Ni
    Date: 2022.9.28
==============================================================*/

#ifndef _FAILURE_BASE_H_
#define _FAILURE_BASE_H_

#include "../../main/MPM3D_MACRO.h"
#include "../../body/PhysicalProperty.h"

void CubicFunctionSolution(MPM_FLOAT a, MPM_FLOAT b, MPM_FLOAT c, MPM_FLOAT d, MPM_FLOAT (&roots)[3]);

class Failure_Base
{
public:
    Failure_Base();
    ~Failure_Base();

    inline string GetName() {return Type;};

    inline void SetErosion(bool erosion) {Erosion = erosion;};

    //!> Identify if the particle fails according to its physical property
    virtual bool CheckFailure(PhysicalProperty* pp) = 0;

    //!> Write failure model information into file
    virtual void Write(ofstream &os) = 0;

    //!> Initial the Failure model with parameters' map
    virtual bool Initialize(map<string, MPM_FLOAT> &failure_para);

    //!> Add extra particle properties based on different failure model
    virtual bool AddExtraParticleProperty_Failure(vector<MPM::ExtraParticleProperty> &ExtraProp) = 0;
protected:
    string Type;                            //!< Failure Behavior Type
    bool Erosion;                           //!< whether to erode particles when failed
    
    //!> Failure model parameters for initialization
    map<string, MPM_FLOAT*> ParameterMap_Failure;   
};

#endif