#ifndef Antipatrea__CfgDistance_HPP_
#define Antipatrea__CfgDistance_HPP_

#include "Components/CfgManagers/Cfg.hpp"
#include "Components/Component.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface for computing the distance between two configurations.
     */
    class CfgDistance : public Component
    {
    public:
	CfgDistance(void) : Component()
	{
	}
	
	virtual ~CfgDistance(void)
	{
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Computes the distance between two configurations.
	 */
	virtual double Distance(const Cfg & cfg1, const Cfg & cfg2) = 0;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgDistance.
     */
    ClassContainer(CfgDistance, m_cfgDistance);
    
}

#endif
