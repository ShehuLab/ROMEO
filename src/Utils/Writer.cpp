#include "Utils/Writer.hpp"
#include "Utils/Misc.hpp"
#include <fstream>

namespace Antipatrea
{
    void Writer::PrintToFile(const char fname[],
			     std::ios_base::openmode mode) const
    {
	std::ofstream out(fname, mode);
	if(!out.is_open())
	    return;
	OutputFormat(out);
	Print(out);
	out.close();
    }
}



