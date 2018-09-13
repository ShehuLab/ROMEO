#include "Plugin2D/SetupPoint2D.hpp"
#include "Setup/Defaults.hpp"
#include "Components/CfgAcceptors/CfgAcceptorBasedOnDistance.hpp"


extern "C" Antipatrea::Setup* CreateSetupPoint2D(void)
{
    return new Antipatrea::SetupPoint2D();
}

namespace Antipatrea
{
    void SetupPoint2D::Prepare(Params & params)
    {
        //overwrite user-defined values
        params.SetValue(Constants::KW_UseCfgAcceptor, Constants::KW_CfgAcceptorPoint2D);
        params.SetValue(Constants::KW_UseGoalAcceptor, Constants::KW_GoalAcceptorBasedOnDistance);
        params.SetValue(Constants::KW_UseSignedDistanceBetweenTwoValues, Constants::KW_SignedDistanceBetweenTwoNumbers);
        params.SetValue(Constants::KW_UseCfgProjector, Constants::KW_CfgProjectorPoint2D);
        params.SetValue(Constants::KW_UseCfgSampler, Constants::KW_CfgSamplerPoint2D);
        params.SetValue(Constants::KW_UseCfgOffspringGenerator, Constants::KW_CfgOffspringGeneratorToTargetByLinearInterpolation);
        params.SetValue(Constants::KW_UseEdgeCostEvaluator, Constants::KW_EdgeCostEvaluatorBasedOnDistance);
        
        NewInstances(params);
        
        GetCfgManager()->SetDim(2);
        GetCfgProjector()->SetDim(2);
        
        SetupPointers();
        SetupFromParams(params);
        
        GetCfgManager()->SetDim(2);
        GetCfgProjector()->SetDim(2);

        FixGrid();
        
        
        Cfg *cfgInit = GetCfgManager()->NewCfg();
        Cfg *cfgGoal = GetCfgManager()->NewCfg();
        
        //set random start & goal
        do
            GetCfgSampler()->Sample(*cfgInit);
        while(GetCfgAcceptor()->IsAcceptable(*cfgInit) == false);
        do
            GetCfgSampler()->Sample(*cfgGoal);
        while(GetCfgAcceptor()->IsAcceptable(*cfgGoal) == false);
        
        GetPlannerProblem()->SetInitialCfg(cfgInit);

        auto goalAcceptor = dynamic_cast<GoalAcceptorBasedOnDistance*>(GetGoalAcceptor());
        if(goalAcceptor)
            goalAcceptor->SetTargetCfg(cfgGoal);
    }

}
