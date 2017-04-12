#include "PluginRosetta/CfgProjectorUSR.hpp"

namespace Antipatrea
{
    void CfgProjectorUSR::SetupFromParams(Params & params)
    {
	CfgProjector::SetupFromParams(params);

	auto data = params.GetData(Constants::KW_CfgProjectorUSR);
	if(data && data->m_params)
	{
	    //setup any other parameters that you may have
	    //see src/Components/CfgDistances/CfgDistanceLp.hpp for an example
	}
    }

    void CfgProjectorUSR::Project(Cfg & cfg, double proj[])
    {
	/*
	  auto cfgManager = GetCfgManager();
          auto mol        = GetMolecularStructureRosetta();
          const int dim   = cfgManager->GetDim(); //number of configuration dimensions;
	  const double   *vals  = cfg.GetValues(); //values of the configuration
 
	  //add code to compute the USR projection
	 */
		
    }
}
