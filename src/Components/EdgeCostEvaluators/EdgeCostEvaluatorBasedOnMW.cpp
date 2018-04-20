#include "Components/EdgeCostEvaluators/EdgeCostEvaluatorBasedOnMW.hpp"

namespace Antipatrea
{
    EdgeCostEvaluator::Costs EdgeCostEvaluatorBasedOnMW::EvaluateGivenFromToCfgs(Cfg & cfgFrom, Cfg & cfgTo)
    {
	if(cfgFrom.IsEnergySet() == false)
	    cfgFrom.SetEnergy(GetCfgEnergyEvaluator()->EvaluateEnergy(cfgFrom));
 	if(cfgTo.IsEnergySet() == false)
	    cfgTo.SetEnergy(GetCfgEnergyEvaluator()->EvaluateEnergy(cfgTo));

	Costs costs;
	costs.m_costFromTo = cfgTo.GetEnergy() - cfgFrom.GetEnergy();
	costs.m_costToFrom = -costs.m_costFromTo;

	if (costs.m_costFromTo < 0.0)
		costs.m_costFromTo = 0.0;

	if (costs.m_costToFrom < 0.0)
		costs.m_costToFrom = 0.0;

	return costs;
    }

}

