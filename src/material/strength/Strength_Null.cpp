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
    Info: Implementation of class 'Strength_Null'
    Code-writter: Ruichen Ni
    Date: 2022.10.11
==============================================================*/

#include "Strength_Null.h"
#include "../../solver/Solver_Base.h"

Strength_Null::Strength_Null()
{
    Type = "Null: Null strength model(should be used with EOS)";

    _mu = 0.0;
    _ck = 0.0;
    _nn = 1.0;

    ParameterMap_Strength["mu"] = &_mu;
    ParameterMap_Strength["ck"] = &_ck;
    ParameterMap_Strength["nn"] = &_nn;
}

Strength_Null::~Strength_Null()
{
}

bool Strength_Null::Initialize(map<string, MPM_FLOAT> &strength_para, MPM_FLOAT rho0, ofstream& os)
{
    if (!Strength_Base::Initialize(strength_para, rho0, os))
        return false;
    
    if (_mu > MPM_EPSILON && _ck > MPM_EPSILON)
    {
        string error_msg = "*** Error *** Null strength cannot be set as both Newton and non-Newton!";
        MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
        MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
        return false;
    }
    return true;
}

void Strength_Null::Write(ofstream& os)
{
    os << "Strength Type: " << Type << endl;
    os << "Density     mu         ck         nn" << endl;
    os << _density_0 << " " << _mu << " " << _ck << " " << _nn << endl;
    os << endl;
}

void Strength_Null::UpdateDeviatoricStress(PhysicalProperty* pp, SymTensor& delta_strain, 
        SymTensor& delta_vortex, map<string, MPM_FLOAT>& transfer)
{
    if (_mu > MPM_EPSILON)
    {
        MPM_FLOAT delta_strain_mean = (delta_strain[0] + delta_strain[1] + delta_strain[2])/3.0;
        MPM_FLOAT mu_half = _mu*0.5;
        MPM_FLOAT dt = Solver_Base::GetDTn_I();
        SymTensor sd;

        sd[0] = _mu*(delta_strain[0] - delta_strain_mean)/dt;
        sd[1] = _mu*(delta_strain[1] - delta_strain_mean)/dt;
        sd[2] = _mu*(delta_strain[2] - delta_strain_mean)/dt;
        sd[3] = mu_half*delta_strain[3]/dt;
        sd[4] = mu_half*delta_strain[4]/dt;
        sd[5] = mu_half*delta_strain[5]/dt;

        pp->SetDeviatoricStress(sd);
    }
    else if (_ck > MPM_EPSILON)
    {
        MPM_FLOAT delta_strain_mean = (delta_strain[0] + delta_strain[1] + delta_strain[2])/3.0;
        MPM_FLOAT dt = Solver_Base::GetDTn_I();
        Array3D sde;
        sde[0] = delta_strain[0] - delta_strain_mean;
        sde[1] = delta_strain[1] - delta_strain_mean;
        sde[2] = delta_strain[2] - delta_strain_mean;

        SymTensor sd;
        sd[0] = (sde[0]>=0 ? 1.0:-1.0)*_ck*pow(fabs(sde[0])/dt, _nn);
        sd[1] = (sde[1]>=0 ? 1.0:-1.0)*_ck*pow(fabs(sde[1])/dt, _nn);
        sd[2] = (sde[2]>=0 ? 1.0:-1.0)*_ck*pow(fabs(sde[2])/dt, _nn);
        sd[3] = (delta_strain[3]>=0 ? 1.0:-1.0)*_ck*pow(0.5*fabs(delta_strain[3])/dt, _nn);
        sd[4] = (delta_strain[4]>=0 ? 1.0:-1.0)*_ck*pow(0.5*fabs(delta_strain[4])/dt, _nn);
        sd[5] = (delta_strain[5]>=0 ? 1.0:-1.0)*_ck*pow(0.5*fabs(delta_strain[5])/dt, _nn);

        pp->SetDeviatoricStress(sd);
    }
    else
    {
        SymTensor sd{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        pp->SetDeviatoricStress(sd);
    }
}

void Strength_Null::ElasticPressure(PhysicalProperty* pp, MPM_FLOAT delta_vol,
        map<string, MPM_FLOAT>& transfer)
{
    string error_msg = "*** Error *** Null strength model should be used with EOS!";
    MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
    exit(1);
}

MPM_FLOAT Strength_Null::SoundSpeedSquare_Strength(PhysicalProperty* pp)
{
    return 0.0;
}

MPM_FLOAT Strength_Null::SoundSpeedSquare_Elastic(PhysicalProperty* pp)
{
    string error_msg = "*** Error *** Null strength model should be used with EOS!";
    MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
    exit(1);
}

void Strength_Null::UpdateTemperature(PhysicalProperty* pp, MPM_FLOAT delta_vol,
        map<string, MPM_FLOAT>& transfer)
{
    //!> Leave to be implemented
    return;
}

void Strength_Null::ModifyPressureByTemperature(PhysicalProperty* pp)
{
    //!> Leave to be implemented
    return;
}