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
    Info: Implementation of class 'Failure_PlaStrain'
    Code-writter: Ruichen Ni
    Date: 2022.9.29
==============================================================*/

#include "Failure_PlaStrain.h"
Failure_PlaStrain::Failure_PlaStrain()
{
    Type = "Effective Plastic Strain";
    _epmax = 0.0;

    ParameterMap_Failure["epmax"] = &_epmax;
}

Failure_PlaStrain::~Failure_PlaStrain()
{
}

bool Failure_PlaStrain::CheckFailure(PhysicalProperty* pp, map<string, MPM_FLOAT>& transfer)
{
    if (pp->is_Failed())
        return true;
    
    if ((*pp)[MPM::epeff] > _epmax)
    {
        pp->Failed();
        if (Erosion)
            pp->Eroded();
    }

    return pp->is_Failed();
}

void Failure_PlaStrain::Write(ofstream &os)
{
    os << "Failure model: " << Type << endl;
    os << "epmax: " << _epmax << endl;
    os << "Erosion: " << Erosion << endl << endl;
}

bool Failure_PlaStrain::Initialize(map<string, MPM_FLOAT> &failure_para, ofstream& os)
{
    if (!Failure_Base::Initialize(failure_para, os))
        return false;
    
    if (_epmax <= MPM_EPSILON)
    {
        string error_msg = "*** INPUT ERROR *** epmax should be greater than zero.";
        MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
        MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
        return false;
    }
    return true;
}

bool Failure_PlaStrain::AddExtraParticleProperty_Failure(vector<MPM::ExtraParticleProperty> &ExtraProp,
        map<string, MPM_FLOAT>& transfer)
{
    ExtraProp.push_back(MPM::epeff);
    return true;
}