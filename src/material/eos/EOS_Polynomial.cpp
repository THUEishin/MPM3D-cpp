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
    Info: Implementation of class "EOS_Polynomial"
    Code-writter: Ruichen Ni
    Date: 2022.10.5
==============================================================*/

#include "EOS_Polynomial.h"

EOS_Polynomial::EOS_Polynomial(/* args */)
{
    Type = "Polynomial EOS of LS-DYNA";

    ParameterMap_EOS["c0"] = &_c0;
    ParameterMap_EOS["c1"] = &_c1;
    ParameterMap_EOS["c2"] = &_c2;
    ParameterMap_EOS["c3"] = &_c3;
    ParameterMap_EOS["c4"] = &_c4;
    ParameterMap_EOS["c5"] = &_c5;
    ParameterMap_EOS["c6"] = &_c6;
}

EOS_Polynomial::~EOS_Polynomial()
{
}

bool EOS_Polynomial::Initialize(map<string, MPM_FLOAT> &eos_para, MPM_FLOAT rho0, ofstream& os)
{
    if (!EOS_Base::Initialize(eos_para, rho0, os))
        return false;
    _sound_speed_0 = sqrt((_c1 + _c5*_internal_energy_0)/_density_0);
    return true;
}

void EOS_Polynomial::Write(ofstream& os)
{
    os << "EOS Type: " << Type << endl;
    os << "c0          c1          c2          c3          c4          c5          c6          E0" << endl;
    os << _c0 << " " << _c1 << " " << _c2 << " " << _c3 << " " << _c4 << " " << _c5 << " " << _c6 << " " << _internal_energy_0 << endl;
    os << "Initial sound speed: " << _sound_speed_0 << endl << endl;
}

void EOS_Polynomial::UpdatePressure(PhysicalProperty* pp, MPM_FLOAT delta_vol_half, 
    MPM_FLOAT delta_ie, map<string, MPM_FLOAT>& transfer)
{
    MPM_FLOAT V0 = pp->GetMass()/_density_0;
    MPM_FLOAT E = (pp->GetInternalEnergy() + delta_ie)/V0;
    MPM_FLOAT mu = pp->GetDensity()/_density_0 - 1.0;

    MPM_FLOAT A, B;
    //!> c2 and c6 are set to zero when mu < 0 (material in tension)
    if (mu < -MPM_EPSILON)
    {
        A = _c0 + mu*(_c1 + mu*mu*_c3);
        B = _c4 + mu*_c5;
    }
    else
    {
        A = _c0 + mu*(_c1 + mu*(_c2 + mu*_c3));
        B = _c4 + mu*(_c5 + mu*_c6);
    }

    MPM_FLOAT pressure_new = (A + B*E)/(1 + B*delta_vol_half/V0);
    pp->SetMeanStress(-pressure_new);
    return;
}

MPM_FLOAT EOS_Polynomial::SoundSpeedSquare_EOS(PhysicalProperty* pp)
{
    if (pp->is_Failed() || _density_0 <= MPM_EPSILON)
        return 0.0;
    
    MPM_FLOAT pressure = -pp->GetMeanStress();
    MPM_FLOAT rv = _density_0/pp->GetDensity();     //!< Relative volume
    MPM_FLOAT mu = 1.0/rv - 1.0;
    MPM_FLOAT E = pp->GetInternalEnergy()*_density_0/pp->GetMass();

    MPM_FLOAT B, C, D;
    if (mu < -MPM_EPSILON)
    {
        B = _c4 + mu*_c5;
        C = _c1 + mu*3.0*_c3*mu;
        D = _c5;
    }
    else
    {
        B = _c4 + mu*(_c5 + mu*_c6);
        C = _c1 + mu*(2.0*_c2 + 3.0*_c3*mu);
        D = _c5 + 2.0*_c6*mu;
    }

    MPM_FLOAT result = (C + D*E + B*pressure*rv*rv)/_density_0;
    return result;
}