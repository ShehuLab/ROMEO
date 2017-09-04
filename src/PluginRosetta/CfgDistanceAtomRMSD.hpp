#ifndef Antipatrea__CfgDistanceAtomRMSD_HPP_
#define Antipatrea__CfgDistanceAtomRMSD_HPP_

#include "Components/CfgDistances/CfgDistance.hpp"
#include "Components/CfgManagers/CfgManager.hpp"
#include "PluginRosetta/MolecularStructureRosetta.hpp"

namespace Antipatrea
{
    /**
     *@author Kevin Molloy, Erion Plaku, Amarda Shehu
     *@brief Defines the distance between two configurations as the least root
     *       mean square deviation (RMSD) between the backbone atoms.
     *
     *
     *@remarks
     *  - Distance is computed as the normalized sum of the square of the differences in each atom's position.
     *  - It requires access to CfgManager to access the configuration dimension.
     */

    class CfgDistanceAtomRMSD : public CfgDistance,
			        public CfgManagerContainer,
			        public MolecularStructureRosettaContainer
    {
    public:
	CfgDistanceAtomRMSD(void) : CfgDistance(),
			      CfgManagerContainer()
	{
	}
	
	virtual ~CfgDistanceAtomRMSD(void)
	{
	}
	
	virtual bool CheckSetup(void) const
	{
	    return
		CfgDistance::CheckSetup() &&
		GetCfgManager() != NULL &&
		GetCfgManager()->CheckSetup() &&
	    GetMolecularStructureRosetta() != NULL &&
	    GetMolecularStructureRosetta()->CheckSetup();}

	virtual void Info(const char prefix[]) const
	{
	    CfgDistance::Info(prefix);
	    Logger::m_out << prefix << " CfgManager          = " << Name(GetCfgManager()) << std::endl;
	}

	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 * 
	 *@remarks
	 * - Function first invokes CfgDistance::SetupFromParams(params).
	 */
	virtual void SetupFromParams(Params & params)
	{
	    CfgDistance::SetupFromParams(params);
	}

	
	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Computes the distance between two configurations as the least RMSD
	 *       of the pairwise distances between heavy backbone atom positions.
	 *
	 */
	virtual double Distance(const Cfg & cfg1,
				const Cfg & cfg2);
    };

    /**
     *@author Kevin Molloy, Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgDistanceLp.
     */	 
    ClassContainer(CfgDistanceAtomRMSD, m_cfgDistanceAtomRMSD);
    
}

#endif
