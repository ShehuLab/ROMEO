#include "Planners/TreeSamplingBasedPlanner.hpp"
#include "Components/CfgOffspringGenerators/CfgOffspringGeneratorTowardTarget.hpp"

#include "Utils/Timer.hpp"

namespace Antipatrea
{
    void TreeSamplingBasedPlanner::Start(void)
    {
	if(m_cfgTarget == NULL)
	    m_cfgTarget = GetCfgManager()->NewCfg();
	
	SamplingBasedPlanner::Start();
/*	
	m_vidsGoal.clear();
	m_vidInit = AddVertex(GetCfgManager()->CopyCfg(*(GetPlannerProblem()->GetInitialCfg())));
*/
    }

    
    bool TreeSamplingBasedPlanner::Solve(const double tmax)
    {
	Timer::Clock  clk;

	Timer::Start(clk);
	while(Timer::Elapsed(clk) < tmax && IsSolved() == false)
	{
	    SampleTargetCfg(*m_cfgTarget);
	    const int vid = SelectVertex();
	    if(vid >= 0 && m_cfgTarget != NULL)
		ExtendToward(vid, *m_cfgTarget);
	}
	return IsSolved();
    }

    
    void TreeSamplingBasedPlanner::SampleTargetCfg(Cfg & cfgTarget)
    {
	bool usedGoal = false;
	
	if(RandomUniformReal() <= GetGoalBias())
	{
	    auto cfg = GetPlannerProblem()->GetGoalAcceptor()->GetAnAcceptableCfg();
	    if(cfg != NULL)
	    {
		usedGoal = true;
		GetCfgManager()->CopyCfg(cfgTarget, *cfg);
	    }
	}

	if(usedGoal == false)
	{
	    if(GetSampleValidTargetCfg())
	    {
		do
		{
		    GetCfgSampler()->Sample(cfgTarget);
		    GetCfgImprover()->Improve(cfgTarget);
		}
		while(GetCfgAcceptor()->IsAcceptable(cfgTarget) == false);
	    }
	    else
		GetCfgSampler()->Sample(cfgTarget);
	}
	
    }
    
    
    TreeSamplingBasedPlanner::ExtendStatus TreeSamplingBasedPlanner::ExtendToward(const int vid, Cfg & cfgTo)
    {
	PlannerGraph *graph = GetPlannerGraph();
	CfgAcceptor                        *goalAcceptor          = GetPlannerProblem()->GetGoalAcceptor();
	CfgOffspringGenerator             *cfgOffspringGenerator         = GetCfgOffspringGenerator();
	CfgManager                         *cfgManager            = GetCfgManager();
	CfgDistance                        *cfgDistance           = GetCfgDistance();
	CfgAcceptor                        *cfgAcceptor           = GetCfgAcceptor();
	EdgeCostEvaluator                  *edgeCostEvaluator     = GetEdgeCostEvaluator();
	Cfg                                *cfgNew                = cfgManager->NewCfg();
	Cfg                                *cfgCurr               = GetPlannerGraph()->GetVertex(vid)->GetCfg();
	int                                 vidCurr               = vid;
	double                              d                     = cfgDistance->Distance(*cfgCurr, cfgTo);
	
	if(dynamic_cast<CfgOffspringGeneratorTowardTarget*>(cfgOffspringGenerator))
	    dynamic_cast<CfgOffspringGeneratorTowardTarget*>(cfgOffspringGenerator)->SetTargetCfg(&cfgTo);
	
	for(int nrSteps = 0; nrSteps < m_extendMaxNrSteps; ++nrSteps)
	{
	    cfgOffspringGenerator->SetParentCfg(cfgCurr);
	    cfgOffspringGenerator->SetStep(std::min(1.0, GetOneStepDistance() / d));
	    cfgOffspringGenerator->GenerateOffspringCfg(*cfgNew);
	    
	    if(cfgAcceptor->IsAcceptable(*cfgNew) == false)
	    {
		cfgManager->DeleteCfg(cfgNew);
		return nrSteps == 0 ? EXTEND_FAILED : EXTEND_OK;
	    }
	    
	    const int vidNew = AddVertex(cfgNew);
	    
	    if(vidNew < 0)
	    {
		cfgManager->DeleteCfg(cfgNew);
		return nrSteps == 0 ? EXTEND_FAILED : EXTEND_OK;
	    }

	    auto edge  = NewEdge();
	    auto costs = edgeCostEvaluator->EvaluateGivenFromToCfgs(*cfgCurr, *cfgNew);
	    edge->SetFromToVertexIds(vidCurr, vidNew);
	    edge->SetCosts(costs.m_costFromTo, costs.m_costToFrom);
	    graph->AddEdge(edge);
	    
	    if(IsSolved())
		return EXTEND_REACHED_GOAL;
	    
	    d = cfgDistance->Distance(*cfgCurr, cfgTo);
	    
	    if(d <= m_extendReachedTargetDistanceThreshold)
		return EXTEND_REACHED_TARGET;
	    
	    //continue expansion
	    
	    cfgCurr = cfgNew;
	    vidCurr = vidNew;
	    cfgNew  = cfgManager->NewCfg();
	}
	
	cfgManager->DeleteCfg(cfgNew);
	return EXTEND_OK;
    }
    
}
