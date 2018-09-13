#ifndef Antipatrea__EdgeCostEvaluatorBasedOnEnergy_HPP_
#define Antipatrea__EdgeCostEvaluatorBasedOnEnergy_HPP_

#include "Components/EdgeCostEvaluators/EdgeCostEvaluator.hpp"
#include "Components/CfgEnergyEvaluators/CfgEnergyEvaluator.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Edge cost is defined as the change in energy  between the start and end configurations.
     *
     *@remarks
     * - This class needs access to CfgEnergyEvaluator in order to evaluate the configuration energy.
     *
     *@par Notes for Kevin
     * - Change the implementation here if it makes more sense to define the cost in 
     *   a different way. I just used change in energy as a placeholder.
     * - See also if you need to reimplement EvaluateGivenFromToAndIntermediateCfgs.
     *   The default implementation uses the accumulative costs along the sequence of 
     *   configurations defined by <from, intermediates, to>.
     *   Override the implementation here if costs should be computed in a different way.
     */ 
    class EdgeCostEvaluatorBasedOnEnergy : public EdgeCostEvaluator,
                                           public CfgEnergyEvaluatorContainer
    {
    public:
        EdgeCostEvaluatorBasedOnEnergy(void) : EdgeCostEvaluator(),
                                               CfgEnergyEvaluatorContainer()
        {
        }
        
        virtual ~EdgeCostEvaluatorBasedOnEnergy(void)
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
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to EdgeCostEvaluatorBasedOnEnergy.
     */
    ClassContainer(EdgeCostEvaluatorBasedOnEnergy, m_edgeCostEvaluatorBasedOnEnergy);
}

#endif
