#include "PluginRosetta/CfgProjectorDeltaR.hpp"



namespace Antipatrea

{
    void CfgProjectorDeltaR::SetupFromParams(Params & params)
    {
        CfgProjector::SetupFromParams(params);

        auto data = params.GetData(Constants::KW_CfgProjectorDeltaR);
        if(data && data->m_params)
        {
            //setup any other parameters that you may have
            //see src/Components/CfgDistances/CfgDistanceLp.hpp for an example
        }
    }

    void CfgProjectorDeltaR::Project(const Cfg & cfg, double proj[])
    {
            // measure distance from start
    	double distFromStart = distanceRMSD.Distance(*m_initCfg,cfg);
    	double distFromGoal  = distanceRMSD.Distance(*m_goalCfg,cfg);

    	double deltaR = distFromStart - distFromGoal;

    	proj[0] = deltaR + m_rmsdBetween; // map between 0 and RMSDbetween*2

    	if (m_verboseFlag)
    	{
    		std::cout << "deltaR proj:friStart:" << distFromStart << ":distFromGoal:" << distFromGoal
    			   << "deltaR:" << deltaR << ":proj" << proj[0] << std::endl;
    	}
    }

    unsigned int CfgProjectorDeltaR::GetCellCount()
    {
    	return(m_cellCount);
    }
}
