#include "Planners/PlannerSolution.hpp"

namespace Antipatrea
{            
    std::ostream& PlannerSolution::Print(std::ostream & out) const
    {
        auto cfgManager = GetCfgManager();

        out << GetCost() << std::endl;
        out << m_vids.size() << std::endl;
        for(auto & vid : m_vids)
            out << vid << " ";
        out << std::endl;
        out << GetNrCfgs() << std::endl;
        for(auto & cfg : m_cfgs)
            cfgManager->PrintCfg(out, *cfg);
           return out;
        
    }
    
    std::istream& PlannerSolution::Read(std::istream & in)
    {
        Clear();
        
        auto cfgManager = GetCfgManager();

        Cfg   *cfg;
        double cost;
        int    vid;
        int    n;
        
        if(!(in >> cost))
        {
            Logger::m_out << " error PlannerSolution::Read : expecting cost value" << std::endl;
            return in;
        }
        SetCost(cost);

        if(!(in >> n))
        {
            Logger::m_out << " error PlannerSolution::Read : expecting number of vertices in sequence" << std::endl;
            return in;
        }

        for(int i = 0; i < n; ++i)
            if((in >> vid))
                m_vids.push_back(vid);
            else
            {
                Logger::m_out << " error PlannerSolution::Read : expecting vid " << i << " out of " << n << " in vertex sequence " << std::endl;
                return in;
            }

        if(!(in >> n))
        {
            Logger::m_out << " error PlannerSolution::Read : expecting number of cfgs" << std::endl;
            return in;
        }


        for(int i = 0; i < n; ++i)
        {
            cfg = cfgManager->NewCfg();
            cfgManager->ReadCfg(in, *cfg);
            if(!in.good())
            {
                cfgManager->DeleteCfg(cfg);
                Logger::m_out << " error PlannerSolution::Read : expecting cfg " << i << " out of " << n << std::endl;
                return in;
            }
            AddCfg(cfg);
        }
        
        return in;
        
    }
    

}
