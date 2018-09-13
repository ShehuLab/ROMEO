#include "Components/EdgeCostEvaluators/EdgeCostEvaluatorBasedOnEnergy.hpp"

namespace Antipatrea
{
    EdgeCostEvaluator::Costs EdgeCostEvaluatorBasedOnEnergy::EvaluateGivenFromToCfgs(Cfg & cfgFrom, Cfg & cfgTo)
    {
        if(cfgFrom.IsEnergySet() == false)
            cfgFrom.SetEnergy(GetCfgEnergyEvaluator()->EvaluateEnergy(cfgFrom));
         if(cfgTo.IsEnergySet() == false)
            cfgTo.SetEnergy(GetCfgEnergyEvaluator()->EvaluateEnergy(cfgTo));

        Costs costs;
        costs.m_costFromTo = cfgTo.GetEnergy() - cfgFrom.GetEnergy();
        costs.m_costToFrom = -costs.m_costFromTo;

        return costs;
    }

}
