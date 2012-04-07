#ifndef PLATFORM_H
#define PLATFORM_H

// 区分不不同的编译平台, 包括有区分windows和Linux平台以及msvc, mingw, gcc编译器
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
