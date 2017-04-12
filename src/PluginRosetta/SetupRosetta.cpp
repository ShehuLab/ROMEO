#include "PluginRosetta/SetupRosetta.hpp"
#include "Components/CfgAcceptors/CfgAcceptorBasedOnDistance.hpp"
#include <fstream>

extern "C" Antipatrea::Setup* CreateSetupRosetta(void)
{
    return new Antipatrea::SetupRosetta();
}

namespace Antipatrea
{
    void SetupRosetta::Prepare(Params & params)
    {
	//overwrite user-defined parameter values
	//Note for Kevin: change these values to your Rosetta plugins once you implement
	//them
	params.SetValue(Constants::KW_UseCfgAcceptor, Constants::KW_CfgAcceptorBasedOnEnergy);
	params.SetValue(Constants::KW_UseGoalAcceptor, Constants::KW_GoalAcceptorBasedOnDistance);
	params.SetValue(Constants::KW_UseSignedDistanceBetweenTwoValues, Constants::KW_SignedDistanceBetweenTwoAngles);
	params.SetValue(Constants::KW_UseCfgForwardKinematics, Constants::KW_MolecularStructureRosetta);
	params.SetValue(Constants::KW_UseCfgEnergyEvaluator, Constants::KW_CfgEnergyEvaluatorRosetta);
	params.SetValue(Constants::KW_UseCfgImprover, Constants::KW_CfgImproverRosetta);
	params.SetValue(Constants::KW_UseCfgOffspringGenerator, Constants::KW_CfgOffspringGeneratorToTargetByLinearInterpolation); //change to Constants::KW_CfgOffspringGeneratorRosetta
	params.SetValue(Constants::KW_UseCfgSampler, Constants::KW_CfgUniformSamplerInJointSpace); //change to Constants::KW_CfgSamplerRosetta

	NewInstances(params);
	SetupPointers();
	SetupFromParams(params);
	
	//set start & goal
	Cfg *cfgInit = GetCfgManager()->NewCfg();
	Cfg *cfgGoal = GetCfgManager()->NewCfg();

	const char *fname = params.GetValue(Constants::KW_ReadInitAndGoalCfgsFromFile);
	if(fname != NULL)
	{
	    std::ifstream in(fname);
	    GetCfgManager()->ReadCfg(in, *cfgInit);
	    GetCfgManager()->ReadCfg(in, *cfgGoal);
	    in.close();
	}
	
	GetPlannerProblem()->SetInitialCfg(cfgInit);
	auto goalAcceptor = dynamic_cast<GoalAcceptorBasedOnDistance*>(GetGoalAcceptor());
	if(goalAcceptor)
	    goalAcceptor->SetTargetCfg(cfgGoal);

    }

    void SetupRosetta::Test(void)
    {
	Setup::Test();
	//add code here to test whatever you would like
	//at this point, all the components have been setup so you can use them
    }
    
}
