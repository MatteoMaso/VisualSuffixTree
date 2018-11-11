//
// Created by root on 11/11/18.
//

#include "Logger.h"
//#include"Utilities.h"
#include "Utils.hpp"
#include <iostream>
#include <string>
#include <time.h>
//#include <stdio.h>
//#include <fstream>      // std::ofstream


const string CLogger::m_sFileName = "Log.txt";
bool VERBOSE_LOGGER;
CLogger* CLogger::m_pThis = NULL;
//ofstream CLogger::m_Logfile;
CLogger::CLogger()
{
    VERBOSE_LOGGER = false;
}
CLogger* CLogger::GetLogger(){
    if (m_pThis == NULL){
        m_pThis = new CLogger();
        //m_Logfile.open(m_sFileName.c_str(), ios::out | ios::app);
    }
    return m_pThis;
}

int _vscprintf (const char * format, va_list pargs)
{
    int retval;
    va_list argcopy;
    va_copy(argcopy, pargs);
    retval = vsnprintf(NULL, 0, format, argcopy);
    va_end(argcopy);
    return retval;
}

void CLogger::SetVerbose(bool mode)
{
    VERBOSE_LOGGER = mode;
}

void CLogger::Log(const char * format, ...)
{
    char* sMessage = NULL;
    int nLength = 0;
    va_list args;
    va_start(args, format);
    //  Return the number of characters in the string referenced the list of arguments.
    // _vscprintf doesn't count terminating '\0' (that's why +1)
    nLength = _vscprintf(format, args) + 1;
    sMessage = new char[nLength];
    //vsprintf_s(sMessage, nLength, format, args);
    vsprintf(sMessage, format, args);

    if(VERBOSE_LOGGER){
        //m_Logfile << Util::CurrentDateTime() << ":\t";
        std::cout << CurrentTime() << ":\t" ;
        //std::cout << sMessage << std::endl;
        //m_Logfile << sMessage << "\n";
        std::cout << sMessage << "\n";
    }

    va_end(args);

    delete [] sMessage;
}

void CLogger::Log(const string& sMessage)
{
    //m_Logfile <<  Util::CurrentDateTime() << ":\t";
    std::cout <<  CurrentTime() << ":\t"  << std::endl;
    //m_Logfile << sMessage << "\n";
    std::cout << sMessage << "\n"  << std::endl;
}

CLogger& CLogger::operator<<(const string& sMessage)
{
    //m_Logfile << "\n" << Util::CurrentDateTime() << ":\t";
    std::cout << "\n" << CurrentTime() << ":\t"  << std::endl;
    //m_Logfile << sMessage << "\n";
    std::cout << sMessage << "\n"  << std::endl;
    return *this;
}