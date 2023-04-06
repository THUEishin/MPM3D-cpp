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
    Info: Implementation of class 'Strength_IsoHarden'
    Code-writter: Ruichen Ni
    Date: 2022.10.11
==============================================================*/

#include "Strength_IsoHarden.h"

Strength_IsoHarden::Strength_IsoHarden()
{
    Type = "ISO-Plasticity: Isotropic hardening plasticity";

    ParameterMap_Strength["TangMod"] = &_tangential_modulus;
}

Strength_IsoHarden::~Strength_IsoHarden()
{
}

bool Strength_IsoHarden::Initialize(map<string, MPM_FLOAT> &strength_para, MPM_FLOAT rho0, ofstream& os)
{
    if (!Strength_ElaPlastic::Initialize(strength_para, rho0, os))
        return false;
    
    _plastic_modulus = _Young_Modulus*_tangential_modulus/(_Young_Modulus - _tangential_modulus);
    return true;
}

void Strength_IsoHarden::Write(ofstream& os)
{
    os << "Strength Type: " << Type << endl;
    os << "Density     E          Poisson        Sigma_y        TangMod" << endl;
    os << _density_0 << " " << _Young_Modulus << " " << _Poisson_Rate << " " 
       << _yield_0 << " " << _tangential_modulus << endl;
    if (_compute_temperature)
    {
        os << "Room Temp.    Temp. Coef.    Plas. Work Coef." << endl;
        os << _room_temperature << " " << _temperature_coefficient << " " << _plastic_work_coefficient << endl;
    }
    os << endl;
}

void Strength_IsoHarden::UpdateDeviatoricStress(PhysicalProperty* pp, SymTensor& delta_strain, 
        SymTensor& delta_vortex, map<string, MPM_FLOAT>& transfer)
{
    _ElasticDeviatoricStress(pp, delta_strain);

    pp->EquivalentStress();
    MPM_FLOAT seqv = pp->GetEquivalentStress();

    MPM_FLOAT depeff = 0.0;
    transfer["yield"] = -1.0;
    if (seqv > _yield_0)    //!< when the yield condition is violated
    {
        depeff = (seqv - (*pp)[MPM::sigma_y])/(3.0*_shear_modulus + _plastic_modulus);
        transfer["depeff"] = depeff;

        (*pp)[MPM::epeff] += depeff;
        (*pp)[MPM::sigma_y] += _plastic_modulus*depeff;
        MPM_FLOAT ratio = (*pp)[MPM::sigma_y]/seqv;
        if (ratio < 1.0)
        {
            transfer["yield"] = 1.0;
            pp->DeviatoricStressMultiplyScalar(ratio);
            pp->SetEquivalentStress((*pp)[MPM::sigma_y]);
        }
    }
}