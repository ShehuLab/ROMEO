#ifndef Antipatrea__CfgSamplerRosetta_HPP_
#define Antipatrea__CfgSamplerRosetta_HPP_

#include "Components/CfgSamplers/CfgSampler.hpp"
#include "PluginRosetta/MolecularStructureRosetta.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Use Rosetta to sample values for the configuration.
     *
     *@par Notes for Kevin
     * - Add all the necessary components that you need to implement this class.
     */    
    class CfgSamplerRosetta : public CfgSampler,
                              public CfgManagerContainer,
                              public MolecularStructureRosettaContainer
    {
    public:
        CfgSamplerRosetta(void) : CfgSampler(),
                                  CfgManagerContainer(),
                                  MolecularStructureRosettaContainer()
        {
        }
        
        virtual ~CfgSamplerRosetta(void)
        {
        }

        virtual bool CheckSetup(void) const
        {
            return
                CfgSampler::CheckSetup() &&
                GetCfgManager() != NULL &&
                GetMolecularStructureRosetta() != NULL &&
                GetCfgManager()->CheckSetup() &&
                GetMolecularStructureRosetta()->CheckSetup();
        }

        virtual void Info(const char prefix[]) const
        {
            CfgSampler::Info(prefix);
            Logger::m_out << prefix << " CfgManager                = " << Name(GetCfgManager()) << std::endl
                          << prefix << " MolecularStructureRosetta = " << Name(GetMolecularStructureRosetta()) << std::endl;
        }

        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Set the parameter values of the component from the given parameters.
         */
        virtual void SetupFromParams(Params & params);
        
        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief  Sample from -PI to PI for phi/psi, omega is set to PI
         */
        virtual bool Sample(Cfg & cfg);
                
    };

    /**
     *@author Kevin Molloy, Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgSamplerRosetta.
     */
    ClassContainer(CfgSamplerRosetta, m_cfgSamplerRosetta);
    
}

#endif
