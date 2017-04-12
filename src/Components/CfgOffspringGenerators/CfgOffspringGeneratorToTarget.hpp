#ifndef Antipatrea__CfgOffspringGeneratorToTarget_HPP_
#define Antipatrea__CfgOffspringGeneratorToTarget_HPP_

#include "Components/CfgOffspringGenerators/CfgOffspringGeneratorTowardTarget.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Generate an offspring configuration along a path that starts at the parent configuration
     *       and reaches the specific target configuration.
     * 
     *@remarks
     * - The parameter, m_step, is interpreted here as time, ranging from 0 to 1.
     * - Classes implementing this interface should ensure that the path starts
     *   at the specified configuration (GetParentCfg()) (when step = 0) and ends at the 
     *   specified configuration (GetTargetCfg()) (when step = 1).
     * - Such class is used by sampling-based motion planners that build a roadmap
     *   to connect configurations to one another.
     */
    class CfgOffspringGeneratorToTarget : public CfgOffspringGeneratorTowardTarget
    {
    public:
	CfgOffspringGeneratorToTarget(void) : CfgOffspringGeneratorTowardTarget()
	{
	}
	
	virtual ~CfgOffspringGeneratorToTarget(void)
	{
	}
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgOffspringGeneratorToTarget.
     */	 
    ClassContainer(CfgOffspringGeneratorToTarget, m_cfgOffspringGeneratorToTarget);
    
}

#endif
