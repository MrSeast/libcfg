#ifndef __libcfg_compat_h__
#define __libcfg_compat_h__


#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifndef LIBCFG_STATIC
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return(TRUE);
}
#endif /* LIBCFG_STATIC */

#ifndef snprintf
#define snprintf	_snprintf
#endif

#ifndef atoll
#define atoll		_atoi64
#endif

#endif /* WIN32 */


#endif /* __libcfg_compat_h__ */
