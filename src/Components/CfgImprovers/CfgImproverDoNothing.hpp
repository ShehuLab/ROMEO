#ifndef Antipatrea__CfgImproverDoNothing_HPP_
#define Antipatrea__CfgImproverDoNothing_HPP_

#include "Components/CfgImprovers/CfgImprover.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief It does nothing; so no improvement -- useful for settings where an improvement cannot be defined.
     */    
    class CfgImproverDoNothing : public CfgImprover
    {
    public:
	CfgImproverDoNothing(void) : CfgImprover()
	{
	}
	
	virtual ~CfgImproverDoNothing(void)
	{
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief It does nothing.
	 */
	virtual void Improve(Cfg & cfg)
	{
	}
	
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgImproverDoNothing.
     */
    ClassContainer(CfgImproverDoNothing, m_cfgImproverDoNothing);
    
}

#endif
