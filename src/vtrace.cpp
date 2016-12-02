//#include "stdafx.h"
#include "vtrace.h"

#ifdef _DEBUG

#include <crtdbg.h>

#include <cstdio>
#include <cstring>
#include <cstdarg>

#include <iostream>
#include <fstream>
using namespace std;

static char trace_temp[1024];

void CMyTrace:: operator ()(const char *message, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, message);
	vsprintf(trace_temp, message, arg_ptr);

	//strcat(trace_temp, "\n");

	trace_temp[sizeof(trace_temp)-1] = '\0';
	_CrtDbgReport(_CRT_WARN, mFileName, mLine, NULL, trace_temp);
	if (m_logOn)
	{
		//m_logFile << trace_temp << std::endl;
		fprintf(m_logFile, "%s", trace_temp);
		//fflush(m_logFile);
	}
	trace_temp[0] = '\0';
}

bool CMyTrace::m_logOn = false;
FILE * CMyTrace::m_logFile = NULL;

void CMyTrace::LogOn()
{
	m_logFile = fopen("log.txt", "a");
	m_logOn = (bool)(m_logFile != NULL);

	//m_logFile.open("log.txt", std::ios::app);
	//m_logOn = true;
}
void CMyTrace::LogOff()
{
	//m_logFile.close();
	if (m_logFile != NULL)
		fclose(m_logFile);
	m_logOn = false;
}

#endif
