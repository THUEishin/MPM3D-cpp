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
    Info: Implementation of class 'MPM3D'
    Code-writter: Ruichen Ni
    Date: 2022.10.18
==============================================================*/

#include "MPM3D.h"

MPM3D::MPM3D(/* args */)
{
    _n_teststeps = 0;
}

MPM3D::~MPM3D()
{
}

void MPM3D::run(const char *argv)
{

}

void MPM3D::HelpMessage()
{
    ofstream Outs("help.txt");
    PrintLogo(Outs);

    cout << "Usage: MPM3D [options] InputFileName\n"
            << "Options:\n"
            << "-nsteps n  Stop after n console output steps.\n"
            << "--help     Display this information.\n\n";
}

void MPM3D::CommandLineOptions(int argc, char* argv[], vector<string> &paras)
{
    int index = 1;  //!< argv[0] is the executable file name

    while (index < argc)
    {
        if (!strcmp(argv[index], "-nsteps"))
        {
            _n_teststeps = atoi(argv[++index]);
            if (_n_teststeps < 0)
                _n_teststeps = 0;
        }
        else if (!strcmp(argv[index], "--help"))
        {
            HelpMessage();
            exit(0);
        }
        else if (argv[index][0] != '-')
            paras.push_back(argv[index]);
        else if (argv[index][0] == '-')
        {
            HelpMessage();
            cout << "*** Error *** Invalid option: " << argv[index] << endl;
            exit(1);
        }
        index++;
    }

    if (paras.size() == 0)
    {
        HelpMessage();
        cout << "*** Error *** No input file specified!" << endl;
        exit(1);
    }
    else if (paras.size() > 1)
    {
        HelpMessage();
        cout << "*** Error *** Too many input file specified!" << endl;
        exit(1);
    }
    return;
}

void MPM3D::PrintLogo(ofstream& os)
{
    string s1 ("********************************************************************\n");
    string s2 ("*                                                                  *\n");
    string s3 ("*   **       **   *******    **       **   *********   *******     *\n");
    string s4 ("*   **       **   **    **   **       **   ********    ********    *\n");
    string s5 ("*   ***     ***   **    **   ***     ***       **      **    ***   *\n");
    string s6 ("*   ****   ****   **    **   ****   ****      ***      **     **   *\n");
    string s7 ("*   ** ** ** **   **   **    ** ** ** **     *****     **     **   *\n");
    string s8 ("*   **  ***  **   ******     **  ***  **        ***    **     **   *\n");
    string s9 ("*   **  ***  **   **         **  ***  **         ***   **     **   *\n");
    string s10("*   **   *   **   **         **   *   **   ***   ***   **    ***   *\n");
    string s11("*   **       **   **         **       **    *******    ********    *\n");
    string s12("*   **       **   **         **       **     *****     *******     *\n");

    string logo = s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9 + s10 + s11 + s12 + s2 + s1;
    cout << logo;
    os << logo;

    s3 =       "*    MPM3D - Three Dimensional Material Point Method code for      *\n";
    s4 =       "*            hypervelocity impact and high explosive simulation    *\n";
    s5 =       "*    Version          : " + MPM::MPM3DVersion + MPM::MPM3DRevision +
               "                                      *\n";
    s6 =       "*    Compiled Date    : " + MPM::CompileDate + 
               " (" + MPM::CompileTime + ")                     *\n";

    s7 =       "*    Precision        : ";
#ifndef _MPM_DOUBLE
    s7 +=      "Single precision                           *\n";
#else
    s7 +=      "Double precision                           *\n";
#endif

    s8 =       "*    VTKData support  : ";
#ifndef _MPM_NOVTKDATA
    s8 +=      "Enabled                                    *\n";
#else
    s8 +=      "Disabled                                   *\n";
#endif

    s10 =      "*    Activated solvers                                             *\n";
#ifdef MPM_SOLVER
    s10 +=     "*       -MPM solver                                                *\n";
#endif

    string info;
    info = s2 + s3 + s4 + s2 + s5 + s6 + s7 + s8 + s2 + s2 + s10 + s2;
    cout << info;
    os << info;

    s9 =       "*    Computational Dynamics Laboratory                             *\n";
    s10 =      "*    School of Aerospace, Tsinghua University                      *\n";
    s10 +=     "*    Beijing 100084, China                                         *\n";
    s11 =      "*    Tel: +86 10 62782078                                          *\n";
    s11 +=     "*    web: http://comdyn.hy.tsinghua.edu.cn                         *\n";

    string addr;
    addr = s2 + s9 + s10 + s11 + s2;
    cout << addr;
    os << addr;

    cout << s1 + "\n\n";
    os << s1 + "\n\n";
}