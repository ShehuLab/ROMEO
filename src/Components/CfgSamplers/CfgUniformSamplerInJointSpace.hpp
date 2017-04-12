#ifndef Antipatrea__CfgUniformSamplerInJointSpace_HPP_
#define Antipatrea__CfgUniformSamplerInJointSpace_HPP_

#include "Components/CfgSamplers/CfgSampler.hpp"
#include "Components/CfgManagers/CfgManager.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Sample a configuration by sampling each value at random from <tt>[-PI, PI]</tt>.
     * 
     *@remarks
     * - This class requires access to CfgManager as it uses the configuration dimension.
     */
    class CfgUniformSamplerInJointSpace : public CfgSampler,
					  public CfgManagerContainer
    {
    public:
	CfgUniformSamplerInJointSpace(void) : CfgSampler(),
					      CfgManagerContainer()
	{
	}
	
	virtual ~CfgUniformSamplerInJointSpace(void)
	{
	}


	virtual bool CheckSetup(void) const
	{
	    return
		CfgSampler::CheckSetup() &&
		GetCfgManager() != NULL &&
		GetCfgManager()->CheckSetup();
	}
	
	virtual void Info(const char prefix[]) const
	{
	    CfgSampler::Info(prefix);
	    Logger::m_out << prefix << " CfgManager = " << Name(GetCfgManager()) <<  std::endl;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Sample a configuration by sampling each value at random from <tt>[-PI, PI]</tt>.
	 */
	virtual bool Sample(Cfg & cfg);
    };
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgUniformSamplerInJointSpace.
     */	 
    ClassContainer(CfgUniformSamplerInJointSpace, m_cfgUniformSamplerInJointSpace);
    
}

#endif
