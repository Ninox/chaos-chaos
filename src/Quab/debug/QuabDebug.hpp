#ifndef QUAB_DEBUG_HPP
#define QUAB_DEBUG_HPP

#ifdef QUAB_STATUS_DEBUG
#define QUAB_DEBUG(mname, v) \
	Quab::QuabDebug::debug(mname, __FILE__, __LINE__, #v, v)

#define QUAB_DEBUG_STR(mname, s) \
	Quab::QuabDebug::debugCharString(mname, __FILE__, __LINE__, #s, s)

#include <iostream>
#include <string>
#include <stdio.h>

#ifdef QUAB_OS_WIN32
#include <windows.h>
#endif

namespace Quab
{	
	// definition of the debug class
	class QuabDebug
	{
	private:
		static void _getTitle()	{
#ifdef QUAB_OS_WIN32
			WORD colorOld;
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(handle, &csbi);
			colorOld = csbi.wAttributes;
			SetConsoleTextAttribute(handle, (WORD)(0xE0));  // Yellow background and Black font color
			printf("    * DEBUG *    \n");
			SetConsoleTextAttribute(handle, colorOld);			
#else
			printf("\033[43;30m    * DEBUG *    \n\033[0m", text);
#endif
		}
		
		static void _showProperties(const char *moduleName, const char *filename, 
		                            int line, const char *vname)	{
			std::cout<<"  File Name : "<<filename<<std::endl
			         <<"Module Name : "<<moduleName<<std::endl
					 <<"       line : "<<line<<std::endl
					 <<"    variable: "<<vname<<std::endl
					 <<"       value: ";
		}
	public:
		template<class T>
		static void debug(const char *moduleName, const char *filename, 
		                  int line, const char *vname, const T &v)	{
			// print title
			QuabDebug::_getTitle();
			// print debug properties
			QuabDebug::_showProperties(moduleName, filename, line, vname);
			// print content, if you want to output the user-defined varible, please overload the operator <<
			std::cout<<v<<std::endl;
			std::cout<<std::endl;
		}
		
		static void debugCharString(const char *moduleName, const char *filename, 
		                            int line, const char *vname, const char *v)	
		{
			// print title & properties
			_getTitle();
			QuabDebug::_showProperties(moduleName, filename, line, vname);			
			// print content
			printf("'%s'\n", v);
		}
	};
}

#else
#define QUAB_DEBUG(name, obj)
#define QUAB_DEBUG_STRING(name, sobj)	
#endif

#endif