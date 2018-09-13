#include "Utils/Reader.hpp"
#include <fstream>
#include <string>
#include "Utils/Logger.hpp"

namespace Antipatrea
{
    std::istream& Reader::ReadDouble(std::istream & in, double & val)
    {
        std::string s;

        if((in >> s))
            val = std::stod(s);
        else
            Logger::m_out << "error Reader::ReadDouble : could not read double value" << std::endl;
        return in;
    }
    

    void Reader::ReadFromFile(const char fname[],
                              std::ios_base::openmode mode)
    {
        std::ifstream in(fname, mode);
        if(!in.is_open())
            return;
        Read(in);
        in.close();
    }
}



