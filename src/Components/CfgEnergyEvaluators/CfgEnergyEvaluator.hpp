#ifndef Antipatrea__CfgEnergyEvaluator_HPP_
#define Antipatrea__CfgEnergyEvaluator_HPP_

#include "Components/CfgManagers/Cfg.hpp"
#include "Components/Component.hpp"

namespace Antipatrea
{        
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface for computing the energy value of a configuration.
     */
    class CfgEnergyEvaluator : public Component
    {
    public:
        CfgEnergyEvaluator(void) : Component()
        {
        }
        
        virtual ~CfgEnergyEvaluator(void)
        {
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Computes the energy value of the configuration.
         */
        virtual double EvaluateEnergy(Cfg & cfg) = 0;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgEnergyEvaluator.
     */
    ClassContainer(CfgEnergyEvaluator, m_cfgEnergyEvaluator);
    
}

#endif
