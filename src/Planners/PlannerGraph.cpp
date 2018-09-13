#include "Planners/PlannerGraph.hpp"

namespace Antipatrea
{
    PlannerGraph::~PlannerGraph(void)
    {
        for(auto & v : m_vertices)
            if(v)
                delete v;
        for(auto & iter : m_edges)
            if(iter.second)
                delete iter.second;
    }
    
    void PlannerGraph::AddEdge(PlannerEdge * const edge)
    {
        const int      vidFrom = edge->GetVertexId(PlannerEdge::INDEX_FROM);
        const int      vidTo   = edge->GetVertexId(PlannerEdge::INDEX_TO);
        PlannerVertex *vfrom   = GetVertex(vidFrom);
        PlannerVertex *vto     = GetVertex(vidTo);
        
        m_edges.insert(std::make_pair(GetEdgeKey(vidFrom, vidTo), edge));

        vfrom->GetConnections()->insert(vidTo);
        vto->GetConnections()->insert(vidFrom);
        
        m_components.Join(vfrom->GetDisjointSetElem(), vto->GetDisjointSetElem());
    }
}
