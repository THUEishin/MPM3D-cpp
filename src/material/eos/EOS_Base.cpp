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
    Info: Implementation of class 'EOS_Base'
    Code-writter: Ruichen Ni
    Date: 2022.10.4
==============================================================*/

#include "EOS_Base.h"
EOS_Base::EOS_Base()
{
    Type = "";
    _density_0 = 0.0;
    _sound_speed_0 = 0.0;
    _internal_energy_0 = 0.0;

    ParameterMap_EOS["C0"] = &_sound_speed_0;
    ParameterMap_EOS["E0"] = &_internal_energy_0;
}

EOS_Base::~EOS_Base()
{
}

bool EOS_Base::Initialize(map<string, MPM_FLOAT> &eos_para, MPM_FLOAT rho0, ofstream& os)
{
    for(map<string, MPM_FLOAT>::iterator iter = eos_para.begin(); 
        iter != eos_para.end(); iter++)
    {
        if(ParameterMap_EOS.find(iter->first) != ParameterMap_EOS.end())
            *ParameterMap_EOS[iter->first] = iter->second;
        else
        {
            string error_msg = "Can't find the EOS model parameter " + iter->first + " at " + Type;
            MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
            MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
            return false;
        }
    }

    _density_0 = rho0;
    return true;
}

bool EOS_Base::AddExtraParticleProperty_EOS(vector<MPM::ExtraParticleProperty> &ExtraProp,
        map<string, MPM_FLOAT>& transfer)
{
    // Nothing needs to be added
    return true;
}