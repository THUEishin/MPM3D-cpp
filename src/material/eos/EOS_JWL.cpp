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
    Info: Implementation of class "EOS_JWL"
    Code-writter: Ruichen Ni
    Date: 2022.10.5
==============================================================*/

#include "EOS_JWL.h"

EOS_JWL::EOS_JWL()
{
    Type = "Jones-Wilkins-Lee EOS";

    ParameterMap_EOS["A"] = &_A;
    ParameterMap_EOS["B"] = &_B;
    ParameterMap_EOS["R1"] = &_R1;
    ParameterMap_EOS["R2"] = &_R2;
    ParameterMap_EOS["w"] = &_w;
}

EOS_JWL::~EOS_JWL()
{
}

bool EOS_JWL::Initialize(map<string, MPM_FLOAT> &eos_para, MPM_FLOAT rho0, ofstream& os)
{
    if (!EOS_Base::Initialize(eos_para, rho0, os))
        return false;
    return true;
}

void EOS_JWL::Write(ofstream& os)
{
    os << "EOS Type: " << Type << endl;
    os << "A          B          R1          R2          w          E0" << endl;
    os << _A << " " << _B << " " << _R1 << " " << _R2 << " " << _w << " " << _internal_energy_0 << endl << endl;
}

void EOS_JWL::UpdatePressure(PhysicalProperty* pp, MPM_FLOAT delta_vol_half, 
    MPM_FLOAT delta_ie, map<string, MPM_FLOAT>& transfer)
{
    MPM_FLOAT V0 = pp->GetMass()/_density_0;
    MPM_FLOAT E = (pp->GetInternalEnergy() + delta_ie)/V0;
    MPM_FLOAT rv = _density_0/pp->GetDensity();     //!< Relative volume

    MPM_FLOAT R1rv = _R1*rv;
    MPM_FLOAT R2rv = _R2*rv;
    MPM_FLOAT Aw_R1rv = _A*_w/R1rv;
    MPM_FLOAT Bw_R2rv = _B*_w/R2rv;
    MPM_FLOAT A_Aw_R1rv = _A - Aw_R1rv;
    MPM_FLOAT B_Bw_R2rv = _B - Bw_R2rv;
    MPM_FLOAT exp_R1rv = exp(-R1rv);
    MPM_FLOAT exp_R2rv = exp(-R2rv);

    MPM_FLOAT A = A_Aw_R1rv*exp_R1rv + B_Bw_R2rv*exp_R2rv;
    MPM_FLOAT B = _w/rv;

    MPM_FLOAT pressure_new = (A + B*E)/(1 + B*delta_vol_half/V0);

    //!> LSDYNA
    if (pressure_new < MPM_EPSILON)
        pressure_new = 0.0;
    
    pp->SetMeanStress(-pressure_new);
    return;
}

MPM_FLOAT EOS_JWL::SoundSpeedSquare_EOS(PhysicalProperty* pp)
{
    MPM_FLOAT pressure = -pp->GetMeanStress();
    MPM_FLOAT rv = _density_0/pp->GetDensity();     //!< Relative volume
    MPM_FLOAT E = pp->GetInternalEnergy()*_density_0/pp->GetMass();

    MPM_FLOAT R1rv = _R1*rv;
    MPM_FLOAT R2rv = _R2*rv;
    MPM_FLOAT Aw_R1rv = _A*_w/R1rv;
    MPM_FLOAT Bw_R2rv = _B*_w/R2rv;
    MPM_FLOAT A_Aw_R1rv = _A - Aw_R1rv;
    MPM_FLOAT B_Bw_R2rv = _B - Bw_R2rv;
    MPM_FLOAT exp_R1rv = exp(-R1rv);
    MPM_FLOAT exp_R2rv = exp(-R2rv);

    MPM_FLOAT B = _w/rv;

    MPM_FLOAT ca = (_R1*A_Aw_R1rv - Aw_R1rv/rv)*exp_R1rv;
    MPM_FLOAT cb = (_R2*B_Bw_R2rv - Bw_R2rv/rv)*exp_R2rv;

    MPM_FLOAT result = rv*rv/_density_0*(ca + cb + B*E/rv) + pressure*_w/pp->GetDensity();
    return result;
}