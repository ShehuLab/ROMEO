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

    void CfgProjectorDeltaR::Project(Cfg & cfg, double proj[])
    {
	/*
	  auto cfgManager = GetCfgManager();
          auto mol        = GetMolecularStructureRosetta();
          const int dim   = cfgManager->GetDim(); //number of configuration dimensions;
	  const double   *vals  = cfg.GetValues(); //values of the configuration
 
	  //add code to compute the DeltaR projection
	 */
		
    }
}
