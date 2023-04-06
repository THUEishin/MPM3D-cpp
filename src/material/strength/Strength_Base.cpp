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
    Info: Implementation of class 'Strength_Base'
    Code-writter: Ruichen Ni
    Date: 2022.9.28
==============================================================*/

#include "Strength_Base.h"

Strength_Base::Strength_Base()
{
    Type = "";
    _density_0 = 0.0;
    _compute_temperature = false;
}

Strength_Base::~Strength_Base()
{
}

bool Strength_Base::Initialize(map<string, MPM_FLOAT> &strength_para, MPM_FLOAT rho0, ofstream& os)
{
    for(map<string, MPM_FLOAT>::iterator iter = strength_para.begin(); 
        iter != strength_para.end(); iter++)
    {
        if(ParameterMap_Strength.find(iter->first) != ParameterMap_Strength.end())
            *ParameterMap_Strength[iter->first] = iter->second;
        else if(iter->first == "ComputeTemperature")
        {
            if (iter->second > MPM_EPSILON)
                _compute_temperature = true;
        }
        else
        {
            string error_msg = "Can't find the strength model parameter " + iter->first + " at " + Type;
            MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
            MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
            return false;
        }
    }

    _density_0 = rho0;
    return true;
}

bool Strength_Base::AddExtraParticleProperty_Strength(vector<MPM::ExtraParticleProperty> &ExtraProp,
        map<string, MPM_FLOAT>& transfer)
{
    // Nothing to be added
    return true;
}