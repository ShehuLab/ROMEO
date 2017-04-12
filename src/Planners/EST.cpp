#include "Planners/EST.hpp"

namespace Antipatrea
{
    int EST::AddVertex(Cfg * const cfg)
    {
	const int vid = TreeSamplingBasedPlanner::AddVertex(cfg);
	if(vid >= 0)
	{
	    ProximityQuery<int>   query;
	    ProximityResults<int> results;
	    PlannerGraph         *graph = GetPlannerGraph();
	    ESTVertex            *v;
	    
	    query.SetKey(vid);
	    query.SetRange(GetNeighborhoodRadius());
	    query.SetNrNeighbors(graph->GetNrVertices());
	    m_proximityDataStructure.Neighbors(query, results);

	    const int n = results.GetNrResults();
	    for(int i = 0; i < n; ++i)
		if(results.GetKey(i) != vid)
		{
		    v = dynamic_cast<ESTVertex*>(graph->GetVertex(results.GetKey(i)));
		    m_totalWeight -= GetVertexWeight(*v);
		    v->SetNrNeighbors(1 + v->GetNrNeighbors());
		    m_totalWeight += GetVertexWeight(*v);
		}
	    v = dynamic_cast<ESTVertex*>(graph->GetVertex(vid));
	    v->SetNrNeighbors(n);
	    m_totalWeight += GetVertexWeight(*v);
	}
	return vid;
    }

    double EST::GetVertexWeight(const ESTVertex & v) const
    {
	return 1.0 / (1.0 + v.GetNrSelections() + v.GetNrNeighbors());
    }
    
    int EST::SelectVertex(void) 
    {
	ESTVertex *v;
	
	const double  r     = RandomUniformReal(0, m_totalWeight);
        PlannerGraph *graph = GetPlannerGraph();
	double        w     = 0.0;

	for(int i = graph->GetNrVertices() - 1; i >= 0; --i)
	{
	    v = dynamic_cast<ESTVertex*>(graph->GetVertex(i));
	    w += GetVertexWeight(*v);
	    if(w >= r)
	    {
		v->SetNrSelections(1 + v->GetNrSelections());
		return i;
	    }
	}

	v = dynamic_cast<ESTVertex*>(graph->GetVertex(0));
	v->SetNrSelections(1 + v->GetNrSelections());
	
	return 0;
    }
}
