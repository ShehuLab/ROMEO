#include "Planners/PRM.hpp"
#include "Setup/Defaults.hpp"
#include "Utils/Timer.hpp"
#include "Utils/Stats.hpp"
#include "Utils/Logger.hpp"

#include "Components/CfgAcceptors/CfgAcceptorBasedOnMMC.hpp"
#include "Components/CfgAcceptors/CfgAcceptorBasedOnFixedMMC.hpp"

namespace Antipatrea
{
    void PRM::Start(void)
    {
                SamplingBasedPlanner::Start();

                auto cfgGoal = GetPlannerProblem()->GetGoalAcceptor()->GetAnAcceptableCfg();
                if(cfgGoal != NULL)
                        AddVertex(GetCfgManager()->CopyCfg(*cfgGoal));
    }

    bool PRM::Solve(const double tmax)
    {

                Timer::Clock clk;

                Timer::Start(clk);
                while(IsSolved() == false && Timer::Elapsed(clk) < tmax)
                {
                        if(m_nrRemainingToCompleteBatch < 0)
                        m_nrRemainingToCompleteBatch = m_batchSizeToGenerateVertices;
                        if(m_nrRemainingToCompleteBatch > 0)
                        {
                                const int n = GenerateVertices(m_nrRemainingToCompleteBatch, tmax - Timer::Elapsed(clk));

                                m_nrRemainingToCompleteBatch -= n;
                                if(m_nrRemainingToCompleteBatch < 0)
                                        m_nrRemainingToCompleteBatch = 0;
                        }
                        else if(m_vidsToBeConnected.size() > 0)
                                GenerateEdgesForRemainingVertices(tmax - Timer::Elapsed(clk));
                        else
                                m_nrRemainingToCompleteBatch = -1;
                }
                return IsSolved();
    }

    
    int PRM::AddVertex(Cfg * const cfg)
    {
                const int vid = SamplingBasedPlanner::AddVertex(cfg);
                if(vid >= 0)
                        m_vidsToBeConnected.insert(vid);
                return vid;
    }

    int PRM::GenerateVertices(const int nrCfgs, const double tmax)
    {
                CfgSampler   *cfgSampler = GetCfgSampler();
                CfgImprover  *cfgImprover = GetCfgImprover();
                CfgAcceptor  *cfgAcceptor     = GetCfgAcceptor();
                CfgManager   *cfgManager    = GetCfgManager();
                Cfg             *cfg             = cfgManager->NewCfg();
                int              count           = 0;
                Timer::Clock     clk;

                Timer::Start(clk);
                while(count < nrCfgs && Timer::Elapsed(clk) < tmax)
                {
                        cfgSampler->Sample(*cfg);
                        cfgImprover->Improve(*cfg);
                        if(cfgAcceptor->IsAcceptable(*cfg))
                        {
                        ++count;
                        AddVertex(cfg);
                        cfg = cfgManager->NewCfg();
                        }
                }
                cfgManager->DeleteCfg(cfg);

                Stats::GetSingleton()->AddValue(Constants::KW_Runtime_PRM_GenerateVertices, Timer::Elapsed(clk));

                return count;
    }
    
    int PRM::GenerateEdgesForRemainingVertices(const double tmax)
    {
                Timer::Clock clk;
                Timer::Start(clk);

                while(IsSolved() == false &&
                          Timer::Elapsed(clk) < tmax &&
                          m_vidsToBeConnected.empty() == false)
                {
                        GenerateEdgesForVertex(*(m_vidsToBeConnected.begin()));
                }

                Stats::GetSingleton()->AddValue(Constants::KW_Runtime_PRM_GenerateEdges, Timer::Elapsed(clk));

                return m_vidsToBeConnected.size();
    }
    
    void PRM::GenerateEdgesForVertex(const int vid)
    {
                ProximityQuery<int>   query;
                ProximityResults<int> res;

                query.SetNrNeighbors(GetNrNeighbors());
                query.SetKey(vid);
                m_proximityDataStructure.Neighbors(query, res);

                const int n = res.GetNrResults();
                for(int i = 0; i < n; ++i)
                        GenerateEdge(vid, res.GetKey(i), res.GetDistance(i));
                m_vidsToBeConnected.erase(vid);
    }
  

    bool PRM::GenerateEdge(const int vid1, const int vid2, const double d)
    {
                auto graph                 = GetPlannerGraph();
                auto cfgOffspringGenerator = GetCfgOffspringGenerator();
                auto cfgManager            = GetCfgManager();
                auto cfgAcceptor           = GetCfgAcceptor();
                auto cfgDistance           = GetCfgDistance();

                if(vid1 == vid2 || (RandomUniformReal() > GetProbAllowCycles() && graph->AreVerticesPathConnected(vid1, vid2)))
                        return false;

                PlannerVertex *v1 = graph->GetVertex(vid1);
                PlannerVertex *v2 = graph->GetVertex(vid2);

                if(v1->GetConnectionAttempts()->find(vid2) != v1->GetConnectionAttempts()->end())
                        return false;
                v1->GetConnectionAttempts()->insert(vid2);
                v2->GetConnectionAttempts()->insert(vid1);

                const double tstep = GetOneStepDistance() / d;
                Cfg         *cfg   = cfgManager->NewCfg();
                PlannerEdge *edge  = NewEdge();
        
                edge->SetFromToVertexIds(vid1, vid2);

                cfgOffspringGenerator->SetParentCfg(v1->GetCfg());
                if(dynamic_cast<CfgOffspringGeneratorTowardTarget*>(cfgOffspringGenerator))
                        dynamic_cast<CfgOffspringGeneratorTowardTarget*>(cfgOffspringGenerator)->SetTargetCfg(v2->GetCfg());
        
                // MMC Acceptors require the starting configuration
                if (dynamic_cast<CfgAcceptorBasedOnMMC*>(cfgAcceptor))
                        dynamic_cast<CfgAcceptorBasedOnMMC*>(cfgAcceptor)->SetSourceCfg(v1->GetCfg());

                if (dynamic_cast<CfgAcceptorBasedOnFixedMMC*>(cfgAcceptor))
                        dynamic_cast<CfgAcceptorBasedOnFixedMMC*>(cfgAcceptor)->SetSourceCfg(v1->GetCfg());

                if(dynamic_cast<CfgOffspringGeneratorToTarget*>(cfgOffspringGenerator))
                {
                        for(double t = tstep; t < 1; t += tstep)
                        {
                        cfgOffspringGenerator->SetStep(t);
                        cfgOffspringGenerator->GenerateOffspringCfg(*cfg);
                        if(cfgAcceptor->IsAcceptable(*cfg) == false)
                        {
                                cfgManager->DeleteCfg(cfg);
                                delete edge;
                                return false;
                        }
                        edge->GetIntermediateCfgs()->push_back(cfg);
        
                        if (dynamic_cast<CfgAcceptorBasedOnMMC*>(cfgAcceptor))
                                        dynamic_cast<CfgAcceptorBasedOnMMC*>(cfgAcceptor)->SetSourceCfg(cfg);
                        if (dynamic_cast<CfgAcceptorBasedOnFixedMMC*>(cfgAcceptor))
                                        dynamic_cast<CfgAcceptorBasedOnFixedMMC*>(cfgAcceptor)->SetSourceCfg(cfg);

                        cfg = cfgManager->NewCfg();
        
                        }
                        cfgManager->DeleteCfg(cfg);
                }
                else
                {
                        bool connected = false;

                        cfgOffspringGenerator->SetStep(tstep);
                        for(double t = tstep; t < 1 && !connected; t += tstep)
                        {
                        cfgOffspringGenerator->GenerateOffspringCfg(*cfg);
                        if(cfgAcceptor->IsAcceptable(*cfg) == false)
                        {
                                cfgManager->DeleteCfg(cfg);
                                delete edge;
                                return false;
                        }
                        edge->GetIntermediateCfgs()->push_back(cfg);
                        cfgOffspringGenerator->SetParentCfg(cfg);

                        connected = cfgDistance->Distance(*cfg, *(v2->GetCfg())) <= GetOneStepDistance();

                        cfg = cfgManager->NewCfg();
                        }
                        cfgManager->DeleteCfg(cfg);

                        if(!connected)
                        {
                        delete edge;
                        return false;
                        }
                }

                auto costs = GetEdgeCostEvaluator()->EvaluateGivenFromToAndIntermediateCfgs(*(v1->GetCfg()), *(v2->GetCfg()), *(edge->GetIntermediateCfgs()));
                edge->SetCosts(costs.m_costFromTo, costs.m_costToFrom);
                graph->AddEdge(edge);
                return true;
    }
    
}
