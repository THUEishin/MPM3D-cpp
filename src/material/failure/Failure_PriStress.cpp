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
    Info: Implementation of class 'Failure_PriStress'
    Code-writter: Ruichen Ni
    Date: 2022.10.4
==============================================================*/

#include "Failure_PriStress.h"

Failure_PriStress::Failure_PriStress()
{
    Type = "Principle Stress";
    _min_principle_stress = 0.0;
    _max_principle_stress = 0.0;
    _max_shear_stress = 0.0;

    ParameterMap_Failure["PriStressMin"] = &_min_principle_stress;
    ParameterMap_Failure["PriStressMax"] = &_max_principle_stress;
    ParameterMap_Failure["ShearStressMax"] = &_max_shear_stress;
}

Failure_PriStress::~Failure_PriStress()
{
}

bool Failure_PriStress::CheckFailure(PhysicalProperty* pp, map<string, MPM_FLOAT>& transfer)
{
    if (pp->is_Failed()) 
        return true;

    Array3D principle_stress = pp->CalculatePrincipleStress();
    MPM_FLOAT min_principle_stress = *min_element(principle_stress.begin(), principle_stress.end());
    MPM_FLOAT max_principle_stress = *max_element(principle_stress.begin(), principle_stress.end());
    MPM_FLOAT max_shear_stress = (max_principle_stress - min_principle_stress)*0.5;

    bool failure = false;
    if (_min_principle_stress < -MPM_EPSILON &&
        min_principle_stress < -MPM_EPSILON &&
        min_principle_stress < _min_principle_stress)
        failure = true;
    
    if (_max_principle_stress > MPM_EPSILON &&
        max_principle_stress > MPM_EPSILON &&
        max_principle_stress > _max_principle_stress)
        failure = true;

    if (_max_shear_stress > MPM_EPSILON &&
        max_shear_stress > MPM_EPSILON &&
        max_shear_stress > _max_shear_stress)
        failure = true;
    
    if (failure)
    {
        pp->Failed();
        if(Erosion)
            pp->Eroded();
    }
    return pp->is_Failed();
}

void Failure_PriStress::Write(ofstream &os)
{
    os << "Failure model: " << Type << endl;
    os << "PriStressMin: " << _min_principle_stress << endl;
    os << "PriStressMax: " << _max_principle_stress << endl;
    os << "ShearStressMax: " << _max_shear_stress << endl;
    os << "Erosion: " << Erosion << endl << endl;
}

bool Failure_PriStress::Initialize(map<string, MPM_FLOAT> &failure_para, ofstream& os)
{
    if (!Failure_Base::Initialize(failure_para, os))
        return false;
    
    if (_min_principle_stress >= -MPM_EPSILON)
    {
        string error_msg = "*** INPUT ERROR *** min_principle_strain should be less than zero.";
        MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
        MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
        return false;
    }

    if (_max_principle_stress <= MPM_EPSILON)
    {
        string error_msg = "*** INPUT ERROR *** max_principle_strain should be greater than zero.";
        MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
        MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
        return false;
    }

    if (_max_shear_stress <= MPM_EPSILON)
    {
        MPM3D_ErrorMessage(__FILE__, __LINE__, 
            "*** INPUT ERROR *** max_shear_strain should be greater than zero.");
        return false;
    }
    return true;
}