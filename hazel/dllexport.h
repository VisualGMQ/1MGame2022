#ifndef __HAZEL_DLLEXPORT_H__
#define __HAZEL_DLLEXPORT_H__

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#endif
