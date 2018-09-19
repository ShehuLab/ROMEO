#include <Planners/Sprint.hpp>
#include <Components/CfgAcceptors/CfgAcceptorBasedOnMMC.hpp>
#include <Components/CfgAcceptors/CfgAcceptorBasedOnFixedMMC.hpp>

#include <iostream>

namespace Antipatrea
{
  void Sprint::SetupDeltaRRegions()
  {
	  auto projector = GetCfgProjector();
	  unsigned int cellCount = (dynamic_cast<CfgProjectorDeltaR*>(projector))->GetCellCount();
	  // allocate vector all at once so pointers do not change
	  // this can be a BIG problem since selectors point to these positions
	  m_regions.resize(cellCount);

	  for (unsigned int i=0;i< cellCount;++i)
	  {
		  std::cout << "Creating region for id:" << i+1 << std::endl;
		  DeltaRRegion &r = m_regions[i];
		  r.weightId = i+1;
		  auto regionNode = m_selector.Create();
		  regionNode->SetKey(i);
		  regionNode->SetWeight(0.0);
          m_selector.Insert(regionNode);
          r.nodePtr = regionNode;

	  }
  }


int Sprint::AddVertex(Cfg * const cfg)
    {
        auto graph = GetPlannerGraph();

        auto projector = GetCfgProjector();

         // now project this vertex into the Sprint grid
        double *projection = projector->NewValues();
        projector->Project(*cfg,projection);

        unsigned int regionID = (unsigned int)projection[0];
        if (m_regions.size() == 0)
        	SetupDeltaRRegions();

        if (m_verbosityFlag == SAMPLING_PLANNER_VERBOSE_LOW)
        {
        	std::cout << "New cfg projected into region:" << regionID << std::endl;
        }

        DeltaRRegion &region = m_regions[regionID];
        auto regionNode = region.nodePtr;
        int vid = -1;
		vid = TreeSamplingBasedPlanner::AddVertex(cfg);
		if (vid >= 0)
		{
			auto newVertex = dynamic_cast<SprintVertex*>(graph->GetVertex(vid));
			region.AddVertex(vid);

			m_selector.Update(regionNode,region.GetWeight());
		}
		projector->DeleteValues(projection);
        return (vid);
    }


    int Sprint::SelectVertex(void)
    {
        auto regionNode = m_selector.Select();

        auto regionIndex = regionNode->GetKey();

        if (m_verbosityFlag == SAMPLING_PLANNER_VERBOSE_LOW)
        {
        	 std::cout << "Regionid:" << regionIndex
                      << " nbrOfConfs:" << m_regions[regionIndex].GetNrConfs()
                      << "and w:" << m_regions[regionIndex].GetWeight() << "\n";
        }

        auto cfgAcceptor = GetCfgAcceptor();

        auto vertex = m_regions[regionIndex].SelectVertex();

        PlannerGraph         *graph = GetPlannerGraph();
        auto v = dynamic_cast<SprintVertex*>(graph->GetVertex(vertex));

        if (dynamic_cast<CfgAcceptorBasedOnMMC*>(cfgAcceptor))
            dynamic_cast<CfgAcceptorBasedOnMMC*>(cfgAcceptor)->SetSourceCfg(v->GetCfg());
        if (dynamic_cast<CfgAcceptorBasedOnFixedMMC*>(cfgAcceptor))
            dynamic_cast<CfgAcceptorBasedOnFixedMMC*>(cfgAcceptor)->SetSourceCfg(v->GetCfg());

        return(vertex);
    }
}
