#ifndef Antipatrea__EdgeCostEvaluatorBasedOnDistance_HPP_
#define Antipatrea__EdgeCostEvaluatorBasedOnDistance_HPP_

#include "Components/EdgeCostEvaluators/EdgeCostEvaluator.hpp"
#include "Components/CfgDistances/CfgDistance.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Edge cost is defined as the distance between the start and end configurations.
     *
     *@remarks
     * - This class needs access to CfgDistance in order to evaluate the distance
     *   between the start and end configurations.
     */ 
    class EdgeCostEvaluatorBasedOnDistance : public EdgeCostEvaluator,
                                             public CfgDistanceContainer
    {
    public:
        EdgeCostEvaluatorBasedOnDistance(void) : EdgeCostEvaluator(),
                                                 CfgDistanceContainer()
        {
        }
        
        virtual ~EdgeCostEvaluatorBasedOnDistance(void)
        {
        }

        virtual bool CheckSetup(void)
        {
            return
                EdgeCostEvaluator::CheckSetup() &&
                GetCfgDistance() != NULL &&
                GetCfgDistance()->CheckSetup();
        }

        virtual void Info(const char prefix[]) const
        {
            EdgeCostEvaluator::Info(prefix);
            Logger::m_out << prefix << " CfgDistance = " << Name(GetCfgDistance()) << std::endl;
        }


        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Compute the costs for the direct and reverse path  as the distance between the start and end configurations.
         */ 
        virtual Costs EvaluateGivenFromToCfgs(Cfg & cfgFrom, Cfg & cfgTo)
        {
            Costs costs;
            
            costs.m_costFromTo = costs.m_costToFrom = GetCfgDistance()->Distance(cfgFrom, cfgTo);
            return costs;
        }
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to EdgeCostEvaluatorBasedOnDistance.
     */
    ClassContainer(EdgeCostEvaluatorBasedOnDistance, m_edgeCostEvaluatorBasedOnDistance);
}

#endif
