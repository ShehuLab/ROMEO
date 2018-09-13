#include "Planners/RRT.hpp"

#include <iostream>

namespace Antipatrea
{
    int RRT::SelectVertex(void)
    {
		ProximityQuery<int> query;

		GetCfgManager()->CopyCfg(*m_proximityAuxCfg, *m_cfgTarget);
		query.SetKey(Constants::ID_UNDEFINED);

		//return m_proximityDataStructure.Neighbor(query);

		int q =  m_proximityDataStructure.Neighbor(query);
		if (m_verbosityFlag == SAMPLING_PLANNER_VERBOSE_FULL)
			std::cout << "return vertexId:" << q << std::endl;

		return(q);
    }
}
