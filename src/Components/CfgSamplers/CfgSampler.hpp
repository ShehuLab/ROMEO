#ifndef Antipatrea__CfgSampler_HPP_
#define Antipatrea__CfgSampler_HPP_

#include "Components/CfgManagers/Cfg.hpp"
#include "Components/Component.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface for configuration sampling.
     */    
    class CfgSampler : public Component
    {
    public:
        CfgSampler(void) : Component()
        {
        }
        
        virtual ~CfgSampler(void)
        {
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Sample a configuration.
         */    
        virtual bool Sample(Cfg & cfg) = 0;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgSampler.
     */
    ClassContainer(CfgSampler, m_cfgSampler);
    
}

#endif
