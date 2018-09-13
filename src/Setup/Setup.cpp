#include "Setup/Setup.hpp"
#include "Utils/Logger.hpp"
#include <iomanip>

#include "Setup/Defaults.hpp"

#include "Components/CfgAcceptors/CfgAcceptorBasedOnDistance.hpp"
#include "Components/CfgAcceptors/CfgAcceptorBasedOnEnergy.hpp"
#include "Components/CfgAcceptors/CfgAcceptorBasedOnMMC.hpp"
#include "Components/CfgAcceptors/CfgAcceptorBasedOnFixedMMC.hpp"

#include "Components/CfgDistances/CfgDistanceLp.hpp"

#include "Components/CfgDistances/SignedDistanceBetweenTwoAngles.hpp"
#include "Components/CfgDistances/SignedDistanceBetweenTwoNumbers.hpp"
 
#include "Components/CfgSamplers/CfgUniformSamplerInJointSpace.hpp"
  
#include "Components/CfgImprovers/CfgImproverDoNothing.hpp"

#include "Components/CfgOffspringGenerators/CfgOffspringGeneratorToTargetByLinearInterpolation.hpp"
#include "Components/CfgOffspringGenerators/CfgOffspringGeneratorByGaussianPerturbation.hpp"

#include "Components/EdgeCostEvaluators/EdgeCostEvaluatorBasedOnDistance.hpp"
#include "Components/EdgeCostEvaluators/EdgeCostEvaluatorBasedOnEnergy.hpp"

#include "PluginRosetta/CfgEnergyEvaluatorRosetta.hpp"
#include "PluginRosetta/CfgDistanceAtomRMSD.hpp"
#include "Planners/PRM.hpp"
#include "Planners/RRT.hpp"
#include "Planners/EST.hpp"
#include "Planners/PGT.hpp"
#include "Planners/FELTR.hpp"

namespace Antipatrea
{    
    bool Setup::CheckSetup(void) const
    {
        bool ok = true;
        int  nrErrors = 0;

        for(auto & comp : m_components)
        {
            ok = comp->CheckSetup();
            Logger::m_out << "   " << std::setw(60) << std::left << Name(comp) << "...CheckSetup = " << (ok ? "ok" : "failed") << std::endl;
            if(ok)
                ++nrErrors;
        }
        return nrErrors == 0;
    }

    void Setup::Info(const char prefix[]) const
    {
        for(auto & comp : m_components)
            comp->Info(prefix);
    }
    
    void Setup::NewInstances(Params & params)
    {
                NewCfgAcceptor(params);
                NewGoalAcceptor(params);
                NewCfgManager(params);
                NewSignedDistanceBetweenTwoValues(params);
                NewCfgDistance(params);
                NewCfgEnergyEvaluator(params);
                NewCfgForwardKinematics(params);
                NewCfgImprover(params);
                NewCfgProjector(params);
                NewCfgSampler(params);
                NewEdgeCostEvaluator(params);
                NewCfgOffspringGenerator(params);
                NewPlannerProblem(params);
                NewPlannerSolution(params);
                NewPlannerGraph(params);
                NewSamplingBasedPlanner(params);
    }
    

    void Setup::NewCfgAcceptor(Params & params)
    {
                auto name = params.GetValue(Constants::KW_UseCfgAcceptor);
                if(StrSameContent(name, Constants::KW_CfgAcceptorBasedOnDistance))
                        SetCfgAcceptor(new CfgAcceptorBasedOnDistance());
                else
                if (StrSameContent(name, Constants::KW_CfgAcceptorBasedOnMMC))
                        SetCfgAcceptor(new CfgAcceptorBasedOnMMC());
                else
                if (StrSameContent(name, Constants::KW_CfgAcceptorBasedOnFixedMMC))
                        SetCfgAcceptor(new CfgAcceptorBasedOnFixedMMC());
                else
                        SetCfgAcceptor(new CfgAcceptorBasedOnEnergy());
                OnNewInstance(GetCfgAcceptor());
    }
    
    void Setup::NewGoalAcceptor(Params & params)
    {
                auto name = params.GetValue(Constants::KW_UseGoalAcceptor);
                if(StrSameContent(name, Constants::KW_GoalAcceptorBasedOnDistance))
                        SetGoalAcceptor(new GoalAcceptorBasedOnDistance());
                else
                        SetGoalAcceptor(new GoalAcceptorBasedOnEnergy());
                GetGoalAcceptor()->UseAsGoalAcceptor(true);
                OnNewInstance(GetGoalAcceptor());
    }
    
    void Setup::NewCfgManager(Params & params)
    {
        SetCfgManager(new CfgManager());
        OnNewInstance(GetCfgManager());
    }
    
    void Setup::NewCfgDistance(Params & params)
    {
        auto name = params.GetValue(Constants:: KW_UseCfgDistance);

        if (StrSameContent(name, Constants::KW_CfgDistanceAtomRMSD))
            SetCfgDistance(new CfgDistanceAtomRMSD);
        else
            SetCfgDistance(new CfgDistanceLp());

        OnNewInstance(GetCfgDistance());
    }

    void Setup::NewSignedDistanceBetweenTwoValues(Params & params)
    {
        auto name = params.GetValue(Constants::KW_UseSignedDistanceBetweenTwoValues);

        if(StrSameContent(name, Constants::KW_SignedDistanceBetweenTwoNumbers))
            SetSignedDistanceBetweenTwoValues(new SignedDistanceBetweenTwoNumbers());
        else
            SetSignedDistanceBetweenTwoValues(new SignedDistanceBetweenTwoAngles());
        OnNewInstance(GetSignedDistanceBetweenTwoValues());
    }
    
    
    void Setup::NewCfgEnergyEvaluator(Params & params)
    {
            auto name = params.GetValue(Constants::KW_UseCfgEnergyEvaluator);
            if (StrSameContent(name,Constants::KW_CfgEnergyEvaluatorRosetta))
                    SetCfgEnergyEvaluator(new CfgEnergyEvaluatorRosetta());

    }
    
    void Setup::NewCfgForwardKinematics(Params & params)
    {        
    }
    
    void Setup::NewCfgImprover(Params & params)
    {
        SetCfgImprover(new CfgImproverDoNothing());
        OnNewInstance(GetCfgImprover());
    }
    
    
    void Setup::NewCfgProjector(Params & params)
    {
    }
    
    void Setup::NewCfgSampler(Params & params)
    {
        SetCfgSampler(new CfgUniformSamplerInJointSpace());
        OnNewInstance(GetCfgSampler());
    }
    
    
    void Setup::NewEdgeCostEvaluator(Params & params)
    {
        auto name = params.GetValue(Constants::KW_UseEdgeCostEvaluator);
        if(StrSameContent(name, Constants::KW_EdgeCostEvaluatorBasedOnDistance))
            SetEdgeCostEvaluator(new EdgeCostEvaluatorBasedOnDistance());
        else
            SetEdgeCostEvaluator(new EdgeCostEvaluatorBasedOnEnergy());
        OnNewInstance(GetEdgeCostEvaluator());
    }
    
    void Setup::NewCfgOffspringGenerator(Params & params)
    {
                auto name = params.GetValue(Constants::KW_UseCfgOffspringGenerator);

                if(StrSameContent(name, Constants::KW_CfgOffspringGeneratorByGaussianPerturbation))
                        SetCfgOffspringGenerator(new CfgOffspringGeneratorByGaussianPerturbation());
                else
                        SetCfgOffspringGenerator(new CfgOffspringGeneratorToTargetByLinearInterpolation());
                OnNewInstance(GetCfgOffspringGenerator());
    }

    void Setup::NewPlannerProblem(Params & params)
    {
        SetPlannerProblem(new PlannerProblem());
        OnNewInstance(GetPlannerProblem());
    }
    
    void Setup::NewPlannerSolution(Params & params)
    {
        SetPlannerSolution(new PlannerSolution());
        OnNewInstance(GetPlannerSolution());
    }
    
    void Setup::NewPlannerGraph(Params & params)
    {
        SetPlannerGraph(new PlannerGraph());
        OnNewInstance(GetPlannerGraph());
    }
    
    void Setup::NewSamplingBasedPlanner(Params & params)
    {
                auto name = params.GetValue(Constants::KW_UsePlanner);

                if(StrSameContent(name, Constants::KW_RRT))
                        SetSamplingBasedPlanner(new RRT());
                else if(StrSameContent(name, Constants::KW_EST))
                        SetSamplingBasedPlanner(new EST());
                else if(StrSameContent(name, Constants::KW_PGT))
                        SetSamplingBasedPlanner(new PGT());
                else if(StrSameContent(name, Constants::KW_FELTR))
                        SetSamplingBasedPlanner(new FELTR());
                else
                  SetSamplingBasedPlanner(new PRM());
                OnNewInstance(GetSamplingBasedPlanner());
    }

    void Setup::SetupPointersCfgAcceptor(void)
    {
                SetupPointersComponent(GetCfgAcceptor());
                if(dynamic_cast<CfgAcceptorBasedOnEnergy*>(GetCfgAcceptor()))
                        dynamic_cast<CfgAcceptorBasedOnEnergy*>(GetCfgAcceptor())->SetCfgEnergyEvaluator(GetCfgEnergyEvaluator());
                else if (dynamic_cast<CfgAcceptorBasedOnMMC*>(GetCfgAcceptor()))
                        dynamic_cast<CfgAcceptorBasedOnMMC*>(GetCfgAcceptor())->SetCfgEnergyEvaluator(GetCfgEnergyEvaluator());
                else if (dynamic_cast<CfgAcceptorBasedOnFixedMMC*>(GetCfgAcceptor()))
                        dynamic_cast<CfgAcceptorBasedOnFixedMMC*>(GetCfgAcceptor())->SetCfgEnergyEvaluator(GetCfgEnergyEvaluator());

                else if(dynamic_cast<CfgAcceptorBasedOnDistance*>(GetCfgAcceptor()))
                        dynamic_cast<CfgAcceptorBasedOnDistance*>(GetCfgAcceptor())->SetCfgDistance(GetCfgDistance());
    }

    void Setup::SetupPointersGoalAcceptor(void)
    {
                SetupPointersComponent(GetGoalAcceptor());
        
                GoalAcceptorBasedOnEnergy *ge;
                GoalAcceptorBasedOnDistance *gd;

                if((ge = dynamic_cast<GoalAcceptorBasedOnEnergy*>(GetGoalAcceptor())))
                        ge->SetCfgEnergyEvaluator(GetCfgEnergyEvaluator());
                else if((gd = dynamic_cast<GoalAcceptorBasedOnDistance*>(GetGoalAcceptor())))
                        gd->SetCfgDistance(GetCfgDistance());
    }
    
    void Setup::SetupPointersCfgManager(void)
    {
         SetupPointersComponent(GetCfgManager());
    }

    void Setup::SetupPointersCfgDistance(void)
    {
                SetupPointersComponent(GetCfgDistance());

                CfgDistanceLp *cfgDistanceLp = dynamic_cast<CfgDistanceLp*>(GetCfgDistance());

                if(cfgDistanceLp)
                {
                        cfgDistanceLp->SetCfgManager(GetCfgManager());
                        cfgDistanceLp->SetSignedDistanceBetweenTwoValues(GetSignedDistanceBetweenTwoValues());
                }
        
                CfgDistanceAtomRMSD *cfgDistanceAtomRMSD = dynamic_cast<CfgDistanceAtomRMSD*>(GetCfgDistance());
                if (cfgDistanceAtomRMSD)
                {
                        cfgDistanceAtomRMSD->SetCfgManager(GetCfgManager());
                }
    }

    void Setup::SetupPointersSignedDistanceBetweenTwoValues(void)
    {
        SetupPointersComponent(GetSignedDistanceBetweenTwoValues());
    }

    void Setup::SetupPointersCfgEnergyEvaluator(void)
    {
        SetupPointersComponent(GetCfgEnergyEvaluator());
    }

    void Setup::SetupPointersCfgForwardKinematics(void)
    {
        if(GetCfgForwardKinematics())
            GetCfgForwardKinematics()->SetCfgManager(GetCfgManager());
        SetupPointersComponent(GetCfgForwardKinematics());
    }

    void Setup::SetupPointersCfgImprover(void)
    {
        SetupPointersComponent(GetCfgImprover());
    }

    void Setup::SetupPointersCfgProjector(void)
    {
        SetupPointersComponent(GetCfgProjector());
    }
    
    void Setup::SetupPointersCfgSampler(void)
    {
        SetupPointersComponent(GetCfgSampler());
        
        if(dynamic_cast<CfgUniformSamplerInJointSpace*>(GetCfgSampler()))
            dynamic_cast<CfgUniformSamplerInJointSpace*>(GetCfgSampler())->SetCfgManager(GetCfgManager());
    }

    void Setup::SetupPointersEdgeCostEvaluator(void)
    {
        SetupPointersComponent(GetEdgeCostEvaluator());

        if(dynamic_cast<EdgeCostEvaluatorBasedOnDistance*>(GetEdgeCostEvaluator()))
            dynamic_cast<EdgeCostEvaluatorBasedOnDistance*>(GetEdgeCostEvaluator())->SetCfgDistance(GetCfgDistance());
        else if(dynamic_cast<EdgeCostEvaluatorBasedOnEnergy*>(GetEdgeCostEvaluator()))
            dynamic_cast<EdgeCostEvaluatorBasedOnEnergy*>(GetEdgeCostEvaluator())->SetCfgEnergyEvaluator(GetCfgEnergyEvaluator());
    }

    void Setup::SetupPointersCfgOffspringGenerator(void)
    {
        SetupPointersComponent(GetCfgOffspringGenerator());
        GetCfgOffspringGenerator()->SetCfgManager(GetCfgManager());
        auto pathGen = dynamic_cast<CfgOffspringGeneratorToTargetByLinearInterpolation*>(GetCfgOffspringGenerator());
        if(pathGen)
            pathGen->SetSignedDistanceBetweenTwoValues(GetSignedDistanceBetweenTwoValues());
    }
    
    void Setup::SetupPointersPlannerProblem(void)
    {
        SetupPointersComponent(GetPlannerProblem());
        GetPlannerProblem()->SetGoalAcceptor(GetGoalAcceptor());
    }
    
    void Setup::SetupPointersPlannerSolution(void)
    {
        SetupPointersComponent(GetPlannerSolution());
        GetPlannerSolution()->SetCfgManager(GetCfgManager());
    }
    
    void Setup::SetupPointersPlannerGraph(void)
    {
        SetupPointersComponent(GetPlannerGraph());
    }

    void Setup::SetupPointersSamplingBasedPlanner(void)
    {
                auto planner = GetSamplingBasedPlanner();

                SetupPointersComponent(planner);
        
                planner->SetCfgManager(GetCfgManager());
                planner->SetPlannerProblem(GetPlannerProblem());
                planner->SetPlannerGraph(GetPlannerGraph());
                planner->SetCfgSampler(GetCfgSampler());
                planner->SetCfgImprover(GetCfgImprover());
                planner->SetCfgDistance(GetCfgDistance());
                planner->SetCfgAcceptor(GetCfgAcceptor());
                planner->SetEdgeCostEvaluator(GetEdgeCostEvaluator());
                planner->SetCfgOffspringGenerator(GetCfgOffspringGenerator());
        
                if(dynamic_cast<PGT*>(planner))
                        dynamic_cast<PGT*>(planner)->SetCfgProjector(GetCfgProjector());

                if(dynamic_cast<FELTR*>(planner))
                                        dynamic_cast<FELTR*>(planner)->SetCfgProjector(GetCfgProjector());
    }

    void Setup::SetupPointers(void)
    {
                SetupPointersCfgManager();
                SetupPointersCfgDistance();
                SetupPointersCfgAcceptor();
                SetupPointersGoalAcceptor();
                SetupPointersSignedDistanceBetweenTwoValues();
                SetupPointersCfgEnergyEvaluator();
                SetupPointersCfgForwardKinematics();
                SetupPointersCfgImprover();
                SetupPointersCfgProjector();
                SetupPointersCfgSampler();
                SetupPointersEdgeCostEvaluator();
                SetupPointersCfgOffspringGenerator();
                SetupPointersPlannerProblem();
                SetupPointersPlannerSolution();
                SetupPointersPlannerGraph();

                SetupPointersSamplingBasedPlanner();
    }

    void Setup::SetupFromParams(Params & params)
    {
        GetCfgManager()->SetupFromParams(params);
        for(auto & comp : m_components)
            comp->SetupFromParams(params);
    }
    
}
