#include "Components/CfgManagers/CfgManager.hpp"
#include "Utils/Reader.hpp"
#include <cmath>

namespace Antipatrea
{
    double Cfg::ENERGY_UNDEFINED = INFINITY;

    std::ostream& CfgManager::PrintCfg(std::ostream & out, const Cfg & cfg) const
    {        
        const int     n    = GetDim();
        const double *vals = cfg.GetValues();
        
        for(int i = 0; i < n; ++i)
            out <<  vals[i] << " ";
        out << std::endl;
        out << cfg.GetEnergy() << std::endl;
        
        return out;
    }

    std::istream& CfgManager::ReadCfg(std::istream & in, Cfg & cfg) const
    {
        const int n    = GetDim();
        double   *vals = cfg.GetValues();
        double    energy;
        
        for(int i = 0; i < n; ++i)
        {
            if(!(in >> vals[i]))
            {
                Logger::m_out << "CfgStreamer::Read error: failed to read value " << i << " out of " << n << " values" << std::endl;
                return in;
            }
        }
        
        Reader::ReadDouble(in, energy);
        if(!in.good())
        {
            Logger::m_out << "CfgStreamer::Read error: failed to read energy value" << std::endl;
            return in;
        }
        
        cfg.SetValues(vals);
        if(energy == INFINITY)
            cfg.SetEnergy(Cfg::ENERGY_UNDEFINED);
        else
            cfg.SetEnergy(energy);

        return in;
    }
}
