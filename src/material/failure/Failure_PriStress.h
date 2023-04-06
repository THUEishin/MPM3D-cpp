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
        Principle Stress'
    Code-writter: Ruichen Ni
    Date: 2022.10.4
==============================================================*/

#ifndef _FAILURE_PRISTRESS_H_
#define _FAILURE_PRISTRESS_H_

#include "Failure_Base.h"

class Failure_PriStress:public Failure_Base
{
public:
    Failure_PriStress();
    ~Failure_PriStress();

    //!> Identify if the particle fails according to its physical property
    virtual bool CheckFailure(PhysicalProperty* pp, map<string, MPM_FLOAT>& transfer);

    //!> Write failure model information into file
    virtual void Write(ofstream &os);

    //!> Initial the Failure model with parameters' map
    virtual bool Initialize(map<string, MPM_FLOAT> &failure_para, ofstream& os);
private:
    //!> Threshold of principle stress
    MPM_FLOAT _min_principle_stress;
    MPM_FLOAT _max_principle_stress;
    MPM_FLOAT _max_shear_stress;
};

#endif