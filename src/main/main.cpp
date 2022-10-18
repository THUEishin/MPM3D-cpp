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
    Info: Program entry
    Code-writter: Ruichen Ni
    Date: 2022.10.18
==============================================================*/

#include "MPM3D.h"

int main(int argc, char *argv[])
{
    MPM3D* mpm = new MPM3D;

    if (argc < 2)
    {
        mpm->HelpMessage();
        exit(1);
    }

    vector<string> parameters;
    mpm->CommandLineOptions(argc, argv, parameters);
    mpm->run(parameters[0].c_str());

    delete mpm;
    return 0;
}