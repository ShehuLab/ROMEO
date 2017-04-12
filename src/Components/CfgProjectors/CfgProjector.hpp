#ifndef Antipatrea__CfgProjector_HPP_
#define Antipatrea__CfgProjector_HPP_

#include "Components/CfgManagers/Cfg.hpp"
#include "Components/Component.hpp"
#include "Setup/Defaults.hpp"
#include "Utils/Allocator.hpp"

namespace Antipatrea
{	
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Motion-planning component for managing and defining configuration projections.
     * 
     *@remarks
     * - Projections are usually over a low-dimensional (Euclidean) space.
     * - The user should set the projection dimension.
     */
    class CfgProjector : public Component,
			 public Allocator
    {
    public:
	CfgProjector(const int dim = 0) :  Component(),
					   Allocator(dim)
	{
	}
	
	virtual ~CfgProjector(void)
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
	    return
		Component::CheckSetup() &&
		GetDim() > 0;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Print the paramater values and the names of the components
	 *       used by this class.
	 * 
	 *@remarks
	 * - This function is useful for debugging purposes to see
	 *   that the component has been setup as desired.
	 * - prefix is used to ensure proper alignment when printing (you can use "" for the prefix, for example).
	 */
	virtual void Info(const char prefix[]) const
	{
	    Component::Info(prefix);
	    Logger::m_out << prefix << " Dim = " << GetDim() << std::endl;
	}

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 * 
	 *@remarks
	 * - Function first invokes Component::SetupFromParams(params).
      	 * - It then sets the projection dimension (keyword defined in Constants::KW_Dim).
	 * - It uses the parameter group associated with the keyword Constants::KW_CfgProjector.
	 * - The parameter value can be specified in a text file as, for example,
	 *     <center><tt>CfgProjector { Dim 2 }</tt></center>
	 */
	virtual void SetupFromParams(Params & params)
	{
	    Component::SetupFromParams(params);

	    auto data = params.GetData(Constants::KW_CfgProjector);
	    if(data && data->m_params)
		SetDim(data->m_params->GetValueAsInt(Constants::KW_Dim, GetDim()));
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Compute the configuration projection.
	 *
	 *@remarks
	 * - The function assumes that <tt>proj</tt> has been properly allocated
	 *   (using CfgProjector::NewValues())
	 */
	virtual void Project(Cfg & cfg, double proj[]) = 0;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgProjector.
     */
    ClassContainer(CfgProjector, m_cfgProjector);
    
}

#endif
