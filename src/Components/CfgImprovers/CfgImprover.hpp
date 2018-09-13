#ifndef Antipatrea__CfgImprover_HPP_
#define Antipatrea__CfgImprover_HPP_

#include "Components/CfgManagers/Cfg.hpp"
#include "Components/Component.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface for modifying a configuration to further improve it, e.g., via energy minimization.
     */
    class CfgImprover : public Component
    {
    public:
        CfgImprover(void) : Component()
        {
        }
        
        virtual ~CfgImprover(void)
        {
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Try to improve the given configuration.
         */
        virtual void Improve(Cfg & cfg) = 0;
    };
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgImprover.
     */
    ClassContainer(CfgImprover, m_cfgImprover);
    
}

#endif
