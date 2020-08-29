#ifndef _KOT_PLATFORM_
#define _KOT_PLATFORM_

#ifdef _WIN32
#	define _KOT_PLATFORM_WINDOWS
#elif defined(__linux__)
#	define _KOT_PLATFORM_LINUX
#else
#	define _KOT_PLATFORM_UNKNOWN
#endif

#endif