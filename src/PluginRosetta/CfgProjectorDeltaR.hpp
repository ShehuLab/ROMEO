#ifndef Antipatrea__CfgProjectorDeltaR_HPP_
#define Antipatrea__CfgProjectorDeltaR_HPP_


#include "PluginRosetta/CfgDistanceAtomRMSD.hpp"

#include "Components/CfgProjectors/CfgProjector.hpp"
#include "PluginRosetta/MolecularStructureRosetta.hpp"
#include "Planners/PlannerProblem.hpp"
namespace Antipatrea
{
    /**
     *@author Kevin Molloy, Erion Plaku, Amarda Shehu
     *@brief  Compute DeltaR distance
     *
     */
	const int DELTAR_PROJDIM = 1;

    class CfgProjectorDeltaR : public CfgProjector,
                               public CfgManagerContainer,
                               public MolecularStructureRosettaContainer,
							   public PlannerProblemContainer
    {
    public:
        CfgProjectorDeltaR(void) : CfgProjector( DELTAR_PROJDIM),
                                CfgManagerContainer(),
                                MolecularStructureRosettaContainer(),
								PlannerProblemContainer()
        {
        	m_firstProjection = true;
        	m_verboseFlag = false;
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

        virtual void PostSetup()
        {
        	distanceRMSD.SetMolecularStructureRosetta(GetMolecularStructureRosetta());
			distanceRMSD.SetCfgManager(GetCfgManager());

			m_initCfg = GetPlannerProblem()->GetInitialCfg();
			m_goalCfg = GetPlannerProblem()->GetGoalCfg();

			m_rmsdBetween = distanceRMSD.Distance(*m_initCfg,*m_goalCfg);
			m_cellRange = m_rmsdBetween * 2;

			m_cellSize  = 1.0;
			m_cellCount = (int)(ceil(m_cellRange/m_cellSize));
			std::cout << "Dist between is:" << m_cellRange << " with cell count:" << m_cellCount << std::endl;
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
        unsigned int GetCellCount();
    protected:
        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief Object to compute distances.
         *
         *@remarks
         * - The function assumes that <tt>proj</tt> has been properly allocated
         *   (using CfgProjector::NewValues())
         */
        CfgDistanceAtomRMSD distanceRMSD;
    	const Cfg *m_initCfg;
    	const Cfg *m_goalCfg;
    	unsigned int m_cellCount;
    	double m_cellRange;
    	double m_cellSize;
    	double m_rmsdBetween;
    	bool m_firstProjection;
    	bool m_verboseFlag;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgProjectorDeltaR.
     */
    ClassContainer(CfgProjectorDeltaR, m_cfgProjectorDeltaR);
    
}

#endif
