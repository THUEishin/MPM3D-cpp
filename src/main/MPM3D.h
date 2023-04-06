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
    Info: Define the solving including parsing, loading, saving 
        and solver
    Code-writter: Ruichen Ni
    Date: 2022.10.18
==============================================================*/

#ifndef _MPM3D_H_
#define _MPM3D_H_

#include "MPM3D_MACRO.h"
#include "Domain.h"

class MPM3D
{
public:
    MPM3D();
    ~MPM3D();

    //!> Execute MPM3D simulation
    void run(const char *argv);

    //!> Print help message
    void HelpMessage();

    //!> Parse command line options
    void CommandLineOptions(int argc, char* argv[], vector<string> &paras);

    //!> Print logo
    void PrintLogo(ofstream& os);

    //!> Initialize the computational region, solver and outputter
    bool Initialize(XMLDocument* doc);
private:
    int _n_teststeps;       //!< Stop after n console output steps
    string _path;           //!< path of input file
    string _filename;       //!< input file name

    ofstream _log_file;     //!< print console echo information into .log file

    Domain* _region;        //!< computational domain
};

#endif