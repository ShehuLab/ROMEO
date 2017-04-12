#include "PluginRosetta/CfgSamplerRosetta.hpp"

namespace Antipatrea
{
    void CfgSamplerRosetta::SetupFromParams(Params & params)
    {
	CfgSampler::SetupFromParams(params);

	auto data = params.GetData(Constants::KW_CfgSamplerRosetta);
	if(data && data->m_params)
	{
	    //setup any other parameters that you may have
	    //see src/Components/CfgDistances/CfgDistanceLp.hpp for an example
	}
    }

    bool CfgSamplerRosetta::Sample(Cfg & cfg)
    {
	/*
	  auto cfgManager = GetCfgManager();
          auto mol        = GetMolecularStructureRosetta();
          const int dim   = cfgManager->GetDim(); //number of configuration dimensions;
	  double   *vals  = cfg.GetValues(); //values of the configuration

	  //add code to do the sampling

	  //don't forget at the end to say
	  cfg.SetValues(vals);
	  //so that cfg remembers that the values have been changed.
	  //In such cases, it sets the energy to undefined as an indication that it may need to be computed again.
	 */
	
	//true: sample was successfully generated
	//false: failed to generate a sample
	return true;
	
    }

}
