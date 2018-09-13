#ifndef Antipatrea__CfgEnergyEvaluatorRosetta_HPP_
#define Antipatrea__CfgEnergyEvaluatorRosetta_HPP_

#include "Components/CfgEnergyEvaluators/CfgEnergyEvaluator.hpp"
#include "PluginRosetta/MolecularStructureRosetta.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Use Rosetta to evaluate the energy of the configuration.
     *
     *@par Notes for Kevin
     * - Add all the necessary components that you need to implement this class.
     */    
    class CfgEnergyEvaluatorRosetta : public CfgEnergyEvaluator,
                                      public CfgManagerContainer,
                                      public MolecularStructureRosettaContainer
    {
    public:
        CfgEnergyEvaluatorRosetta(void) : CfgEnergyEvaluator(),
                                          CfgManagerContainer(),
                                          MolecularStructureRosettaContainer()
        {
        }
        
        virtual ~CfgEnergyEvaluatorRosetta(void)
        {
        }

        virtual bool CheckSetup(void) const
        {
            return
                CfgEnergyEvaluator::CheckSetup() &&
                GetCfgManager() != NULL &&
                GetMolecularStructureRosetta() != NULL &&
                GetCfgManager()->CheckSetup() &&
                GetMolecularStructureRosetta()->CheckSetup();
        }

        virtual void Info(const char prefix[]) const
        {
            CfgEnergyEvaluator::Info(prefix);
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
         *@brief Use Rosetta to evaluate the energy of the configuration.
         */
        virtual double EvaluateEnergy(Cfg & cfg);
                
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgEnergyEvaluatorRosetta.
     */
    ClassContainer(CfgEnergyEvaluatorRosetta, m_cfgEnergyEvaluatorRosetta);
    
}

#endif
