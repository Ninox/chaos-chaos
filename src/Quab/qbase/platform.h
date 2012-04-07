#ifndef PLATFORM_H
#define PLATFORM_H

// ���ֲ���ͬ�ı���ƽ̨, ����������windows��Linuxƽ̨�Լ�msvc, mingw, gcc������
#ifdef WIN32
    #define QUAB_OS_WIN32
    #ifdef  MSVC
        #define QUAB_CL_MSVC
    #else
        #define QUAB_CL_MINGW
    #endif
#else
    #ifdef LINUX
        #define QUAB_OS_LINUX
        #define QUAB_CL_GCC
    #else
        #define QUAB_OS_OTHER
    #endif
#endif

#endif // PLATFORM.h
