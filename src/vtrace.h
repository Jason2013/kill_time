#ifndef _MYTRACE_H_
#define _MYTRACE_H_

//s#define _DEBUG

#ifdef _DEBUG

#include <fstream>

class CMyTrace
{
public:
	CMyTrace(const char *filename, int line)
		: mFileName(filename), mLine(line)
	{}

	void operator()(const char *message, ...);
	static void LogOn();
	static void LogOff();
private:
	const char *mFileName;
	int mLine;
	//static std::ofstream m_logFile;
	static FILE * m_logFile;
	static bool m_logOn;

};


#define VTRACE_ON CMyTrace::LogOn();
#define VTRACE_OFF CMyTrace::LogOff();


#define VTRACE CMyTrace(__FILE__, __LINE__)

#else

inline void __noop(...) {}

#define VTRACE_ON ;
#define VTRACE_OFF ;
#define VTRACE __noop

#endif

#endif // _MYTRACE_H_