#include "PluginRosetta/CfgEnergyEvaluatorRosetta.hpp"

namespace Antipatrea
{

    void CfgEnergyEvaluatorRosetta::SetupFromParams(Params & params)
    {
	CfgEnergyEvaluator::SetupFromParams(params);

	auto data = params.GetData(Constants::KW_CfgEnergyEvaluatorRosetta);
	if(data && data->m_params)
	{
	    //setup any other parameters that you may have
	    //see src/Components/CfgDistances/CfgDistanceLp.hpp for an example
	}
    }

    double CfgEnergyEvaluatorRosetta::EvaluateEnergy(Cfg & cfg)
    {
	/*
	  auto cfgManager = GetCfgManager();
          auto mol        = GetMolecularStructureRosetta();
          const int dim   = cfgManager->GetDim(); //number of configuration dimensions;
	  const double *vals  = cfg.GetValues(); //values of the configuration

	  double energy = //add code to evaluate the energy

	  cfg.SetEnergy(energy);
	 */

	return cfg.GetEnergy();
    }

}

