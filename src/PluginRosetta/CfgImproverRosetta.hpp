#ifndef Antipatrea__CfgImproverRosetta_HPP_
#define Antipatrea__CfgImproverRosetta_HPP_

#include "Components/CfgImprovers/CfgImprover.hpp"
#include "PluginRosetta/MolecularStructureRosetta.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Use Rosetta to improve the energetic value of the configuration.
     *
     *@par Notes for Kevin
     * - Add all the necessary components that you need to implement this class.
     */    
    class CfgImproverRosetta : public CfgImprover,
                               public CfgManagerContainer,
                               public MolecularStructureRosettaContainer
    {
    public:
        CfgImproverRosetta(void) : CfgImprover(),
                                   CfgManagerContainer(),
                                   MolecularStructureRosettaContainer()
        {
        }
        
        virtual ~CfgImproverRosetta(void)
        {
        }

        virtual bool CheckSetup(void) const
        {
            return
                CfgImprover::CheckSetup() &&
                GetCfgManager() != NULL &&
                GetMolecularStructureRosetta() != NULL &&
                GetCfgManager()->CheckSetup() &&
                GetMolecularStructureRosetta()->CheckSetup();
        }

        virtual void Info(const char prefix[]) const
        {
            CfgImprover::Info(prefix);
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
         *@brief Use Rosetta to improve the energetic value of the configuration.
         */
        virtual void Improve(Cfg & cfg);
                
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgImproverRosetta.
     */
    ClassContainer(CfgImproverRosetta, m_cfgImproverRosetta);
    
}

#endif
