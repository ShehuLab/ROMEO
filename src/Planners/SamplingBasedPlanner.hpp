#ifndef Antipatrea__SamplingBasedPlanner_HPP_
#define Antipatrea__SamplingBasedPlanner_HPP_

#include "Planners/Planner.hpp"
#include "Planners/PlannerGraph.hpp"
#include "Planners/PlannerGraphSearchInfo.hpp"
#include "Components/CfgSamplers/CfgSampler.hpp"
#include "Components/CfgImprovers/CfgImprover.hpp"
#include "Components/CfgManagers/CfgManager.hpp"
#include "Components/CfgDistances/CfgDistance.hpp"
#include "Components/CfgAcceptors/CfgAcceptor.hpp"
#include "Components/CfgOffspringGenerators/CfgOffspringGenerator.hpp"
#include "Components/EdgeCostEvaluators/EdgeCostEvaluator.hpp"
#include "Utils/ProximityDefault.hpp"
#include "Utils/GraphSearch.hpp"
#include "Setup/Defaults.hpp"
#include <cmath>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface for a sampling-based planner which explores the configuration space
     *       by sampling acceptable configurations, expanding branches, and 
     *       connecting neighboring configurations.
     * 
     *@remarks
     * - A sampling-based planner constructs a planner graph.
     * - A sampling-based planner could be roadmap- or tree-based.
     * - A sampling-based planner often relies on a proximity data structure to compute nearest neighbors.
     * - This class requires access to several components such as 
     *   PlannerGraph, CfgSampler, CfgImprover, CfgManager, CfgDistance,
     *   CfgAcceptor, CfgOffspringGenerator, and EdgeCostEvaluator.
     */
    class SamplingBasedPlanner : public Planner,
				 public PlannerGraphContainer,
				 public CfgSamplerContainer,
				 public CfgImproverContainer,
				 public CfgManagerContainer,
				 public CfgDistanceContainer,
				 public CfgAcceptorContainer,
				 public CfgOffspringGeneratorContainer,
				 public EdgeCostEvaluatorContainer
    {
    public:
	SamplingBasedPlanner(void) : Planner(),
				     PlannerGraphContainer(),
				     CfgSamplerContainer(),
				     CfgImproverContainer(),
				     CfgManagerContainer(),
				     CfgDistanceContainer(),
				     CfgAcceptorContainer(),
				     CfgOffspringGeneratorContainer(),
				     EdgeCostEvaluatorContainer(),
				     m_vidInit(Constants::ID_UNDEFINED),
				     m_oneStepDistance(Constants::VAL_SamplingBasedPlanner_OneStepDistance),
				     m_proximityAuxCfg(NULL)
	{
	    m_proximityDataStructure.m_distFn     = ProximityDistFn;
	    m_proximityDataStructure.m_distFnData = this;
	}

	virtual ~SamplingBasedPlanner(void)
	{
	    if(m_proximityAuxCfg)
		delete m_proximityAuxCfg;
	}

	virtual bool CheckSetup(void)
	{
	    return
		Planner::CheckSetup() &&
		GetPlannerGraph() != NULL &&
		GetCfgSampler() != NULL &&
		GetCfgImprover() != NULL &&
		GetCfgManager() != NULL &&
		GetCfgDistance() != NULL &&
		GetCfgAcceptor() != NULL &&
		GetCfgOffspringGenerator() != NULL &&
		GetEdgeCostEvaluator() != NULL &&
		GetPlannerGraph()->CheckSetup() &&
		GetCfgSampler()->CheckSetup() &&
		GetCfgImprover()->CheckSetup() &&
		GetCfgManager()->CheckSetup() &&
		GetCfgDistance()->CheckSetup() &&
		GetCfgAcceptor()->CheckSetup() &&
		GetCfgOffspringGenerator()->CheckSetup() &&
		GetEdgeCostEvaluator()->CheckSetup();
	}
	
	virtual void Info(const char prefix[]) const
	{
	    Planner::Info(prefix);
	    Logger::m_out << prefix << " PlannerGraph      = " << Name(GetPlannerGraph()) << std::endl
			  << prefix << " CfgSampler        = " << Name(GetCfgSampler()) << std::endl
			  << prefix << " CfgImprover       = " << Name(GetCfgImprover()) << std::endl
			  << prefix << " CfgDistance       = " << Name(GetCfgDistance()) << std::endl
			  << prefix << " CfgAcceptor       = " << Name(GetCfgAcceptor()) << std::endl
			  << prefix << " CfgOffspringGenerator = " << Name(GetCfgOffspringGenerator()) << std::endl
			  << prefix << " EdgeCostEvaluator = " << Name(GetEdgeCostEvaluator()) << std::endl
			  << prefix << " OneStepDistance   = " << GetOneStepDistance() << std::endl;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the planner from the given parameters.
	 * 
	 *@remarks
	 * - Function first invokes Planner::SetupFromParams(params).
	 * - It then sets the one-step distance (keyword Constants::KW_OneStepDistance), which
	 *   is used by sampling-based planners to determine the resolution at which to discretize
	 *   paths (e.g., PRM-like planners) or to determine the step when expanding a branch (e.g., tree-based planners).
	 * - It uses the parameter group associated with the keyword Constants::KW_SamplingBasedPlanner.
	 * - The parameter value can be specified in a text file as, for example,
	 *     <center><tt>SamplingBasedPlanner { OneStepDistance 0.01 }</tt></center>
	 */
	virtual void SetupFromParams(Params & params)
	{
	    Planner::SetupFromParams(params);
	    
	    auto data = params.GetData(Constants::KW_SamplingBasedPlanner);
	    if(data && data->m_params)
	    {
		SetOneStepDistance(data->m_params->GetValueAsDouble(Constants::KW_OneStepDistance, GetOneStepDistance()));
	    }
	    
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the one-step distance.
	 *
	 *@remarks
	 * - This is used by sampling-based planners to determine the resolution at which to discretize
	 *   paths (e.g., PRM-like planners) or to determine the step when expanding a branch (e.g., tree-based planners).
	 */
	virtual double GetOneStepDistance(void) const
	{
	    return m_oneStepDistance;
	}
		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the one-step distance.
	 *
	 *@remarks
	 * - This is used by sampling-based planners to determine the resolution at which to discretize
	 *   paths (e.g., PRM-like planners) or to determine the step when expanding a branch (e.g., tree-based planners).
	 */
	virtual void SetOneStepDistance(const double d)
	{
	    m_oneStepDistance = d;
	}

	
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Initialize the planner.
	 *
	 *@remarks
	 * - This function should be called only once at the beginning
	 *   after the problem and other components have been setup.
	 * - The function allocates memory for an auxilary configuration 
	 *   needed to compute nearest neighbors.
	 */
	virtual void Start(void);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Return true iff the planner has found a solution.
	 *
	 *@remarks
	 * - The function uses the disjoint-set data structure to check whether any vertex that is marked as goal
	 *   belongs to the same connected component as the initial vertex.
	 */
	virtual bool IsSolved(void);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Construct the solution that the planner has found.
	 *
	 *@remarks
	 * - The function returns true iff the planner has found a solution.
	 * - If a solution is found, A* is used to find the shortest path in the planner graph.
	 *   The solution is constructed based on the vertex ids of this shortest path
	 *   and the corresponding configurations. 
	 */	
	virtual bool GetSolution(PlannerSolution & sol);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Print the configurations, vertices, and edges of the planner graph.
	 *
	 *@remarks
	 *  - This function can be used to save the planner graph to an output stream for later use.
	 */
	virtual std::ostream& Print(std::ostream & out) const;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Read configurations, vertices, and edges and add them to the planner graph.
	 *
	 *@remarks
	 * - This allows the user to reuse a previously-stored planner graph or to 
	 *   populate the planner graph with additinal configurations and edges that may
	 *   come from other sources, e.g., databases of known good configurations/structures.
	 * - The function avoids adding configurations or edges that already exist in the planner graph
	 *   (using FindCfg and PlannerGraph::FindEdge).
	 */
	virtual std::istream& Read(std::istream & in);

    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief If there is a vertex whose configuration is the same as <tt>cfg</tt>, then return its id.
	 *
	 *@remarks
	 * - The function finds the closest vertex to <tt>cfg</tt> that is within Constants::EPSILON distance:
	 *    - If successful, it returns the id of that vertex.
	 *    - If there is no configuration in the planner graph that is within Constants::EPSILON distance
	 *      from <tt>cfg</tt>, then the function returns Constants::ID_UNDEFINED. 
	 * - If the planner uses the proximity data structure, then the function uses it to find the configuration.
	 *   Otherwise, it does a brute-force search.
	 */
	virtual int FindCfg(Cfg & cfg);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Add a new vertex to the graph with <tt>cfg</tt> as its associated configuration.
	 *
	 *@remarks
	 * - The function does not make a copy of <tt>cfg</tt>; it simply does pointer assignment.
	 * - The function does not check if <tt>cfg</tt> is already in the graph. It is the 
	 *   responsibility of the calling function to perform such a check (using FindCfg)
	 *   if indeed there is a possibility that the same configuration is added to the 
	 *   graph multiple times (which even if it happened would not cause any errors).
	 */
	virtual int AddVertex(Cfg * const cfg);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Factory for creating new planner vertices.
	 *
	 *@remarks
	 * - Planners should use NewVertex instead of <tt>new PlannerVertex()</tt> to 
	 *   allocate memory for a new vertex, since planners extending SamplingBasedPlanner could
	 *   also extend PlannerVertex. For example, EST does exactly this. 
	 */
	virtual PlannerVertex* NewVertex(void) const
	{
	    return new PlannerVertex();
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Factory for creating new planner edges.
	 *
	 *@remarks
	 * - Planners should use NewEdge instead of <tt>new PlannerEdge()</tt> to 
	 *   allocate memory for a new edge, since planners extending SamplingBasedPlanner could
	 *   also extend PlannerEdge. 
	 */
	virtual PlannerEdge* NewEdge(void) const
	{
	    return new PlannerEdge();
	}
		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Construct the sequence of configurations based on the sequence of vertices
	 *       associated with the planner solution.
	 *
	 *@remarks
	 *  - Additional intermediate configurations could be added to the solution
	 *    since the paths associated with the edges of the solution could be discretized
	 *    (using OneStepDistance to define the resolution).
	 */
	virtual void FromVertexSequenceToCfgs(PlannerSolution & sol);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Wrapper for the distance function used by the proximity data structure --
	 *       it uses CfgDistance.
	 */
	static double ProximityDistFn(const int vid1, const int vid2, SamplingBasedPlanner * sbPlanner);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Proximity data structure to facilitate nearest-neighbors queires.
	 */	
	ProximityDefault<int, SamplingBasedPlanner*>  m_proximityDataStructure;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Auxilary configuration used by the proximity data structure to 
	 *       compute nearest-neighbors queries.
	 */
	Cfg *m_proximityAuxCfg;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief The id of the vertex associated with the initial configuration.
	 */
	int m_vidInit;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief The set of vertices marked as goal (where the goal is satisfied).
	 */
	std::vector<int> m_vidsGoal;
		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Graph-searching algorithms, e.g., DFS, BFS, A*, Dijkstra.
	 */
	GraphSearch<int> m_graphSearch;
		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Implicit representation of the planner graph.
	 */
	PlannerGraphSearchInfo m_graphSearchInfo;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Resolution at which to discretize  paths (e.g., PRM-like planners) or to 
	 *       determine the step when expanding a branch (e.g., tree-based planners).
	 */
	double m_oneStepDistance;

    };
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to SamplingBasedPlanner.
     */
    ClassContainer(SamplingBasedPlanner, m_samplingBasedPlanner);
    
}

#endif
