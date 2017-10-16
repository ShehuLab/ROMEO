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
        auto mol        = GetMolecularStructureRosetta();

	double energy = mol->EvaluateEnergy(cfg);

	cfg.SetEnergy(energy);

	return cfg.GetEnergy();
    }

}

