#include "Planners/PGT.hpp"
#include "Setup/Defaults.hpp"
#include "Utils/Logger.hpp"
#include <string>

namespace Antipatrea
{
    PGT::~PGT(void)
    {
        for(auto & cell : m_cells)
            if(cell.second)
                delete cell.second;
    }

    void PGT::SetupFromParams(Params & params)
    {
        TreeSamplingBasedPlanner::SetupFromParams(params);

        auto data = params.GetData(Constants::KW_PGT);

        if(data && data->m_params)
        {
            auto gdata = data->m_params->GetData(Constants::KW_Grid);
            if(gdata && gdata->m_params)
                m_grid.SetupFromParams(*(gdata->m_params));
        }
    }
    

    int PGT::AddVertex(Cfg * const cfg)
    {
        const int vid = TreeSamplingBasedPlanner::AddVertex(cfg);
        if(vid >= 0)
        {
            auto    cfgProjector = GetCfgProjector();
            double *cfgProj      = cfgProjector->NewValues();

            cfgProjector->Project(*cfg, cfgProj);

            Cell     *cell;
            const int cid = m_grid.GetCellId(cfgProj);
            auto      curr= m_cells.find(cid);

            if(curr == m_cells.end())
            {
                cell = NewCell();
                m_cells.insert(std::make_pair(cid, cell));
            }
            else
                cell = curr->second;
            
            m_totalWeight -= cell->GetWeight();
            cell->AddVertex(vid);
            m_totalWeight += cell->GetWeight();

            cfgProjector->DeleteValues(cfgProj);
            
        }
        return vid;
    }

    int PGT::SelectVertex(void) 
    {
        const double  r     = RandomUniformReal(0, m_totalWeight);
        double        w     = 0.0;

        for(auto & cell : m_cells)
        {
            w += cell.second->GetWeight();
            if(w >= r)
                return cell.second->SelectVertex();
        }
        
        return RandomUniformInteger(0, GetPlannerGraph()->GetNrVertices() - 1);
    }
}
