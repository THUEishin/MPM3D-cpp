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
    Info: Implementation of class 'Strength_Isotropic'
    Code-writter: Ruichen Ni
    Date: 2022.10.9
==============================================================*/

#include "Strength_Isotropic.h"

Strength_Isotropic::Strength_Isotropic()
{
    Type = "";  //!< Strength_Isotropic cannot be Instantiated

    _Young_Modulus = 0.0;
    _Poisson_Rate = 0.0;
    _shear_modulus = 0.0;
    _volumetric_modulus = 0.0;

    _specific_heat = 0.0;
    _temperature_coefficient = 0.0;
    _room_temperature = 293.0;

    ParameterMap_Strength["Young"] = &_Young_Modulus;
    ParameterMap_Strength["Poisson"] = &_Poisson_Rate;
    ParameterMap_Strength["SpecHeat"] = &_specific_heat;
    ParameterMap_Strength["TemperatureCoefficient"] = &_temperature_coefficient;
    ParameterMap_Strength["roomt"] = &_room_temperature;
}

Strength_Isotropic::~Strength_Isotropic()
{
}

bool Strength_Isotropic::Initialize(map<string, MPM_FLOAT> &strength_para, MPM_FLOAT rho0, ofstream& os)
{
    if (!Strength_Base::Initialize(strength_para, rho0, os))
        return false;
    
    _shear_modulus = 0.5*_Young_Modulus/(1.0 + _Poisson_Rate);
    _volumetric_modulus = _Young_Modulus/(3.0*(1.0 - 2.0*_Poisson_Rate));
    return true;
}

MPM_FLOAT Strength_Isotropic::SoundSpeedSquare_Strength(PhysicalProperty* pp)
{
    return FourThird*_shear_modulus/pp->GetDensity();
}

MPM_FLOAT Strength_Isotropic::SoundSpeedSquare_Elastic(PhysicalProperty* pp)
{
    return _volumetric_modulus/pp->GetDensity();
}

void Strength_Isotropic::ModifyPressureByTemperature(PhysicalProperty* pp)
{
    if (!_compute_temperature)
        return;
    MPM_FLOAT mean_stress = pp->GetMeanStress();
    mean_stress -= _temperature_coefficient*((*pp)[MPM::kelvin] - _room_temperature);
    pp->SetMeanStress(mean_stress);
}

bool Strength_Isotropic::AddExtraParticleProperty_Strength(vector<MPM::ExtraParticleProperty> &ExtraProp,
        map<string, MPM_FLOAT>& transfer)
{
    if (_compute_temperature)
    {
        ExtraProp.push_back(MPM::kelvin);
        transfer["roomt"] = _room_temperature;
    }
    return true;
}

void Strength_Isotropic::_ElasticDeviatoricStress(PhysicalProperty* pp, SymTensor& delta_strain)
{
    MPM_FLOAT delta_strain_mean = (delta_strain[0] + delta_strain[1] + delta_strain[2])/3.0;

    SymTensor deviatoric_stress = pp->GetDeviatoricStress();
    deviatoric_stress[0] += 2.0*_shear_modulus*(delta_strain[0] - delta_strain_mean);
    deviatoric_stress[1] += 2.0*_shear_modulus*(delta_strain[1] - delta_strain_mean);
    deviatoric_stress[2] += 2.0*_shear_modulus*(delta_strain[2] - delta_strain_mean);
    deviatoric_stress[3] += _shear_modulus*delta_strain[3];
    deviatoric_stress[4] += _shear_modulus*delta_strain[4];
    deviatoric_stress[5] += _shear_modulus*delta_strain[5];
    pp->SetDeviatoricStress(deviatoric_stress);
}

void Strength_Isotropic::_ElasticPressure(PhysicalProperty* pp, MPM_FLOAT delta_vol)
{
    if (_compute_temperature)
    {
        MPM_FLOAT origin_volume = pp->GetMass()/_density_0;
        MPM_FLOAT mean_stress = _volumetric_modulus*(pp->GetVolume() - origin_volume)/origin_volume - 
            _temperature_coefficient*((*pp)[MPM::kelvin] - _room_temperature);
        pp->SetMeanStress(mean_stress);
    }
    else
    {
        MPM_FLOAT mean_stress = pp->GetMeanStress();
        mean_stress += _volumetric_modulus*delta_vol;
        pp->SetMeanStress(mean_stress);
    }
}

void Strength_Isotropic::_DamageDeviatoricStress(PhysicalProperty* pp, SymTensor& delta_strain)
{
    MPM_FLOAT damage_shear_modulus;
    if (pp->GetMeanStress() - pp->GetBulkViscosity() >= -MPM_EPSILON)   //!< Tension
        damage_shear_modulus = _shear_modulus*(1 - (*pp)[MPM::DMG]);
    else                                                                //!< Compression
        damage_shear_modulus = _shear_modulus;

    MPM_FLOAT delta_strain_mean = (delta_strain[0] + delta_strain[1] + delta_strain[2])/3.0;

    SymTensor deviatoric_stress = pp->GetDeviatoricStress();
    deviatoric_stress[0] += 2.0*damage_shear_modulus*(delta_strain[0] - delta_strain_mean);
    deviatoric_stress[1] += 2.0*damage_shear_modulus*(delta_strain[1] - delta_strain_mean);
    deviatoric_stress[2] += 2.0*damage_shear_modulus*(delta_strain[2] - delta_strain_mean);
    deviatoric_stress[3] += damage_shear_modulus*delta_strain[3];
    deviatoric_stress[4] += damage_shear_modulus*delta_strain[4];
    deviatoric_stress[5] += damage_shear_modulus*delta_strain[5];
    pp->SetDeviatoricStress(deviatoric_stress);
}