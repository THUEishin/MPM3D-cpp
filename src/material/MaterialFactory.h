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
    Info: Material Factory consisted of pointers of Strength,
        EOS and various Failure models. The main purpose of
        material is to update particle stress (deviatoric and
        volumetric) and calculate sound speed.
    Code-writter: Ruichen Ni
    Date: 2022.10.13
==============================================================*/

#ifndef _MATERIALFACTORY_H_
#define _MATERIALFACTORY_H_

#include "StrengthList.h"
#include "EOSList.h"
#include "FailureList.h"

class MaterialFactory
{
public:
    MaterialFactory();
    ~MaterialFactory();

    //!> Initial material with name, parameter map and reference density
    bool Initialize(string& strength_name, map<string, MPM_FLOAT>& strength_para,
                    string& eos_name, map<string, MPM_FLOAT>& eos_para,
                    vector<string>& failure_name_list, vector< map<string, MPM_FLOAT> >& failure_para_list,
                    map<string, MPM_FLOAT>& extra_para, ofstream& os);

    //!> Write material information to file
    void Write(ofstream& os, int number);

    //!> Update stress of deviatoric and volumetric
    void UpdateStress(PhysicalProperty* pp, SymTensor& delta_strain, SymTensor& delta_vortex,
        MPM_FLOAT volume_old);

    //!> Calculate sound speed
    void SoundSpeed(PhysicalProperty* pp);
protected:
    //!> Deal with artificial viscosity
    void ArtificialViscosity(PhysicalProperty* pp, MPM_FLOAT& delta_vol);

    //!> Stress Response when particle failed
    void ResponseFailure(PhysicalProperty* pp, MPM_FLOAT volume_old);
protected:
    Strength_Base*          _strength;
    EOS_Base*               _eos;
    //!> There could be various failure model
    vector<Failure_Base*>   _failure;

    MPM_FLOAT _reference_density;
    MPM_FLOAT _bq1, _bq2;   //!< Artificial viscosity
    MPM_FLOAT _fail_response_type;
    MPM_FLOAT _tensile_cutoff;

    map<string, MPM_FLOAT*> ParameterMap_Material;

//!> Getter/Setter interface
public:
    inline MPM_FLOAT GetReferenceDensity() {return _reference_density;}
};

#endif