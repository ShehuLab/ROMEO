#ifndef Antipatrea__CfgProjectorDeltaR_HPP_
#define Antipatrea__CfgProjectorDeltaR_HPP_

#include "Components/CfgProjectors/CfgProjector.hpp"
#include "PluginRosetta/MolecularStructureRosetta.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Use Rosetta to compute the DeltaR projection of the configuration values.
     *
     *@par Notes for Kevin
     * - Add all the necessary components that you need to implement this class.
     */    
    class CfgProjectorDeltaR : public CfgProjector,
                            public CfgManagerContainer,
                            public MolecularStructureRosettaContainer
    {
    public:
        CfgProjectorDeltaR(void) : CfgProjector(),
                                CfgManagerContainer(),
                                MolecularStructureRosettaContainer()
        {
        }
        
        virtual ~CfgProjectorDeltaR(void)
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
         *@brief Use Rosetta to compute the DeltaR projection of the configuration values.
         *
         *@remarks
         * - The function assumes that <tt>proj</tt> has been properly allocated
         *   (using CfgProjector::NewValues())
         */
        virtual void Project(const Cfg & cfg, double proj[]);
        
        
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgProjectorDeltaR.
     */
    ClassContainer(CfgProjectorDeltaR, m_cfgProjectorDeltaR);
    
}

#endif
