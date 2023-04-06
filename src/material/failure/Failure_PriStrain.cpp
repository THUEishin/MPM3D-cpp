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
    Info: Implementation of class 'Failure_PriStrain'
    Code-writter: Ruichen Ni
    Date: 2022.9.29
==============================================================*/

#include "Failure_PriStrain.h"
#include "../../utility/MathFunctionList.h"

Failure_PriStrain::Failure_PriStrain()
{
    Type = "Principle Strain";
    _min_principle_strain = 0.0;
    _max_principle_strain = 0.0;
    _max_shear_strain = 0.0;

    ParameterMap_Failure["PriStrainMin"] = &_min_principle_strain;
    ParameterMap_Failure["PriStrainMax"] = &_max_principle_strain;
    ParameterMap_Failure["ShearStrainMax"] = &_max_shear_strain;
}

Failure_PriStrain::~Failure_PriStrain()
{
}

bool Failure_PriStrain::CheckFailure(PhysicalProperty* pp, map<string, MPM_FLOAT>& transfer)
{
    if (pp->is_Failed())
        return true;
    
    Array3D principle_strain;
    MPM_FLOAT I_1 = (*pp)[MPM::Exx] + (*pp)[MPM::Eyy] + (*pp)[MPM::Ezz];
    MPM_FLOAT I_2 = -0.5*(I_1*I_1 - ((*pp)[MPM::Exx]*(*pp)[MPM::Exx] +
        (*pp)[MPM::Eyy]*(*pp)[MPM::Eyy] + (*pp)[MPM::Ezz]*(*pp)[MPM::Ezz] +
        2.0*((*pp)[MPM::Exy]*(*pp)[MPM::Exy] + (*pp)[MPM::Exz]*(*pp)[MPM::Exz] +
             (*pp)[MPM::Eyz]*(*pp)[MPM::Eyz])));
    MPM_FLOAT I_3 = (*pp)[MPM::Exx]*(*pp)[MPM::Eyy]*(*pp)[MPM::Ezz] + 
        2.0*(*pp)[MPM::Exy]*(*pp)[MPM::Eyz]*(*pp)[MPM::Exz] - 
            (*pp)[MPM::Exx]*(*pp)[MPM::Eyz]*(*pp)[MPM::Eyz] - 
            (*pp)[MPM::Eyy]*(*pp)[MPM::Exz]*(*pp)[MPM::Exz] - 
            (*pp)[MPM::Ezz]*(*pp)[MPM::Exy]*(*pp)[MPM::Exy];
              
    CubicFunctionRoots(1.0, -I_1, -I_2, -I_3, principle_strain);
    bool failure;
    MPM_FLOAT min_principle_strain = *min_element(principle_strain.begin(), principle_strain.end());
    MPM_FLOAT max_principle_strain = *max_element(principle_strain.begin(), principle_strain.end());
    MPM_FLOAT max_shear_strain = (max_principle_strain - min_principle_strain)*0.5;
    if (_min_principle_strain < -MPM_EPSILON &&
        min_principle_strain < -MPM_EPSILON &&
        min_principle_strain < _min_principle_strain)
        failure = true;

    if (_max_principle_strain > MPM_EPSILON &&
        max_principle_strain > MPM_EPSILON && 
        max_principle_strain > _min_principle_strain)
        failure = true;

    if (_max_shear_strain > MPM_EPSILON &&
        max_shear_strain > MPM_EPSILON && 
        max_shear_strain > _max_shear_strain)
        failure = true;

    if (failure)
    {
        pp->Failed();
        if(Erosion)
            pp->Eroded();
    }
    return pp->is_Failed();
}

void Failure_PriStrain::Write(ofstream &os)
{
    os << "Failure model: " << Type << endl;
    os << "PriStrainMin: " << _min_principle_strain << endl;
    os << "PriStrainMax: " << _max_principle_strain << endl;
    os << "ShearStrainMax: " << _max_shear_strain << endl;
    os << "Erosion: " << Erosion << endl << endl;
}

bool Failure_PriStrain::Initialize(map<string, MPM_FLOAT> &failure_para, ofstream& os)
{
    if (!Failure_Base::Initialize(failure_para, os))
        return false;
    
    if (_min_principle_strain >= -MPM_EPSILON)
    {
        string error_msg = "*** INPUT ERROR *** min_principle_strain should be less than zero.";
        MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
        MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
        return false;
    }

    if (_max_principle_strain <= MPM_EPSILON)
    {
        string error_msg = "*** INPUT ERROR *** max_principle_strain should be greater than zero.";
        MPM3D_ErrorMessage(__FILE__, __LINE__, error_msg);
        MPM3D_ErrorMessage_log(__FILE__, __LINE__, error_msg, os);
        return false;
    }

    if (_max_shear_strain <= MPM_EPSILON)
    {
        MPM3D_ErrorMessage(__FILE__, __LINE__, 
            "*** INPUT ERROR *** max_shear_strain should be greater than zero.");
        return false;
    }
    return true;
}

bool Failure_PriStrain::AddExtraParticleProperty_Failure(vector<MPM::ExtraParticleProperty> &ExtraProp,
        map<string, MPM_FLOAT>& transfer)
{
    ExtraProp.push_back(MPM::Exx);
    ExtraProp.push_back(MPM::Exy);
    ExtraProp.push_back(MPM::Exz);
    ExtraProp.push_back(MPM::Eyy);
    ExtraProp.push_back(MPM::Eyz);
    ExtraProp.push_back(MPM::Ezz);
    return true;
}