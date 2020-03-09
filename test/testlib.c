#define DLL_EXPORT
#include "testlib.h"

#ifdef __cplusplus
extern "C"
{
#endif

    DLL_CODE int answer()
    {
        return 43;
    }

#ifdef __cplusplus
}
#endif