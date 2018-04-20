#ifndef Antipatrea__EdgeCostEvaluatorBasedOnMW_HPP_
#define Antipatrea__EdgeCostEvaluatorBasedOnMW_HPP_

#include "Components/EdgeCostEvaluators/EdgeCostEvaluator.hpp"
#include "Components/CfgEnergyEvaluators/CfgEnergyEvaluator.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Edge cost is defined as the min(0,change in energy  between the start
     *       and end configurations).
     *
     *@remarks
     * - This class needs access to CfgEnergyEvaluator in order to evaluate the configuration energy.
     *
     */
    class EdgeCostEvaluatorBasedOnMW : public EdgeCostEvaluator,
					          public CfgEnergyEvaluatorContainer
    {
    public:
	EdgeCostEvaluatorBasedOnMW(void) : EdgeCostEvaluator(),
					                   CfgEnergyEvaluatorContainer()
	{
	}

	virtual ~EdgeCostEvaluatorBasedOnMW(void)
	{
	}

	virtual bool CheckSetup(void)
	{
	    return
		EdgeCostEvaluator::CheckSetup() &&
		GetCfgEnergyEvaluator() != NULL &&
		GetCfgEnergyEvaluator()->CheckSetup();
	}

	virtual void Info(const char prefix[]) const
	{
	    EdgeCostEvaluator::Info(prefix);
	    Logger::m_out << prefix << " CfgEnergyEvaluator = " << Name(GetCfgEnergyEvaluator()) << std::endl;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Compute the costs for the direct and reverse edge  as the change in energy between the start and end configurations.
	 */
	virtual Costs EvaluateGivenFromToCfgs(Cfg & cfgFrom, Cfg & cfgTo);

    };

    /**
     *@author Erion Plaku, Amarda Shehu, Kevin Molloy
     *@brief Get/set methods for components that need access to EdgeCostEvaluatorBasedOnEnergy.
     */
    ClassContainer(EdgeCostEvaluatorBasedOnMW, m_edgeCostEvaluatorBasedOnMW);
}

#endif
