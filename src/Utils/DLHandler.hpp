#ifndef Antipatrea__DLHandler_HPP_
#define Antipatrea__DLHandler_HPP_

namespace Antipatrea
{
    namespace DLHandler
    {
	void* GetSymbol(void *handle, const char name[]);
	void* GetSymbol(const char name[]);
    }
}

#endif
