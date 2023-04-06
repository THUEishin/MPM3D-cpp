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
    Info: Define the computational region, including body, grid
        and material
    Code-writter: Ruichen Ni
    Date: 2022.10.19
==============================================================*/

#ifndef _DOMAIN_H_
#define _DOMAIN_H_

#include "MPM3D_MACRO.h"

class Domain
{
public:
    Domain();
    ~Domain();

    //!> Initialize the computational region with xml input file
    bool Initialize(XMLDocument* doc, ofstream& os);
private:
    string _header;         // Description of the problem
};

#endif