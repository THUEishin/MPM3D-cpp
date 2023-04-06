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
    Info: Implementation of class 'Failure_Base'
    Code-writter: Ruichen Ni
    Date: 2022.9.28
==============================================================*/

#include "./Failure_Base.h"

Failure_Base::Failure_Base()
{
    Type = "";
    Erosion = false;
}

Failure_Base::~Failure_Base()
{
}

bool Failure_Base::Initialize(map<string, MPM_FLOAT> &failure_para, ofstream& os)
{
    for(map<string, MPM_FLOAT>::iterator iter = failure_para.begin(); 
        iter != failure_para.end(); iter++)
    {
        if(ParameterMap_Failure.find(iter->first) != ParameterMap_Failure.end())
            *ParameterMap_Failure[iter->first] = iter->second;
        else if(iter->first == "Erosion")
        {
            if(iter->second > MPM_EPSILON)
                Erosion = true;
        }
        else
        {
            string error_msg = "Can't find the failure model parameter " + iter->first + " at " + Type;
            MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
            MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
            return false;
        }
    }
    return true;
}

bool Failure_Base::AddExtraParticleProperty_Failure(vector<MPM::ExtraParticleProperty> &ExtraProp,
        map<string, MPM_FLOAT>& transfer)
{
    // Nothing needs to be added
    return true;
}