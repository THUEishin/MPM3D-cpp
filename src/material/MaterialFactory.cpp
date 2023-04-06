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
    Info: Implementation of class "MaterialFactory"
    Code-writter: Ruichen Ni
    Date: 2022.10.13
==============================================================*/

#include "MaterialFactory.h"
#include "../solver/Solver_Base.h"

MaterialFactory::MaterialFactory()
{
    _strength = nullptr;
    _eos = nullptr;
    _reference_density = 0.0;
    _bq1 = 1.5;
    _bq2 = 0.06;
    _fail_response_type = 0;
    _tensile_cutoff = 0.0;

    ParameterMap_Material["ReferenceDensity"] = &_reference_density;
    ParameterMap_Material["bq1"] = &_bq1;
    ParameterMap_Material["bq2"] = &_bq2;
    ParameterMap_Material["FailedType"] = &_fail_response_type;
    ParameterMap_Material["TensileCutoff"] = &_tensile_cutoff;
}

MaterialFactory::~MaterialFactory()
{
    if (_strength)
        delete _strength;
    
    if (_eos)
        delete _eos;
    
    if (!_failure.empty())
        for (auto failure : _failure)
            delete failure;
}

bool MaterialFactory::Initialize(string& strength_name, map<string, MPM_FLOAT>& strength_para,
                    string& eos_name, map<string, MPM_FLOAT>& eos_para,
                    vector<string>& failure_name_list, vector< map<string, MPM_FLOAT> >& failure_para_list,
                    map<string, MPM_FLOAT>& extra_para, ofstream& os)
{
    //!> Initialize reference density and artificial viscosity
    for(map<string, MPM_FLOAT>::iterator iter = extra_para.begin(); 
        iter != extra_para.end(); iter++)
    {
        if(ParameterMap_Material.find(iter->first) != ParameterMap_Material.end())
            *ParameterMap_Material[iter->first] = iter->second;
        else
        {
            string error_msg = "Can't find the material model parameter: " + iter->first;
            MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
            MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
            return false;
        }
    }

    //!> Strength model
    if (strength_name == "IsoElastic")
        _strength = new Strength_IsoElastic;
    else if (strength_name == "ElaPlastic")
        _strength = new Strength_ElaPlastic;
    else if (strength_name == "IsoHarden")
        _strength = new Strength_IsoHarden;
    else if (strength_name == "JohnsonCook")
        _strength = new Strength_JohnsonCook;
    else if (strength_name == "SimJohnsonCook")
        _strength = new Strength_SimpleJohnsonCook;
    else if (strength_name == "Null")
        _strength = new Strength_Null;
    else
    {
        string error_msg = "*** Input Error *** There is no strength model named " + strength_name + "!";
        MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
        MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
        return false;
    }

    if (!_strength->Initialize(strength_para, _reference_density, os))
        return false;
    
    //!> EOS model
    if (eos_name == "Polynomial")
        _eos = new EOS_Polynomial;
    else if (eos_name == "Gruneisen")
        _eos = new EOS_Gruneisen;
    else if (eos_name == "SimGruneisen")
        _eos = new EOS_SimpleGruneisen;
    else if (eos_name == "JWL")
        _eos = new EOS_JWL;
    else if (eos_name == "HighExpBurn")
        _eos = new EOS_HighExpBurn;
    else if (eos_name != "" && eos_name != "none" && eos_name != "None")
    {
        string error_msg = "*** Input Error *** There is no EOS model named " + eos_name + "!";
        MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
        MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
        return false;
    }

    if (_eos)
        if (!_eos->Initialize(eos_para, _reference_density, os))
            return false;

    //!> Failure model
    if (failure_name_list.empty())
        return true;    //!< There is no failure model
    
    for (int n = 0; n < failure_name_list.size(); n++)
    {
        Failure_Base* failure_temp = nullptr;
        if (failure_name_list[n] == "PlaStrain")
            failure_temp = new Failure_PlaStrain;
        else if (failure_name_list[n] == "PriStrain")
            failure_temp = new Failure_PriStrain;
        else if (failure_name_list[n] == "PriStress")
            failure_temp = new Failure_PriStress;
        else if (failure_name_list[n] == "JohnsonCookDamage")
            failure_temp = new Failure_Damage_JohnsonCook;
        else if (failure_name_list[n] != "" && failure_name_list[n] != "none" &&
                 failure_name_list[n] != "None")
        {
            string error_msg = "*** Input Error *** There is no Failure model named " + failure_name_list[n] + "!";
            MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
            MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
            return false;
        }

        if (failure_temp)
        {
            if (!failure_temp->Initialize(failure_para_list[n], os))
                return false;
            _failure.push_back(failure_temp);
        }
    }
    return true;
}

void MaterialFactory::Write(ofstream& os, int number)
{
    os << "Material #" << number << endl;
    _strength->Write(os);

    if (_eos)
        _eos->Write(os);

    if (!_failure.empty())
        for (auto failure : _failure)
            failure->Write(os);
}

void MaterialFactory::UpdateStress(PhysicalProperty* pp, SymTensor& delta_strain, SymTensor& delta_vortex,
    MPM_FLOAT volume_old)
{
    SymTensor sold = pp->GetDeviatoricStress();
    MPM_FLOAT mean_stress_old = pp->GetMeanStress();
    MPM_FLOAT delta_vol = delta_strain[0] + delta_strain[1] + delta_strain[2];
    MPM_FLOAT volume = pp->GetVolume();
    MPM_FLOAT delta_vol_half = 0.5*(volume - volume_old);
    MPM_FLOAT volume_double = (volume + volume_old);
    MPM_FLOAT delta_ie = 0.0;
    map<string, MPM_FLOAT> data_transfer;

    pp->StressRotationJaumann(delta_vortex);

    _strength->UpdateDeviatoricStress(pp, delta_strain, delta_vortex, data_transfer);

    SoundSpeed(pp);

    ArtificialViscosity(pp, delta_vol);
    
    if (_eos)
    {
        if (pp->is_Failed())
        {
            delta_ie = delta_vol_half*(mean_stress_old - pp->GetBulkViscosity()*2.0);
            _eos->UpdatePressure(pp, delta_vol_half, delta_ie, data_transfer);
            _strength->ModifyPressureByTemperature(pp);
        }
        else
        {
            SymTensor sd = pp->GetDeviatoricStress();
            delta_ie = 0.25*(delta_strain[0]*(sold[0] + sd[0]) + 
                             delta_strain[1]*(sold[1] + sd[1]) +
                             delta_strain[2]*(sold[2] + sd[2]) +
                             delta_strain[3]*(sold[3] + sd[3]) +
                             delta_strain[4]*(sold[4] + sd[4]) +
                             delta_strain[5]*(sold[5] + sd[5]))*volume_double;
            delta_ie += delta_vol_half*(mean_stress_old - pp->GetBulkViscosity()*2.0);

            _eos->UpdatePressure(pp, delta_vol_half, delta_ie, data_transfer);
            _strength->ModifyPressureByTemperature(pp);

            delta_ie += delta_vol_half*pp->GetMeanStress();
        }
    }
    else
    {
        //!> Pressure has already been modified by temperature in "Strength_Isotropic::_ElasticPressure"
        //!> May be moved here to be consistent with above procedure
        _strength->ElasticPressure(pp, delta_vol, data_transfer);

        SymTensor sd = pp->GetDeviatoricStress();
        MPM_FLOAT mean_stress_part = mean_stress_old + pp->GetMeanStress() 
            - pp->GetBulkViscosity()*2.0;
        delta_ie = 0.25*(delta_strain[0]*(sold[0] + sd[0] + mean_stress_part) + 
                         delta_strain[1]*(sold[1] + sd[1] + mean_stress_part) +
                         delta_strain[2]*(sold[2] + sd[2] + mean_stress_part) +
                         delta_strain[3]*(sold[3] + sd[3]) +
                         delta_strain[4]*(sold[4] + sd[4]) +
                         delta_strain[5]*(sold[5] + sd[5]))*volume_double;
    }

    for (auto failure : _failure)
        failure->CheckFailure(pp, data_transfer);
    if (pp->is_Failed())
    {
        ResponseFailure(pp, volume_old);
        delta_ie = delta_vol_half*(mean_stress_old + pp->GetMeanStress() - pp->GetBulkViscosity()*2.0);
    }

    pp->UpdateInternalEnergy(delta_ie);

    _strength->UpdateTemperature(pp, delta_vol, data_transfer);
}

void MaterialFactory::SoundSpeed(PhysicalProperty* pp)
{
    MPM_FLOAT sound_speed_square = _strength->SoundSpeedSquare_Strength(pp);
    if (_eos)
        sound_speed_square += _eos->SoundSpeedSquare_EOS(pp);
    else   
        sound_speed_square += _strength->SoundSpeedSquare_Elastic(pp);
    
    if (sound_speed_square <= -MPM_EPSILON)
    {
        cout << "*** Warning *** The sound speed is negative!" << endl;
        cout << "Strength Type: " << _strength->GetName() << endl;
        if (_eos)
            cout << "EOS Type: " << _eos->GetName() << endl;
        
        pp->SetSoundSpeed(MPM_EPSILON);
    }
    else
        pp->SetSoundSpeed(sqrt(sound_speed_square));
}

void MaterialFactory::ArtificialViscosity(PhysicalProperty* pp, MPM_FLOAT& delta_vol)
{
    MPM_FLOAT bulk_q = 0.0;
    if (pp->GetMeanStress() < -MPM_EPSILON)
    {
        MPM_FLOAT dt = Solver_Base::GetDTn_I();
        MPM_FLOAT volume_rate = delta_vol/dt;
        if (volume_rate < -MPM_EPSILON)
        {
            MPM_FLOAT character_length = pow(pp->GetVolume(), 1.0/3.0);
            MPM_FLOAT sound_speed = pp->GetSoundSpeed();
            bulk_q = pp->GetDensity()*character_length*volume_rate*(_bq1*character_length*
                volume_rate - _bq2*sound_speed);
            
            MPM_FLOAT sound_speed_modified = _bq2*sound_speed - _bq1*character_length*volume_rate;
            sound_speed_modified = sound_speed_modified + 
                sqrt(sound_speed_modified*sound_speed_modified + sound_speed*sound_speed);
            pp->SetSoundSpeed(sound_speed_modified);
        }
    }
    pp->SetBulkViscosity(bulk_q);
}

void MaterialFactory::ResponseFailure(PhysicalProperty* pp, MPM_FLOAT volume_old)
{
    pp->DeviatoricStressMultiplyScalar(0.0);

    switch ((unsigned int)_fail_response_type)
    {
    case 0: //!< no tensile
        if (pp->GetMeanStress() > -MPM_EPSILON)
        {
            pp->SetMeanStress(0.0);
            pp->SetBulkViscosity(0.0);
            pp->SetVolume(volume_old);
            pp->UpdateDensity();
        }
        break;
    
    case 1: //!< no compression and no tensile
        pp->SetMeanStress(0.0);
        pp->SetBulkViscosity(0.0);
        pp->SetVolume(volume_old);
        pp->UpdateDensity();
        break;

    case 2: //!< tensile with cut off
        if (pp->GetMeanStress() > -_tensile_cutoff)
        {
            pp->SetMeanStress(0.0);
            pp->SetBulkViscosity(0.0);
        }
        break;

    default:
        break;
    }
}