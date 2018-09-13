#include "Utils/Definitions.hpp"
#include "Utils/DLHandler.hpp"

#if defined OS_WINDOWS
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif


namespace Antipatrea
{
    namespace DLHandler
    {
        void* GetSymbol(void *handle, const char name[])
        {
#if defined OS_WINDOWS
            return GetProcAddress((HMODULE) handle, name);
#else
            return dlsym(handle, (char *) name);
#endif
        }
        
        
        void* GetSymbol(const char name[])
        {

#if defined OS_WINDOWS
            return GetSymbol(NULL, name);
#else
            return GetSymbol(RTLD_DEFAULT, name);
#endif
        }
        
    }
}
