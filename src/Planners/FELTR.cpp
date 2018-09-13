#include <Planners/FELTR.hpp>
#include <Components/CfgAcceptors/CfgAcceptorBasedOnMMC.hpp>
#include <Components/CfgAcceptors/CfgAcceptorBasedOnFixedMMC.hpp>

#include <iostream>

namespace Antipatrea
{
    // To implmenent granularity reduction, we first check to see
    // if we already have a conformation in the energy/USR projection
    // that is "close" to cfg.  If we do, we skip adding cfg.
    int FELTR::AddVertex(Cfg * const cfg)
    {
        auto graph = GetPlannerGraph();

        auto projector = GetCfgProjector();

         // now project this vertex into the FELTR grid
        double *projection = projector->NewValues();
        projector->Project(*cfg,projection);

        int regionID = m_energyGrid.GetCellIdFromPoint(projection);

        auto regionNode = GetEnergyRegion(projection);
        auto region = regionNode->GetKey();
        int vid = -1;
        if (region->CheckVertex(cfg,projection))
        {
            vid = TreeSamplingBasedPlanner::AddVertex(cfg);
            if (vid >= 0)
            {
                auto newVertex = dynamic_cast<FELTRVertex*>(graph->GetVertex(vid));
                region->AddVertex(vid,cfg,projection,
                                  newVertex->region,
                                  newVertex->cell);

                projector->DeleteValues(projection);
                m_selector.Update(regionNode,region->GetWeight());
            }
        }
        return (vid);
    }

    Selector<FELTRRegion *>::Node * FELTR::GetEnergyRegion(double projection[])
    {
        int regionID = m_energyGrid.GetCellIdFromPoint(projection);
        // is this a new region?
        auto iter = m_FELTRRegionToSelectorMap.find(regionID);

        FELTRRegion * region;

        Selector<FELTRRegion *>::Node *regionNode;

        const bool newRegion = (iter == m_FELTRRegionToSelectorMap.end());
        if (newRegion)
        {
        	/*
				//DEBUG
				std::cout << "Creating FELTR new region, projection coordinates are:\n";
				for (unsigned int i(0);i < 4;++i)  std::cout << "coord " << i << ":" << projection[i] << std::endl;
				// END DEBUG
             */

            int cfgDims = GetCfgManager()->GetDim();
            if (cfgDims %3 != 0)
            {
                exit(115);
            }
            cfgDims = cfgDims/3;

            region = new FELTRRegion(regionID,m_cellGridGranularity,cfgDims);
            region->SetCfgDistance(GetCfgDistance());

            regionNode = m_selector.Create();
            regionNode->SetKey(region);
            regionNode->SetWeight(1);
            m_selector.Insert(regionNode);
            m_FELTRRegionToSelectorMap.insert(std::make_pair(regionID,regionNode));
        }
        else
        {
            regionNode = iter->second;
        }
        return(regionNode);
    }

    int FELTR::SelectVertex(void)
    {
        auto regionNode = m_selector.Select();

        auto region = regionNode->GetKey();

        if (m_verbosityFlag == SAMPLING_PLANNER_VERBOSE_LOW)
        {
        	std::cout << "Regionid:" << region->GetId() << " selected with avgEnergy:"
                      << region->GetAvgE() << " nbrOfConfs:" << region->GetNrConfs()
                      << "and w:" << region->GetWeight() << "\n";
        }

        auto cfgAcceptor = GetCfgAcceptor();

        auto vertex = region->SelectVertex();

        PlannerGraph         *graph = GetPlannerGraph();
        auto v = dynamic_cast<FELTRVertex*>(graph->GetVertex(vertex));

        if (dynamic_cast<CfgAcceptorBasedOnMMC*>(cfgAcceptor))
            dynamic_cast<CfgAcceptorBasedOnMMC*>(cfgAcceptor)->SetSourceCfg(v->GetCfg());
        if (dynamic_cast<CfgAcceptorBasedOnFixedMMC*>(cfgAcceptor))
            dynamic_cast<CfgAcceptorBasedOnFixedMMC*>(cfgAcceptor)->SetSourceCfg(v->GetCfg());

        return(vertex);
    }
}
