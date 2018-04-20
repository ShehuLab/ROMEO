#ifndef Antipatrea__TreeSamplingBasedPlanner_HPP_
#define Antipatrea__TreeSamplingBasedPlanner_HPP_

#include "Planners/SamplingBasedPlanner.hpp"
#include "Setup/Defaults.hpp"

namespace Antipatrea
{
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Sampling-based planners that expand a tree in the configuration space.
     * 
     *@remarks
     * - The tree is rooted at the initial configuration.
     * - The tree is iteratively expanded by adding new vertices and new edges.
     * - Specifically, at each iteration a new branch is expanded from
     *   an existing vertex in the tree using CfgOffspringGenerator to define
     *   the path. Intermediate configurations along the path are added to
     *   the tree when they are valid. The branch expansion stops as soon
     *   as an invalid configuration is encountered (as determined by CfgAcceptor),
     *   the goal is reached, the target is reached, or a maximum number of expansion
     *   steps is reached.
     * - If the path generator corresponds to an instance of CfgOffspringGeneratorTowardTarget,
     *   the branch is expanded toward a target configuration  which could be selected at 
     *   random or according to some other probability distribution or strategy.
     * - A parameter, m_extendMaxNrSteps, defines the maximum number of steps to expand each branch.   
     * - Another parameter, m_extendReachedTargetDistanceThreshold, defines the threshold distance
     *   to be used when determining whether or not a configuration has reached the target.
     * - A third parameter, m_goalBias, defines the goal bias used when sampling the target 
     *   configuration. With probability 1-m_goalBias, the target is sampled using CfgSampler.
     *   With probability m_goalBias, the target is sampled using CfgNearTargetSampler with the 
     *   target set as the goal configuration (as obtained by PlannerProblem::GetGoalAcceptor()->GetAnAcceptableCfg()). 
     */
    class TreeSamplingBasedPlanner : public SamplingBasedPlanner
    {
    public:
	TreeSamplingBasedPlanner(void) : SamplingBasedPlanner(),
					 m_extendMaxNrSteps(Constants::VAL_Tree_ExtendMaxNrSteps),
					 m_extendReachedTargetDistanceThreshold(Constants::VAL_Tree_ExtendReachedTargetDistanceThreshold),
					 m_goalBias(Constants::VAL_Tree_GoalBias),
					 m_sampleValidTargetCfg(true),
					 m_cfgTarget(NULL)
	{
	}
	
	virtual ~TreeSamplingBasedPlanner(void)
	{
	    if(m_cfgTarget)
	    	delete m_cfgTarget;
	}
	
	virtual void Info(const char prefix[]) const
	{
	    SamplingBasedPlanner::Info(prefix);
	    Logger::m_out << prefix << " ExtendMaxNrSteps                     = " << GetExtendMaxNrSteps() << std::endl
		          << prefix << " ExtendReachedTargetDistanceThreshold = " << GetExtendReachedTargetDistanceThreshold() << std::endl 
		          << prefix << " GoalBias                             = " << GetGoalBias() << std::endl
		          << prefix << " SampleValidTargetCfg                 = " << GetSampleValidTargetCfg() << std::endl;
	}

	
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the planner from the given parameters.
	 * 
	 *@remarks
	 * - Function first invokes SamplingBasedPlanner::SetupFromParams(params).
	 * - It then sets the maximum number of steps for the tree-branch expansion, the distance threshold to determine whether the target
	 *   configuration has been reached, the goal bias, and the flag to indicate whether the target should be sampled as a valid cfg 
	 *   (keywords Constants::KW_ExtendMaxNrSteps, Constants::KW_ExtendReachedTargetDistanceThreshold, Constants::KW_GoalBias,
	 *    Constants::KW_SampleValidTargetCfg).
	 * - It uses the parameter group associated with the keyword Constants::KW_TreeSamplingBasedPlanner.
	 * - The parameter value can be specified in a text file as, for example,
	 *     <center><tt>TreeSamplingBasedPlanner { ExtendMaxNrSteps 100 ExtendReachedTargetDistanceThreshold 0.1 GoalBias 0.05 SampleValidTargetCfg true }</tt></center>
	 */
	virtual void SetupFromParams(Params & params)
	{
	    SamplingBasedPlanner::SetupFromParams(params);
	    
	    auto data = params.GetData(Constants::KW_TreeSamplingBasedPlanner);
	    if(data && data->m_params)
	    {
			SetExtendMaxNrSteps(data->m_params->GetValueAsInt(Constants::KW_ExtendMaxNrSteps,
									  GetExtendMaxNrSteps()));
			SetExtendReachedTargetDistanceThreshold(data->m_params->GetValueAsDouble(Constants::KW_ExtendReachedTargetDistanceThreshold,
												 GetExtendReachedTargetDistanceThreshold()));
			SetGoalBias(data->m_params->GetValueAsDouble(Constants::KW_GoalBias, GetGoalBias()));
			SetSampleValidTargetCfg(data->m_params->GetValueAsBool(Constants::KW_SampleValidTargetCfg, GetSampleValidTargetCfg()));
	    }
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the maximum number of steps for the tree-branch expansion.
	 */
	virtual double GetExtendMaxNrSteps(void) const
	{
	    return m_extendMaxNrSteps;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the distance threshold to determine whether the target
	 *       configuration has been reached.
	 */
	virtual double GetExtendReachedTargetDistanceThreshold(void) const
	{
	    return m_extendReachedTargetDistanceThreshold;
	}

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the goal bias.
	 */
	virtual double GetGoalBias(void) const
	{
	    return m_goalBias;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the value of the flag indicating whether the target should be sampled as a valid configuration.
	 */
	virtual bool GetSampleValidTargetCfg(void) const
	{
	    return m_sampleValidTargetCfg;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the maximum number of steps for the tree-branch expansion.
	 */
	virtual void SetExtendMaxNrSteps(const int n)
	{
	    m_extendMaxNrSteps = n;
	}

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the distance threshold to determine whether the target
	 *       configuration has been reached.
	 */
	virtual void SetExtendReachedTargetDistanceThreshold(const double d)
	{
	    m_extendReachedTargetDistanceThreshold = d;
	}

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the goal bias.
	 */
	virtual void SetGoalBias(const double bias)
	{
	    m_goalBias = bias;
	}


	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the value of the flag indicating whether the target should be sampled as a valid configuration.
	 */
	virtual void SetSampleValidTargetCfg(const bool valid) 
	{
	    m_sampleValidTargetCfg = valid;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Root the tree at the initial configuration.
	 */
	virtual void Start(void);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Iteratively expand the motion tree by adding new vertices and edges.
	 *
	 *@remarks
	 * - Solve should be called after Start.
	 * - Solve can be called repeatedly until a solution is found.
	 * - Solve returns true as soon as it finds a solution.
	 * - Solve calls SelectVertexAndTarget and then ExpandTowardTarget.
	 */
	virtual bool Solve(const double tmax);

    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Sample a target at random or as the goal configuration.
	 *
	 *@remarks
	 * -  With probability 1-m_goalBias, the target is sampled using CfgSampler.
         * -  With probability m_goalBias, the target is sampled using CfgNearTargetSampler 
	 *    with the target set as the goal configuration 
	 *    (as obtained by PlannerProblem::GetGoalAcceptor()->GetAnAcceptableCfg()). 
	 */
	virtual void SampleTargetCfg(Cfg & cfg);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Select a vertex from which to expand and a target toward which to expand a branch in the planner graph.
	 *
	 *@remarks
	 * - Function returns the id of the selected vertex.
	 */
	virtual int SelectVertex(void)
	{
	    return RandomUniformInteger(0, GetPlannerGraph()->GetNrVertices() - 1);
	}
	
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Status of the planner-graph branch expansion.
	 *
	 *@remarks
	 * - EXTEND_FAILED: no extension occurred.
	 * - EXTEND_OK: at least one extension step succeeded.
	 * - EXTEND_REACHED_TARGET: target was reached.
	 * - EXTEND_REACHED_GOAL: goal was reached.
	 */
	enum ExtendStatus
	    {
		EXTEND_FAILED,
		EXTEND_OK,
		EXTEND_REACHED_TARGET,
		EXTEND_REACHED_GOAL
	    };

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Extend a branch in the planner graph from the selected vertex toward the target configuration.
	 *
	 *@remarks
	 * - If the path generator can be dynamically cast to CfgOffspringGeneratorTowardTarget,
         *   the branch is expanded from the selected vertex toward the target configuration.
	 * - Otherwise, CfgOffspringGenerator is used to expand the branch from the selected vertex. 
	 * - Intermediate configurations along the path are added to
	 *   the planner graph when they are valid. The branch expansion stops as soon
	 *   as an invalid configuration is encountered (as determined by CfgAcceptor),
	 *   the goal is reached, the target is reached, or a maximum number of expansion
	 *   steps is reached.	 
	 */
	virtual ExtendStatus ExtendToward(const int vidFrom, Cfg & cfgTo);

		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Maximum number of steps for the tree-branch expansion.
	 */
	int m_extendMaxNrSteps;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Distance threshold to determine whether the target
	 *       configuration has been reached.
	 */
	double m_extendReachedTargetDistanceThreshold;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Goal bias.
	 */
	double m_goalBias;

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Flag indicating whether the target should be sampled as a valid configuration.
	 */
	bool m_sampleValidTargetCfg;
	

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Target configuration, which is set by the planner at each iteration.
	 */
	Cfg *m_cfgTarget;
    };

     /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to TreeSamplingBasedPlanner.
     */
   ClassContainer(TreeSamplingBasedPlanner, m_treeSamplingBasedPlanner);
    
}

#endif
