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
    Info: Implementation of class "EOS_HighExpBurn"
    Code-writter: Ruichen Ni
    Date: 2022.10.11
==============================================================*/

#include "EOS_HighExpBurn.h"
#include "../../solver/Solver_Base.h"

EOS_HighExpBurn::EOS_HighExpBurn()
{
    Type = "Explosive: High explosive burn";

    _detonation_velocity = 0.0;
    _F1_coefficient = 0.0;
    _F2_coefficient = 0.0;
    _pressure_CJ = 0.0;
    _character_length = 0.0;
    _beta_burning = false;
    _programed_burning = true;

    ParameterMap_EOS["D"] = &_detonation_velocity;
    ParameterMap_EOS["PCJ"] = &_pressure_CJ;
    ParameterMap_EOS["h"] = &_character_length;
}

EOS_HighExpBurn::~EOS_HighExpBurn()
{
}

bool EOS_HighExpBurn::Initialize(map<string, MPM_FLOAT> &eos_para, MPM_FLOAT rho0, ofstream& os)
{
    for(map<string, MPM_FLOAT>::iterator iter = eos_para.begin(); 
        iter != eos_para.end(); iter++)
    {
        if(ParameterMap_EOS.find(iter->first) != ParameterMap_EOS.end())
            *ParameterMap_EOS[iter->first] = iter->second;
        else if (iter->first == "beta")
            if (iter->second > MPM_EPSILON)
                _beta_burning = true;
        else if (iter->first == "programed")
            if (iter->second > MPM_EPSILON)
                _programed_burning = true;
        else
        {
            string error_msg = "Can't find the EOS model parameter " + iter->first + " at " + Type;
            MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
            MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
            return false;
        }
    }

    _density_0 = rho0;
    
    if (_programed_burning && _character_length < MPM_EPSILON)
    {
        string error_msg = "*** Error *** Character length of particle needs to be specified in High explosive burn model.";
        MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
        MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
        return false;
    }

    if (_beta_burning && _pressure_CJ < MPM_EPSILON)
    {
        string error_msg = "*** Error *** Invalid CJ pressure value.";
        MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
        MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
        return false;
    }

    if (!(_beta_burning || _programed_burning))
    {
        string error_msg = "*** Error *** There should be at least one burning method.";
        MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
        MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
        return false;
    }

    _sound_speed_0 = _detonation_velocity;

    if (_programed_burning)
        _F1_coefficient = _detonation_velocity/(1.5*_character_length);
    
    if (_beta_burning)
        _F2_coefficient = _density_0*_detonation_velocity*_detonation_velocity/_pressure_CJ;
    
    return true;
}

void EOS_HighExpBurn::Write(ofstream& os)
{
    os << "EOS Type: " << Type << endl;
    os << "A          B          R1          R2          w          E0          D" << endl;
    os << _A << " " << _B << " " << _R1 << " " << _R2 
       << " " << _w << " " << _internal_energy_0 << " " << _detonation_velocity << endl;
    os << endl;
}

void EOS_HighExpBurn::UpdatePressure(PhysicalProperty* pp, MPM_FLOAT delta_vol_half, 
        MPM_FLOAT delta_ie, map<string, MPM_FLOAT>& transfer)
{
    MPM_FLOAT fraction = CalculateBurningFraction(pp);
    EOS_JWL::UpdatePressure(pp, delta_vol_half, delta_ie, transfer);

    MPM_FLOAT mean_stress = pp->GetMeanStress();
    pp->SetMeanStress(mean_stress*fraction);
}

MPM_FLOAT EOS_HighExpBurn::SoundSpeedSquare_EOS(PhysicalProperty* pp)
{
    MPM_FLOAT fraction = 1.0 - CalculateBurningFraction(pp);
    MPM_FLOAT result = EOS_JWL::SoundSpeedSquare_EOS(pp);
    result = max(result, _detonation_velocity*_detonation_velocity*fraction*fraction);
    return result;
}

bool EOS_HighExpBurn::AddExtraParticleProperty_EOS(vector<MPM::ExtraParticleProperty> &ExtraProp,
        map<string, MPM_FLOAT>& transfer)
{
    if (!EOS_JWL::AddExtraParticleProperty_EOS(ExtraProp, transfer))
        return false;
    
    ExtraProp.push_back(MPM::LT);
    return true;
}

MPM_FLOAT EOS_HighExpBurn::CalculateBurningFraction(PhysicalProperty* pp)
{
    MPM_FLOAT F = 0.0;
    MPM_FLOAT F1 = 0.0;
    MPM_FLOAT F2 = 0.0;
    MPM_FLOAT current_time = Solver_Base::GetCurrentTime();

    if (_programed_burning)
        if (current_time > (*pp)[MPM::LT])
            F1 = (current_time - (*pp)[MPM::LT])*_F1_coefficient;

    if (_beta_burning)
    {
        MPM_FLOAT origin_volume = pp->GetMass()/_density_0;
        F2 = _F2_coefficient*(1.0 - pp->GetVolume()/origin_volume);
    }

    if (F1 > 1.0) F1 = 1.0;
    if (F2 > 0.95) F2 = 1.0;

    F = max(F1, F2);
    if (F < 0.0001) F = 0.0;
    return F;
}