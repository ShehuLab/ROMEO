#ifndef Antipatrea_Setup_HPP_
#define Antipatrea_Setup_HPP_

#include "Components/CfgAcceptors/CfgAcceptor.hpp"
#include "Components/CfgManagers/CfgManager.hpp"
#include "Components/CfgDistances/CfgDistance.hpp"
#include "Components/CfgDistances/SignedDistanceBetweenTwoValues.hpp"
#include "Components/CfgEnergyEvaluators/CfgEnergyEvaluator.hpp"
#include "Components/CfgForwardKinematics/CfgForwardKinematics.hpp"
#include "Components/CfgImprovers/CfgImprover.hpp"
#include "Components/CfgProjectors/CfgProjector.hpp"
#include "Components/CfgSamplers/CfgSampler.hpp"
#include "Components/CfgOffspringGenerators/CfgOffspringGenerator.hpp"
#include "Components/EdgeCostEvaluators/EdgeCostEvaluator.hpp"
#include "Planners/PlannerProblem.hpp"
#include "Planners/PlannerSolution.hpp"
#include "Planners/PlannerGraph.hpp"
#include "Planners/SamplingBasedPlanner.hpp"
#include "Utils/Misc.hpp"
#include "Utils/Params.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Facilitate setting up the planning problem as well as the
     *       planner and its components.
     *
     *@remarks
     * - Setup creates instances of the planning components based on
     *   keywords provided by the user or default settings
     *   when the user does not provide any keywords.
     * - Only one instance per class is created.
     * - After creating the components, Setup sets the pointers
     *   so that components/planners that depend on, for example, CfgDistance
     *   will have the CfgDistance pointer set to the CfgDistance
     *   instance created in the first step.
     * - After setting the pointers, Setup iterates over all the instances
     *   that were created in step 1, and calls the function SetupFromParams
     *   in order to set the parameter values for each instance.
     */ 
    class Setup :  public Component,
		   public CfgAcceptorContainer,
		   public CfgManagerContainer,
		   public CfgDistanceContainer,
		   public SignedDistanceBetweenTwoValuesContainer,
		   public CfgEnergyEvaluatorContainer,
		   public CfgForwardKinematicsContainer,
		   public CfgImproverContainer,
		   public CfgProjectorContainer,
		   public CfgSamplerContainer,
		   public CfgOffspringGeneratorContainer,
		   public EdgeCostEvaluatorContainer,
		   public GoalAcceptorContainer,
		   public PlannerProblemContainer,
		   public PlannerSolutionContainer,
		   public PlannerGraphContainer,
		   public SamplingBasedPlannerContainer
    {
    public:
	Setup(void) : Component(),
		      CfgAcceptorContainer(),
		      CfgManagerContainer(),
		      CfgDistanceContainer(),
		      SignedDistanceBetweenTwoValuesContainer(),
		      CfgEnergyEvaluatorContainer(),
		      CfgImproverContainer(),
		      CfgProjectorContainer(),
		      CfgSamplerContainer(),
		      CfgOffspringGeneratorContainer(),
		      EdgeCostEvaluatorContainer(),
		      GoalAcceptorContainer(),
		      PlannerProblemContainer(),
		      PlannerSolutionContainer(),
		      PlannerGraphContainer(),
		      SamplingBasedPlannerContainer()
	{
	}
	
	virtual ~Setup(void)
	{
	    DeleteItems<Component*>(m_components);
	}
	
	virtual bool CheckSetup(void) const;

	virtual void Info(const char prefix[]) const;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Create new instances, setup pointers, and setup the parameters.
	 *
	 *@remark
	 * - Do everything that needs to be done in order to setup
	 *   the planning problem, the planner, and its components. 
  	 */
	virtual void Prepare(Params & params) = 0;

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Write whatever you would like for testing purposes.
	 *
	 *@remark
	 * - Add code to test various components. 
  	 */
	virtual void Test(void)
	{
	}
	
    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Add the instance to the set of component instances.
	 */ 
	virtual void OnNewInstance(Component * const comp)
	{
	    if(comp != NULL)
	    	m_components.push_back(comp);
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Create all the new instances.
	 *
	 *@remarks
	 *  - Function calls all the specific New functions
	 *    listed below, e.g., NewCfgAcceptor, NewGoalAcceptor, and so on.
	 */ 
	virtual void NewInstances(Params & params);


	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Setup the component pointers.
	 *
	 *@remarks
	 * - After creating the components, Setup sets the pointers
	 *   so that components/planners that depend on, for example, CfgDistance
	 *   will have the CfgDistance pointer set to the CfgDistance
	 *   instance created when NewInstances() was called.
	 */ 	
	virtual void SetupPointers(void);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Create a new instance for the indicated component.
	 *
	 *@remarks
	 *  - If more than one choice, use keywords defined in params to select 
	 *    among the various classes.
	 */
	virtual void NewCfgAcceptor(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewGoalAcceptor(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewCfgManager(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewSignedDistanceBetweenTwoValues(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewCfgDistance(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewCfgEnergyEvaluator(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewCfgForwardKinematics(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewCfgImprover(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewCfgProjector(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewCfgSampler(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewEdgeCostEvaluator(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewCfgOffspringGenerator(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewPlannerProblem(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewPlannerSolution(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewPlannerGraph(Params & params);
	/**
	 *@copydoc Setup::NewCfgAcceptor(Params &)
	 */
	virtual void NewSamplingBasedPlanner(Params & params);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Setup the pointers for the indicated component.
	 */
	virtual void SetupPointersComponent(Component * const comp)
	{
	}
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersCfgAcceptor(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersGoalAcceptor(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersCfgManager(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersCfgDistance(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersSignedDistanceBetweenTwoValues(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersCfgEnergyEvaluator(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersCfgForwardKinematics(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersCfgImprover(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersCfgProjector(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersCfgSampler(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersEdgeCostEvaluator(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersCfgOffspringGenerator(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersPlannerProblem(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersPlannerSolution(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersPlannerGraph(void);
	/**
	 *@copydoc Setup::SetupPointersComponent(Component * const)
	 */
	virtual void SetupPointersSamplingBasedPlanner(void);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Setup the paramater values of each component instance using the user-provided parameters.
	 *
	 *@remarks
	 * - Iterate over all the component instances <tt>comp</tt> that were created by NewInstances
	 *   and call the function <tt>comp->SetupFromParams(params)</tt>
	 *   in order to set the parameter values for each instance.
	 */
	virtual void SetupFromParams(Params & params);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Component instances.
	 */	 
	std::vector<Component*> m_components;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Pointer to function for creating a Setup instance.
     */
    typedef Setup* (*CreateSetupFn)(void);
    
}

#endif
