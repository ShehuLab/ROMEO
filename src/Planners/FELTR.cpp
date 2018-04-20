#include <Planners/FELTR.hpp>
#include <Components/CfgAcceptors/CfgAcceptorBasedOnMMC.hpp>

namespace Antipatrea
{
    int FELTR::AddVertex(Cfg * const cfg)
    {
    	// set distance if the graph is not empty

        auto vid = TreeSamplingBasedPlanner::AddVertex(cfg);
	    if (vid >= 0)
	    {
			auto graph = GetPlannerGraph();
			auto newVertex = dynamic_cast<FELTRVertex*>(graph->GetVertex(vid));
			auto projector = GetCfgProjector();

			// now project this vertex into the FELTR grid
			double *projection = projector->NewValues();
			projector->Project(*cfg,projection);

			int regionID = m_energyGrid.GetCellIdFromPoint(projection);

			auto regionNode = GetEnergyRegion(projection);
			auto region = regionNode->GetKey();

			region->AddVertex(vid,projection,
								newVertex->region,
								newVertex->cell);

			projector->DeleteValues(projection);
			m_selector.Update(regionNode,region->GetWeight());
	    }
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
        	region = new FELTRRegion(regionID,30);
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

    	auto cfgAcceptor = GetCfgAcceptor();
    	auto vertex = region->SelectVertex();

    	PlannerGraph         *graph = GetPlannerGraph();
    	auto v = dynamic_cast<FELTRVertex*>(graph->GetVertex(vertex));

    	if (dynamic_cast<CfgAcceptorBasedOnMMC*>(cfgAcceptor))
	        dynamic_cast<CfgAcceptorBasedOnMMC*>(cfgAcceptor)->SetSourceCfg(v->GetCfg());

    	return(vertex);

    }
}
