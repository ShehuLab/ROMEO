#ifndef Antipatrea__CfgManager_HPP_
#define Antipatrea__CfgManager_HPP_

#include "Components/Component.hpp"
#include "Components/CfgManagers/Cfg.hpp"
#include "Setup/Defaults.hpp"
#include "Utils/Allocator.hpp"
#include <cstdlib>
#include <ostream>
#include <istream>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Motion-planning component for managing configurations.
     * 
     *@remarks
     * - This class provides functionality to create, delete, copy, read, or print configurations.
     * - The user should set the configuration dimension.
     * - Why design it this way? Consider the alternative where Cfg allocates itself. Then, inside a planner/component 
     *   one could write <tt>Cfg *cfg = new Cfg()</tt> to create a new configuration. This is problematic however if someone
     *   extends the configuration class, e.g., <tt>class MyCfg : public Cfg</tt> and wants to use <tt>MyCfg</tt> instead of <tt>Cfg</tt> for
     *   planning purposes. By using CfgManager, such issue is avoided since one would write a class <tt>class MyCfgManager : public CfgManager</tt>,
     *   set the allocator for the planner/component to an instance of MyCfgManager, and then write inside the planner/component
     *   <tt>Cfg *cfg = GetCfgManager()->NewCfg() </tt>.
     */
    class CfgManager : public Component,
		       public Allocator
    {
    public:
	CfgManager(const int dim = 0) :  Component(),
					 Allocator(dim)
	{
	}
	
	virtual ~CfgManager(void)
	{
	}

	virtual bool CheckSetup(void) const
	{
	    return
		Component::CheckSetup() &&
		GetDim() > 0;
	}

	virtual void Info(const char prefix[]) const
	{
	    Component::Info(prefix);
	    Logger::m_out << prefix << " Dim = " << GetDim() << std::endl
		          << prefix << " Cfg::EnergyUndefined = " << Cfg::ENERGY_UNDEFINED << std::endl;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 * 
	 *@remarks
	 * - Function first invokes Component::SetupFromParams(params).
      	 * - It then sets the configuration dimension (keyword Constants::KW_Dim).
	 * - It uses the parameter group associated with the keyword Constants::KW_CfgManager.
	 * - The parameter value can be specified in a text file as, for example,
	 *     <center><tt>CfgManager { Dim 2 }</tt></center>
	 */
	virtual void SetupFromParams(Params & params)
	{
	    Component::SetupFromParams(params);

	    auto data = params.GetData(Constants::KW_CfgManager);
	    if(data && data->m_params)
		SetDim(data->m_params->GetValueAsInt(Constants::KW_Dim, GetDim()));
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Create a new configuration and allocate memory for the configuration values.
	 */
	virtual Cfg* NewCfg(void) const
	{
	    Cfg *cfg = new Cfg();
	    cfg->SetValues(NewValues());
	    return cfg;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Delete the configuration
	 */
	virtual void DeleteCfg(Cfg * cfg)
	{
	    if(cfg)
		delete cfg;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Copy the configuration values from <tt>cfgSrc</tt> to <tt>cfgDest</tt>.
	 *
	 *@remarks
	 *  - It creates a deep copy of the values of <tt>cfgSrc</tt>, not just pointer assignment.
	 *  - It also copies the energy value from <tt>cfgSrc</tt> to <tt>cfgDest</tt>
	 */
	virtual void CopyCfg(Cfg & cfgDest, const Cfg & cfgSrc) const
	{
	    CopyValues(cfgDest.GetValues(), cfgSrc.GetValues());
	    cfgDest.SetEnergy(cfgSrc.GetEnergy());
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Return a copy of the configuration.
	 *
	 *@remarks
	 *  - It creates a deep copy of the values of <tt>cfgSrc</tt>, not just pointer assignment.
	 *  - It also copies the energy value from <tt>cfgSrc</tt> to <tt>cfgDest</tt>
	 */
	virtual Cfg* CopyCfg(const Cfg & cfgSrc) const
	{
	    Cfg *cfgDest = NewCfg();
	    CopyCfg(*cfgDest, cfgSrc);
	    return cfgDest;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Print the configuration to the output stream.
	 *
	 *@remarks
	 *  - It first prints the configuration values and then the energy. 
	 */
	virtual std::ostream& PrintCfg(std::ostream & out, const Cfg & cfg) const;

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Read the configuration from the input stream.
	 *
	 *@remarks
	 *  - It first reads the configuration values and then the energy. 
	 */
	virtual std::istream& ReadCfg(std::istream & in, Cfg & cfg) const;

    protected:
        /**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Configuration dimension.
	 */
	int m_dim;
	
    };
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgManager.
     */
    ClassContainer(CfgManager, m_cfgManager);
    
}

#endif
