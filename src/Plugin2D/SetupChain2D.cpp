#include "Plugin2D/SetupChain2D.hpp"
#include "Components/CfgAcceptors/CfgAcceptorBasedOnDistance.hpp"
#include <fstream>

extern "C" Antipatrea::Setup* CreateSetupChain2D(void)
{
    return new Antipatrea::SetupChain2D();
}

namespace Antipatrea
{
    void SetupChain2D::Prepare(Params & params)
    {
        //overwrite user-defined parameter values
        params.SetValue(Constants::KW_UseCfgAcceptor, Constants::KW_CfgAcceptorChain2D);
        params.SetValue(Constants::KW_UseGoalAcceptor, Constants::KW_GoalAcceptorBasedOnDistance);
        params.SetValue(Constants::KW_UseSignedDistanceBetweenTwoValues, Constants::KW_SignedDistanceBetweenTwoAngles);
        params.SetValue(Constants::KW_UseCfgForwardKinematics, Constants::KW_Chain2D);
        params.SetValue(Constants::KW_UseCfgProjector, Constants::KW_CfgProjectorChain2D);
        params.SetValue(Constants::KW_UseCfgSampler, Constants::KW_CfgUniformSamplerInJointSpace);
        params.SetValue(Constants::KW_UseCfgOffspringGenerator, Constants::KW_CfgOffspringGeneratorToTargetByLinearInterpolation);
        params.SetValue(Constants::KW_UseEdgeCostEvaluator, Constants::KW_EdgeCostEvaluatorBasedOnDistance);
        
        NewInstances(params);
        SetupPointers();
        SetupFromParams(params);

        GetCfgManager()->SetDim(GetChain2D()->GetNrJoints());
        GetCfgProjector()->SetDim(2);
        
        FixGrid();
        
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
        else
        {
            for(int i = GetCfgManager()->GetDim() - 1; i >= 0; --i)
            {
                cfgInit->GetValues()[i] =  0.0;
                cfgGoal->GetValues()[i] =  0.0;
            }
            cfgGoal->GetValues()[0] = M_PI;
        }
        
        cfgInit->SetValues(cfgInit->GetValues());
        cfgGoal->SetValues(cfgGoal->GetValues());
        
        GetPlannerProblem()->SetInitialCfg(cfgInit);
        auto goalAcceptor = dynamic_cast<GoalAcceptorBasedOnDistance*>(GetGoalAcceptor());
        if(goalAcceptor)
            goalAcceptor->SetTargetCfg(cfgGoal);

    }

}
