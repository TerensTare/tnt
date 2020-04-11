#ifndef TESTLIB_H
#define TESTLIB_H

#ifdef DLL_EXPORT
#define DLL_CODE __declspec(dllexport)

#else
#define DLL_CODE __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    DLL_CODE int __cdecl answer();

#ifdef __cplusplus
}
#endif

#endif //!TESTLIB_H