#include "Utils/Logger.hpp"
#include <iostream>

namespace Antipatrea
{
    
    int Logger::m_verbosity = 1;

    std::ostream& Logger::m_out = std::cout;
    
}
