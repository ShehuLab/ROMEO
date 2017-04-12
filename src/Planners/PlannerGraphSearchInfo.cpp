#include "Planners/PlannerGraphSearchInfo.hpp"

namespace Antipatrea
{
    
    void PlannerGraphSearchInfo::GetOutEdges(const int vid, 
					     std::vector<int> & edges,
					     std::vector<double> & costs) const
    {
	edges.clear();
	costs.clear();
	
	const PlannerVertex *v = GetPlannerGraph()->GetVertex(vid);
	
	if(v == NULL)
	    return;
	
	auto connections = v->GetConnections();
	for(auto & vidTo : *connections)
	{
	    edges.push_back(vidTo);
	    auto edge = GetPlannerGraph()->FindEdge(vid, vidTo);
	    if(edge->GetVertexId(PlannerEdge::INDEX_FROM) == vid)
		costs.push_back(edge->GetCost(PlannerEdge::INDEX_FROM));
	    else
		costs.push_back(edge->GetCost(PlannerEdge::INDEX_TO));
	}
    }
}
