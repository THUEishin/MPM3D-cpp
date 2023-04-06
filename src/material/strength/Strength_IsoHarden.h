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
    Info: Class definition for Isotropic Hardening elastic 
        plastic strength behavior of material
    Code-writter: Ruichen Ni
    Date: 2022.10.11
==============================================================*/

#ifndef _STRENGTH_ISOHARDEN_H_
#define _STRENGTH_ISOHARDEN_H_

#include "Strength_ElaPlastic.h"

class Strength_IsoHarden: public Strength_ElaPlastic
{
public:
    Strength_IsoHarden();
    ~Strength_IsoHarden();

    //!> Initial the strength model with parameters' map
    virtual bool Initialize(map<string, MPM_FLOAT> &strength_para, MPM_FLOAT rho0, ofstream& os);

    //!> Write strength model information into file
    virtual void Write(ofstream &os);

    //!> Update the deviatoric stress of the particle
    virtual void UpdateDeviatoricStress(PhysicalProperty* pp, SymTensor& delta_strain, 
        SymTensor& delta_vortex, map<string, MPM_FLOAT>& transfer);
protected:
    MPM_FLOAT _tangential_modulus;
    MPM_FLOAT _plastic_modulus;
};

#endif