#include "Components/EdgeCostEvaluators/EdgeCostEvaluator.hpp"

namespace Antipatrea
{
    EdgeCostEvaluator::Costs EdgeCostEvaluator::EvaluateGivenFromToAndIntermediateCfgs(Cfg & cfgFrom,
                                                                                       Cfg & cfgTo,
                                                                                       std::vector<Cfg*> & intermediates)
    {
        Costs costs1;
        Costs costs;

        if(intermediates.size() == 0)
            return EvaluateGivenFromToCfgs(cfgFrom, cfgTo);
        costs = EvaluateGivenFromToCfgs(cfgFrom, *(intermediates[0]));

        const int n = intermediates.size();
        for(int i = 1; i < n; ++i)
        {
            costs1 = EvaluateGivenFromToCfgs(*(intermediates[i-1]), *(intermediates[i]));
            costs.m_costFromTo += costs1.m_costFromTo;
            costs.m_costToFrom += costs1.m_costToFrom;
        }
        
        costs1 = EvaluateGivenFromToCfgs(*(intermediates[n-1]), cfgTo);
        costs.m_costFromTo += costs1.m_costFromTo;
        costs.m_costToFrom += costs1.m_costToFrom;
        
        return costs;
        
    }
}
