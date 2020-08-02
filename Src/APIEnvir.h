/**
 * @file Contains the includes and definitions necessary for the Add-On to
 *       function.
 */

#ifndef	_APIENVIR_H_
#define	_APIENVIR_H_


#if defined (_MSC_VER)
	#if !defined (WINDOWS)
		#define WINDOWS
	#endif
#endif

#if defined (WINDOWS)
	#include "Win32Interface.hpp"
	#pragma warning (disable: 4068)
#endif

#if defined (macintosh)
	#include <CoreServices/CoreServices.h>
#endif

#if !defined (ACExtension)
	#define ACExtension
#endif

#include	"ACAPinc.h"
#include    "Location.hpp"

#endif
