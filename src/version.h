#ifndef VERSION_H
#define VERSION_H
// Currently, this file is only included by graphics.cpp

/* 20150203 by cyd AT bupt dot edu dot cn 
VC内部版本号    宏的值           VC发行版编号
MS VC++ 13.0 _MSC_VER = 1900 (Visual C++ 2015)
MS VC++ 12.0 _MSC_VER = 1800 (Visual C++ 2013)
MS VC++ 11.0 _MSC_VER = 1700 (Visual C++ 2012)
MS VC++ 10.0 _MSC_VER = 1600 (Visual C++ 2010)
MS VC++ 9.0  _MSC_VER = 1500 (Visual C++ 2008)
MS VC++ 8.0  _MSC_VER = 1400 (Visual C++ 2005)
MS VC++ 7.1  _MSC_VER = 1310 (Visual C++ 7.1)
MS VC++ 7.0  _MSC_VER = 1300 (Visual C++ 7.0)
MS VC++ 6.0  _MSC_VER = 1200 (Visual C++ 6.0)
MS VC++ 5.0  _MSC_VER = 1100 (Visual C++ 5.0)
*/
//编译器版本，目前仅支持vc6/vc2008/vc2010/vc2012/mingw
//cyd@bupt, 20150207 显然要考虑vc2013/vc2015等等编译器
#ifdef _WIN64
#define SYSBITS TEXT("x64")
#else
#define SYSBITS TEXT("x86")
#endif

#ifdef _MSC_VER
	#if (_MSC_VER > 1900)
		#define COMPILER_VER TEXT("VC20XX") SYSBITS
	#elif (_MSC_VER >= 1900)
		#define COMPILER_VER TEXT("VC2015") SYSBITS
	#elif (_MSC_VER >= 1800)
		#define COMPILER_VER TEXT("VC2013") SYSBITS
	#elif (_MSC_VER >= 1700)
		#define COMPILER_VER TEXT("VC2012") SYSBITS
	#elif (_MSC_VER >= 1600)
		#define COMPILER_VER TEXT("VC2010") SYSBITS
	#elif (_MSC_VER > 1500)
		#define COMPILER_VER TEXT("VC2008") SYSBITS
	#else
		#define COMPILER_VER TEXT("VisualC++") SYSBITS
	#endif
#else
	#define TOSTRING_(x) #x
	#define TOSTRING(x) TOSTRING_(x)
	#define GCC_VER TEXT(TOSTRING(__GNUC__)) TEXT(".") TEXT(TOSTRING(__GNUC_MINOR__))
	#define COMPILER_VER TEXT("GCC") GCC_VER SYSBITS
#endif

//#define EGE_TITLE TEXT("EGE13.04 ") COMPILER_VER
// cyd@bupt 20150207 We increase the lib version
#define EGE_TITLE TEXT("EGE13.04.02 ") COMPILER_VER

#endif //end #ifndef VERSION_H