#ifndef Antipatrea__COMPONENT_HPP_
#define Antipatrea__COMPONENT_HPP_

#include "Utils/Object.hpp"
#include "Utils/Params.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Base class for each motion-planning component.
     * 
     *@remarks
     * Each motion-planning component should extend this class and
     * provide additional functionality as needed.
     */
    class Component : public Object
    {
    public:
	Component(void) : Object()
	{
	}
	
	virtual ~Component(void)
	{
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Check whether or not the component has been setup correctly.
	 * 
	 *@remarks
	 * Check that parameter values and/or pointers to other components
	 * or objects have been setup correctly.
	 */
	virtual bool CheckSetup(void) const
	{
	    return true;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 * 
	 *@remarks
	 * For each parameter value of the component, use the associated keyword to 
	 * get the corresponding paramater value from <tt>params</tt>.
	 */
	virtual void SetupFromParams(Params & params)
	{
	}
    };
}

#endif
