#ifndef Antipatrea__CfgProjectorUSR_HPP_
#define Antipatrea__CfgProjectorUSR_HPP_

#include "Components/CfgProjectors/CfgProjector.hpp"
#include "PluginRosetta/MolecularStructureRosetta.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Use Rosetta to compute the USR projection of the configuration values.
     *
     *@par Notes for Kevin
     * - Add all the necessary components that you need to implement this class.
     */    
    class CfgProjectorUSR : public CfgProjector,
			    public CfgManagerContainer,
			    public MolecularStructureRosettaContainer
    {
    public:
	CfgProjectorUSR(void) : CfgProjector(),
				CfgManagerContainer(),
				MolecularStructureRosettaContainer()
	{
	}
	
	virtual ~CfgProjectorUSR(void)
	{
	}
	
	virtual bool CheckSetup(void) const
	{
	    return
		CfgProjector::CheckSetup() &&
		GetCfgManager() != NULL &&
		GetMolecularStructureRosetta() != NULL &&
		GetCfgManager()->CheckSetup() &&
		GetMolecularStructureRosetta()->CheckSetup();
	}
	
	virtual void Info(const char prefix[]) const
	{
	    CfgProjector::Info(prefix);
	    Logger::m_out << prefix << " CfgManager                = " << Name(GetCfgManager()) << std::endl
		          << prefix << " MolecularStructureRosetta = " << Name(GetMolecularStructureRosetta()) << std::endl;
	}

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 */
	virtual void SetupFromParams(Params & params);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Use Rosetta to compute the USR projection of the configuration values.
	 *
	 *@remarks
	 * - The function assumes that <tt>proj</tt> has been properly allocated
	 *   (using CfgProjector::NewValues())
	 */
	virtual void Project(Cfg & cfg, double proj[]);
	
	
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgProjectorUSR.
     */
    ClassContainer(CfgProjectorUSR, m_cfgProjectorUSR);
    
}

#endif
