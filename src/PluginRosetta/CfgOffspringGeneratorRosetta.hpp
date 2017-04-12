#ifndef Antipatrea__CfgOffspringGeneratorRosetta_HPP_
#define Antipatrea__CfgOffspringGeneratorRosetta_HPP_

#include "Components/CfgOffspringGenerators/CfgOffspringGenerator.hpp"
#include "PluginRosetta/MolecularStructureRosetta.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Use Rosetta/library to generate an offspring near the parent configuration.
     *
     *@par Notes for Kevin
     * - Add all the necessary components that you need to implement this class.
     * - Base class already contains CfgManager, so it is not added here.
     */    
    class CfgOffspringGeneratorRosetta : public CfgOffspringGenerator,
					 public MolecularStructureRosettaContainer
    {
    public:
	CfgOffspringGeneratorRosetta(void) : CfgOffspringGenerator(),
					     MolecularStructureRosettaContainer()
	{
	}
	
	virtual ~CfgOffspringGeneratorRosetta(void)
	{
	}

	virtual bool CheckSetup(void) const
	{
	    return
		CfgOffspringGenerator::CheckSetup() &&
		GetMolecularStructureRosetta() != NULL &&
		GetMolecularStructureRosetta()->CheckSetup();
	}

	virtual void Info(const char prefix[]) const
	{
	    CfgOffspringGenerator::Info(prefix);
	    Logger::m_out << prefix << " MolecularStructureRosetta = " << Name(GetMolecularStructureRosetta()) << std::endl;
	}

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 */
	virtual void SetupFromParams(Params & params);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Use Rosetta to generate the offspring configuration near the parent configuration.
	 *
	 *@remarks
	 * - Use GetStep() as a measure of how far/different the offspring should be from its parent.
	 */ 
	virtual void GenerateOffspringCfg(Cfg & cfg);

    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgOffspringGeneratorRosetta.
     */
    ClassContainer(CfgOffspringGeneratorRosetta, m_cfgOffspringGeneratorRosetta);
    
}

#endif
