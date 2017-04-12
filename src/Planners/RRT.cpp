#include "Planners/RRT.hpp"

namespace Antipatrea
{
    int RRT::SelectVertex(void)
    {
	ProximityQuery<int> query;

	GetCfgManager()->CopyCfg(*m_proximityAuxCfg, *m_cfgTarget);
	query.SetKey(Constants::ID_UNDEFINED);
	return m_proximityDataStructure.Neighbor(query);
    }
}
