#ifndef Antipatrea__PRM_HPP_
#define Antipatrea__PRM_HPP_

#include "Planners/SamplingBasedPlanner.hpp"
#include "Components/CfgOffspringGenerators/CfgOffspringGeneratorToTarget.hpp"
#include "Setup/Defaults.hpp"
#include <unordered_set>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Probabilistic RoadMap Method (PRM).
     * 
     *@remarks
     * - PRM requires access to an exact path generator (CfgOffspringGeneratorToTarget)
     *   to connect neighboring configurations and to PathAcceptor to determine
     *   whether those connections are valid.
     * - PRM starts by adding the initial and goal configuration(s) to the planner graph.
     * - The planner graph is further populated by generating acceptable configurations
     *   and adding those as vertices to the planner graph. The component CfgValidSampler
     *   is used to generate such configurations.
     * - To capture the connectivity of the configuration space, attempts are made to
     *   connect neighboring configurations. A proximity data structure is used to
     *   compute several neighbors for each vertex according to the distance metric defined by
     *   CfgDistance. For each vertex pair (A, B), PathAcceptor and CfgOffspringGenerator are used to determine
     *   if the pair are connected with a valid path. If so, EdgeCostEvaluator is used to compute the
     *   cost of the edge (A, B) as well as of its reverse edge (B, A). Both edges, (A, B) and (B, A),
     *   are then added to the planner graph.
     * - This process of populating the planner graph and connecting neighboring vertices is repeated until
     *   a solution is found (the initial vertex and a goal vertex belong to the same connected component)
     *   or a runtime limit is reached.
     * - A parameter, m_batchSizeToGenerateVertices, governs how many (additional) vertices are generated 
     *   before making (additional) attempts to connect neighboring configurations.
     * - Another parameter, m_nrNeighbors, determines how many neighbors to compute for each vertex.
     * - A third parameter, m_probAllowCycles, determines whether or not cycles should be allowed during
     *   the construction of the planner graph. When set to 0, cycles are not allowed. When set to 1, cycles
     *   are always allowed. For any other value, if a candidate edge (A, B) would result in a cycle and
     *   RandomUniformReal(0, 1) <= m_probAllowCycles, then the candidate edge (A, B) is ignored.
     *    - Note that cycles are only useful when interested in selecting the best among many
     *	    solutions that could be computed by PRM.
     *    - If, however, the user is interested in just finding any solution, as quickly as possible,
     *      then cycles should not be allowed as they only increase the runtime
     *      (since PathAcceptor would be called on many more edges).
     */  
    class PRM : public SamplingBasedPlanner
    {
    public:
	PRM(void) : SamplingBasedPlanner(),
		    m_nrNeighbors(Constants::VAL_PRM_NrNeighbors),
		    m_batchSizeToGenerateVertices(Constants::VAL_PRM_BatchSizeToGenerateVertices),
		    m_probAllowCycles(Constants::VAL_PRM_ProbAllowCycles),
		    m_nrRemainingToCompleteBatch(0)
	{
	}
	
	virtual ~PRM(void)
	{
	}

	
	virtual void Info(const char prefix[]) const
	{
	    SamplingBasedPlanner::Info(prefix);
	    Logger::m_out << prefix << " BatchSizeToGenerateVertices = " << GetBatchSizeToGenerateVertices() << std::endl
		          << prefix << " NrNeighbors                 = " << GetNrNeighbors() << std::endl
		          << prefix << " ProbAllowCycles             = " << GetProbAllowCycles() << std::endl;
	}
	

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the planner from the given parameters.
	 * 
	 *@remarks
	 * - Function first invokes SamplingBasedPlanner::SetupFromParams(params).
	 * - It then sets the number of neighbors, batch size, and probability to allow cycles 
	 *   (keywords Constants::KW_NrNeighbors, Constants::KW_BatchSizeToGenerateVertices, Constants::KW_ProbAllowCycles).
	 * - It uses the parameter group associated with the keyword Constants::KW_PEM.
	 * - The parameter value can be specified in a text file as, for example,
	 *     <center><tt>PRM { NrNeighbors 10 BatchSizeToGenerateVertices 100 ProbAllowCycles 0.0 }</tt></center>
	 */
	virtual void SetupFromParams(Params & params)
	{
	    SamplingBasedPlanner::SetupFromParams(params);
	    
	    auto data = params.GetData(Constants::KW_PRM);
	    if(data && data->m_params)
	    {
		SetNrNeighbors(data->m_params->GetValueAsInt(Constants::KW_NrNeighbors, GetNrNeighbors()));
		SetBatchSizeToGenerateVertices(data->m_params->GetValueAsDouble(Constants::KW_BatchSizeToGenerateVertices, GetBatchSizeToGenerateVertices()));
		SetProbAllowCycles(data->m_params->GetValueAsDouble(Constants::KW_ProbAllowCycles, GetProbAllowCycles()));
		
	    }
	    
	}

		/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the number of neighbors.
	 */
	virtual int GetNrNeighbors(void) const
	{
	    return m_nrNeighbors;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the number of acceptable configurations that need to be generated for each batch.
	 */
	virtual int GetBatchSizeToGenerateVertices(void) const
	{
	    return m_batchSizeToGenerateVertices;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the probability to allow cycles in the planner graph.
	 */
	virtual double GetProbAllowCycles(void) const
	{
	    return m_probAllowCycles;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the number of neighbors.
	 */
	virtual void SetNrNeighbors(const int k)
	{
	    m_nrNeighbors = k;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the number of acceptable configurations that need to be generated for each batch.
	 */
	virtual void SetBatchSizeToGenerateVertices(const int n)
	{
	    m_batchSizeToGenerateVertices = n;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the probability to allow cycles in the planner graph.
	 */
	virtual void SetProbAllowCycles(const double p)
	{
	    m_probAllowCycles = p;
	}


	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Initialize PRM by adding the initial configuration and any goal configuration
	 *       to the planner graph.
	 *
	 *@remarks
	 * - PlannerProblem::GetInitialCfg is added as the initial configuration.
	 * - PlannerProblem::GetGoalAcceptor()->GetAnAcceptableCfg() is added as a goal configuration
	 *   (if the function does not returns a NULL pointer);
	 * - This function should be called only once at the beginning
	 *   after the problem and other components have been setup.
	 */
	virtual void Start(void);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Populate the planner graph and connect neighboring configurations.
	 *
	 *@remarks
	 * - Solve should be called after Start.
	 * - Solve can be called repeatedly until a solution is found.
	 * - Solve returns true as soon as it finds a solution.
	 * - See the class description for more details on how the Solve is implemented in PRM. 
	 */
	virtual bool Solve(const double tmax);

    protected:
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Add a new vertex to the graph with <tt>cfg</tt> as its associated configuration.
	 *
	 *@remarks
	 * - The function returns the id of the new vertex.
	 * - The new vertex is also added to the set of vertices that need to be connected.
	 * - The function does not make a copy of <tt>cfg</tt>; it simply does pointer assignment.
	 * - The function does not check if <tt>cfg</tt> is already in the graph. It is the 
	 *   responsibility of the calling function to perform such a check (using FindCfg)
	 *   if indeed there is a possibility that the same configuration is added to the 
	 *   graph multiple times (which even if it happened would not cause any errors).
	 */
	virtual int AddVertex(Cfg * const cfg);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Use sampling to generate at most <tt>nrCfgs</tt> new vertices -- stop earlier
	 *       if the time limit is reached.
	 *
	 *@remarks
	 *  - Function returns the number of vertices that were generated.
	 */
	virtual int GenerateVertices(const int nrCfgs, const double tmax);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Generate edges for the newly added vertices.
	 *
	 *@remarks
	 *  - While there is time, do:
	 *     - select a vertex from the set of those
	 *       that need to be connected;
	 *     - generate the edges for the selected vertex
	 *       (using GenerateEdgesForVertex);
	 *     - remove the selected vertex from the set of vertices that need to be connected. 
	 *  - Function returns the number of remaining vertices that still
	 *    need to be connected.
	 */
	virtual int GenerateEdgesForRemainingVertices(const double tmax);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Compute the nearest neighbors for the vertex
	 *       and attempt to generate an edge to each neighbor.
	 *
	 *@remarks
	 * - GenerateEdge is used to generate each edge from the vertex to a neighbor.
	 */
	virtual void GenerateEdgesForVertex(const int vid);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Generate the edge connecting these two vertices.
	 *
	 *@remarks
	 * - Function returns true iff the edge was generated.
	 * - CfgOffspringGeneratorToTarget is used to define the path
	 *   from the configuration associated with the vertex <tt>vid1</tt>
	 *   to the configuration associated with the vertex <tt>vid2</tt>.
	 * - PathAcceptor is used to determine whether or not such path is valid.
	 * - PathAcceptor uses the distance <tt>d</tt> (distance between 
	 *   the configurations associated with the vertices <tt>vid1</tt> and <tt>vid2</tt>)
	 *   and GetOneStepDistance() to determine an appropriate time-step resolution.
	 */
	virtual bool GenerateEdge(const int vid1,
				  const int vid2,
				  const double d);

		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Number of neighbors.
	 */
	int m_nrNeighbors;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Number of acceptable configurations that need to be generated for each batch.
	 */
	int m_batchSizeToGenerateVertices;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Probability to allow cycles in the roadmap graph.
	 */
	double  m_probAllowCycles;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Internal variable to keep track of how many more vertices need to be 
	 *       generated to complete the batch (in case PRM reaches the time limit)
	 */
	int m_nrRemainingToCompleteBatch;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Vertices for which the nearest neighbors and edge connections
	 *       have not yet been computed.
	 */
	std::unordered_set<int> m_vidsToBeConnected;


    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to PRM.
     */
    ClassContainer(PRM, m_prm);
    
}

#endif
