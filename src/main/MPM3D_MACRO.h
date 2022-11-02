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
    Info: Define common macros and parameters
    Code-writter: Ruichen Ni
    Date: 2022.9.28
==============================================================*/

#ifndef _MPM3D_MACRO_H_
#define _MPM3D_MACRO_H_

#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

#include "../utility/tinyxml2.h"
using namespace tinyxml2;

#ifdef _MPM_DOUBLE
    typedef double MPM_FLOAT;
    const MPM_FLOAT MPM_EPSILON =   DBL_EPSILON;
    const MPM_FLOAT MPM_MIN =       DBL_MIN;
    const MPM_FLOAT MPM_MAX =       DBL_MAX;
    const MPM_FLOAT PI =            3.14159265358979323846;
    const MPM_FLOAT SQRT3 =         1.7320508075688773;
    constexpr MPM_FLOAT FourThird = 4.0/3.0;
#else
    typedef float MPM_FLOAT;
    const MPM_FLOAT MPM_EPSILON =   FLT_EPSILON;
    const MPM_FLOAT MPM_MIN =       FLT_MIN;
    const MPM_FLOAT MPM_MAX =       FLT_MAX;
    const MPM_FLOAT PI =            3.1415926536F;
    const MPM_FLOAT SQRT3 =         1.7320508076F;
    constexpr MPM_FLOAT FourThird = 4.0F/3.0F;
#endif

#ifdef _MPM_MASSIVE_PARTICLE
    typedef long MPM_STATS;
#else
    typedef int MPM_STATS;
#endif

typedef array<MPM_FLOAT, 3> Array3D;
typedef array<MPM_FLOAT, 6> SymTensor;

namespace MPM{
    const string ProgramType = "MPM3D-CPP";
    const string MPM3DVersion = "1.";       //!< Release version
    const string FileVersion = "1.0";       //!< File format version
    const string MPM3DRevision = "0.0";     //!< Revision number
    const string CompileDate = __DATE__;
    const string CompileTime = __TIME__;

    enum MPMScheme
    {
        USL,        //!< update-stress-late formulation
        MUSL,       //!< modified USL formulation
        USF         //!< update-stress-first formulation
    };

    //!> Number of extra particle properties
    const MPM_STATS ExtraParticlePropertySum = 11;
    //!> Extra property list
    enum ExtraParticleProperty
    {
        Exx, Exy, Exz,
             Eyy, Eyz,
                  Ezz,      //!< Strain
        epeff,              //!< effective plastic strian
        kelvin,             //!< absolute temperature
        DMG,                //!< cumulative damage for failure
        sigma_y,            //!< yield stress
        LT                  //!< light time for Explosive
    };
}

// Some inline function definition

inline void MPM3D_ErrorMessage(string filename, MPM_STATS linenumber, string msg)
{
    cout << "(MPM3D)Runtime Error in " << filename << " line " << linenumber << ":\n    " << msg << endl;
}
#endif