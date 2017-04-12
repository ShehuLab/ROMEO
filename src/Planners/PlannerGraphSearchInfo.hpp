#ifndef Antipatrea___PlannerGraphSearchInfo_HPP_
#define Antipatrea___PlannerGraphSearchInfo_HPP_

#include "Planners/PlannerGraph.hpp"
#include "Utils/GraphSearch.hpp"
#include "Utils/Constants.hpp"

namespace Antipatrea
{	    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Wrapper for the planner graph so that it can be used with
     *       the graph searching alogrithms, e.g., DFS, BFS, A*, Dijkstra.
     * 
     *@remarks
     * - The graph-search algorithms in this package work with
     *   general, implicit, graph representation.
     * - This wrapper takes a PlanerGraph and implements the functions
     *   needed by the graph-search algorithms, namely GetOutEdges and IsGoal.
     */
    class PlannerGraphSearchInfo : public GraphSearchInfo<int>,
				   public PlannerGraphContainer
    {
    public:
	PlannerGraphSearchInfo(void) :
	    GraphSearchInfo<int>(),
	    PlannerGraphContainer()
	{
	}

	virtual ~PlannerGraphSearchInfo(void)
	{
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the outgoing edges from vertex <tt>vid</tt> as well as their costs.
	 */
	virtual void GetOutEdges(const int vid, 
				 std::vector<int> & edges,
				 std::vector<double> & costs) const;

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Return true iff the vertex <tt>vid</tt> is marked as a goal vertex.
	 */
	virtual bool IsGoal(const int vid) const
	{
	    return GetPlannerGraph()->GetVertex(vid)->IsGoal();
	}
    };
}

#endif
