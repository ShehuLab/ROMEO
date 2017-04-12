#include "PluginRosetta/CfgOffspringGeneratorRosetta.hpp"

namespace Antipatrea
{


    void CfgOffspringGeneratorRosetta::SetupFromParams(Params & params)
    {
	CfgOffspringGenerator::SetupFromParams(params);

	auto data = params.GetData(Constants::KW_CfgOffspringGeneratorRosetta);
	if(data && data->m_params)
	{
	    //setup any other parameters that you may have
	    //see src/Components/CfgDistances/CfgDistanceLp.hpp for an example
	}
    }

    void CfgOffspringGeneratorRosetta::GenerateOffspringCfg(Cfg & cfg)
    {
	/*
	  auto cfgManager = GetCfgManager();
          auto mol        = GetMolecularStructureRosetta();
          const int dim   = cfgManager->GetDim(); //number of configuration dimensions;
	  double *vals    = cfg.GetValues(); //values of the configuration

	  //parent configuration: GetParentCfg()
	  //step size: GetStep() [as a measure of how far/different the offspring should be from its parent]
	  
	  //add code to do generate the offspring

	  //don't forget at the end to say
	  cfg.SetValues(vals);
	  //so that cfg remembers that the values have been changed.
	  //In such cases, it sets the energy to undefined as an indication that it may need to be computed again.
	 */
		
    }

}

