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
#include <iomanip>

MPM3D::MPM3D(/* args */)
{
    _n_teststeps = 0;
}

MPM3D::~MPM3D()
{
}

void MPM3D::run(const char *argv)
{
    _filename = argv;
    string extension;

    //!> Get input file name
    size_t pos = _filename.find_last_of("/");    //!< For Linux system
    if (pos == string::npos)
    {
        pos = _filename.find_last_of("\\");      //!< For Windows system
        if (pos != string::npos)
            _path = _filename.substr(0, pos + 1);
    }
    else
        _path = _filename.substr(0, pos + 1);
    
    pos = _filename.find_last_of(".");
    if (pos == string::npos)
    {
        _filename += ".xml";
        pos = _filename.find_last_of(".");
        extension = "xml";
    }
    else
        extension = _filename.substr(pos + 1);

    //!> parse the input file with "tinyXML" tool
    XMLDocument* doc = new XMLDocument;
    if (extension == "xml")
    {
        if (doc->LoadFile(_filename.c_str()))
        {
            doc->PrintError();
            exit(1);
        }
        _filename = _filename.substr(0, pos);
    }
    else
    {
        string error = "Invalid file extension: " + extension;
        MPM3D_ErrorMessage(__FILE__, __LINE__, error);
        exit(1);
    }

    //!> Initialize the computational domain
    if (!Initialize(doc))
    {
        string error = "Error in initializing MPM3D!";
        MPM3D_ErrorMessage(__FILE__, __LINE__, error);
        _log_file << error << endl;
        exit(3);
    }

    //!> time integration

    return;
}

void MPM3D::HelpMessage()
{
    ofstream Outs("help.log");
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
    string s3 ("*   MM       MM   PPPPPPP    MM       MM   333333333   DDDDDDD     *\n");
    string s4 ("*   MM       MM   PP    PP   MM       MM   33333333    DDDDDDDD    *\n");
    string s5 ("*   MMM     MMM   PP    PP   MMM     MMM       33      DD    DDD   *\n");
    string s6 ("*   MMMM   MMMM   PP    PP   MMMM   MMMM      333      DD     DD   *\n");
    string s7 ("*   MM MM MM MM   PP   PP    MM MM MM MM     33333     DD     DD   *\n");
    string s8 ("*   MM  MMM  MM   PPPPPP     MM  MMM  MM        333    DD     DD   *\n");
    string s9 ("*   MM  MMM  MM   PP         MM  MMM  MM         333   DD     DD   *\n");
    string s10("*   MM   M   MM   PP         MM   M   MM   333   333   DD    DDD   *\n");
    string s11("*   MM       MM   PP         MM       MM    3333333    DDDDDDDD    *\n");
    string s12("*   MM       MM   PP         MM       MM     33333     DDDDDDD     *\n");

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

bool MPM3D::Initialize(XMLDocument* doc)
{
    string log_filename = _filename + ".log";
    _log_file.open(log_filename.c_str());
    _log_file.setf(ios::scientific|ios::right);
    _log_file << setprecision(4);

    cout.setf(ios::scientific|ios::right);
    cout << setprecision(4);

    //!> Print program information
    PrintLogo(_log_file);

    //!> Initialize computational region: grid, body and material
    _region = new Domain;
    if (!_region->Initialize(doc, _log_file))
        return false;

    //!> Initialize outputter

    //!> Initialize solver

    return true;
}