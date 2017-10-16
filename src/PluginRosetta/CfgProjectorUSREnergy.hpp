#ifndef Antipatrea__CfgProjectorUSREnergy_HPP_
#define Antipatrea__CfgProjectorUSREnergy_HPP_

#include "Components/CfgProjectors/CfgProjector.hpp"
#include "PluginRosetta/MolecularStructureRosetta.hpp"
#include "PluginRosetta/USRFactory.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Use Rosetta to compute the USR projection of the configuration values.
     *
     *@par Notes for Kevin
     * - Add all the necessary components that you need to implement this class.
     */    
	class CfgProjectorUSREnergy : public CfgProjector,
				public CfgManagerContainer,
				public MolecularStructureRosettaContainer
	{
	public:
		CfgProjectorUSREnergy(void) : CfgProjector(4),
					CfgManagerContainer(),
					MolecularStructureRosettaContainer()
		{
		}
	
		virtual ~CfgProjectorUSREnergy(void)
		{
		}

		virtual bool CheckSetup(void) const
		{
			if (GetMolecularStructureRosetta() == NULL) std::cout << "MolStructRos is null" << std::endl;

			if (GetCfgManager() == NULL) std::cout << "GetCfgManager is null" << std::endl;

			if (GetCfgManager() && !GetCfgManager()->CheckSetup()) std::cout << "GetCfgManager is not happy" << std::endl;
			if (GetMolecularStructureRosetta() && !GetMolecularStructureRosetta()->CheckSetup()) std::cout << "GetCfgManager is not happy" << std::endl;

			auto cfgManager = GetCfgManager();
			auto mol        = GetMolecularStructureRosetta();


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
		virtual void Project(const Cfg & cfg, double proj[]);
	protected:
		USRFactory usrFactory;
	
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgProjectorUSR.
     */
    ClassContainer(CfgProjectorUSREnergy, m_cfgProjectorUSREnergy);
    
}

#endif
