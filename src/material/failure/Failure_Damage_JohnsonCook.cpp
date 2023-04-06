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
    Info: Implementation of class 'Failure_Damage_Base'
    Code-writter: Ruichen Ni
    Date: 2022.9.28
==============================================================*/

#include "Failure_Damage_JohnsonCook.h"

Failure_Damage_JohnsonCook::Failure_Damage_JohnsonCook()
{
    Type = "Jhonson-Cook Accumulated Damage";
    _D1 = 0.0;
    _D2 = 0.0;
    _D3 = 0.0;
    _D4 = 0.0;
    _D5 = 0.0;

    ParameterMap_Failure["D1"] = &_D1;
    ParameterMap_Failure["D2"] = &_D2;
    ParameterMap_Failure["D3"] = &_D3;
    ParameterMap_Failure["D4"] = &_D4;
    ParameterMap_Failure["D5"] = &_D5;
}

Failure_Damage_JohnsonCook::~Failure_Damage_JohnsonCook()
{
}

bool Failure_Damage_JohnsonCook::CheckFailure(PhysicalProperty* pp, map<string, MPM_FLOAT>& transfer)
{
    if (pp->is_Failed())
        return true;
    
    MPM_FLOAT lsrate = transfer["lsrate"];      //!< log(strain rate)
    MPM_FLOAT tstar = transfer["tstar"];        //!< dimensionless temperature
    MPM_FLOAT depeff = transfer["depeff"];      //!< plastic strain increment

    MPM_FLOAT sigma_star = pp->GetMeanStress()/(pp->GetEquivalentStress() + MPM_EPSILON);
    MPM_FLOAT strain_fracture = (_D1 + _D2*exp(_D3*sigma_star))*(1 + _D4*lsrate)*(1 + _D5*tstar);

    if (strain_fracture > MPM_EPSILON)
    {
        MPM_FLOAT old_damage = (*pp)[MPM::DMG];
        if (old_damage < 1.0)
        {
            (*pp)[MPM::DMG] += (depeff/strain_fracture);
            if ((*pp)[MPM::DMG] >= 1.0)
                (*pp)[MPM::DMG] = 1.0;
            
            MPM_FLOAT ratio = (1.0 - (*pp)[MPM::DMG])/(1.0 - old_damage);
            pp->DeviatoricStressMultiplyScalar(ratio);
        }

        if (fabs((*pp)[MPM::DMG] - 1.0) <= MPM_EPSILON)
        {
            pp->Failed();
            if (Erosion)
                pp->Eroded();
        }
    }

    return pp->is_Failed();
}

void Failure_Damage_JohnsonCook::Write(ofstream &os)
{
    os << "Failure model: " << Type << endl;
    os << "D1          D2          D3          D4          D5" << endl;
    os << _D1 << " " << _D2 << " " << _D3 << " " << _D4 << " " << _D5 << endl;
    os << "Erosion: " << Erosion << endl << endl;
}

bool Failure_Damage_JohnsonCook::Initialize(map<string, MPM_FLOAT> &failure_para, ofstream& os)
{
    if (!Failure_Base::Initialize(failure_para, os))
        return false;
    
    return true;
}

bool Failure_Damage_JohnsonCook::AddExtraParticleProperty_Failure(vector<MPM::ExtraParticleProperty> &ExtraProp,
        map<string, MPM_FLOAT>& transfer)
{
    ExtraProp.push_back(MPM::DMG);
    return true;
}