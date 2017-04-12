#ifndef Antipatrea__CfgOffspringGeneratorTowardTarget_HPP_
#define Antipatrea__CfgOffspringGeneratorTowardTarget_HPP_

#include "Components/CfgOffspringGenerators/CfgOffspringGenerator.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface for generating an offspring near a parent configuration
     *       and toward a target configuration.
     * 
     *@remarks
     * - There is no specific requirement to reach the target.
     * - Also the notion of "toward the target" is rather vague and can
     *   be defined in any way that the implementer considers reasonable for the problem 
     *   under consideration.
     * - This class is useful for sampling-based motion planners that expand a tree.
     *   Such class can be used to define a branch from a selected configuration in
     *   a tree toward a target configuration (which can be sampled at random or 
     *   selected near the goal configuration).
     */

    class CfgOffspringGeneratorTowardTarget : public CfgOffspringGenerator
    {
    public:
	CfgOffspringGeneratorTowardTarget(void) : CfgOffspringGenerator(),
						  m_cfgTarget(NULL)
	{
	}
	
	virtual ~CfgOffspringGeneratorTowardTarget(void)
	{
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the pointer to the target configuration.
	 */
	virtual const Cfg* GetTargetCfg(void) const
	{
	    return m_cfgTarget;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the pointer to the target configuration.
	 */
	virtual void SetTargetCfg(const Cfg * const cfgTarget)
	{
	    m_cfgTarget = cfgTarget;
	}
	
    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Pointer to the target configuration.
	 */
	const Cfg *m_cfgTarget;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgOffspringGeneratorTowardTarget.
     */	 
    ClassContainer(CfgOffspringGeneratorTowardTarget, m_cfgOffspringGeneratorTowardTarget);
    
}

#endif
