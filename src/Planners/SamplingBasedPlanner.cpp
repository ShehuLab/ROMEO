#include "Planners/SamplingBasedPlanner.hpp"
#include "Components/CfgOffspringGenerators/CfgOffspringGeneratorToTarget.hpp"
#include "Utils/Timer.hpp"
#include "Utils/Stats.hpp"
#include <iostream>
namespace Antipatrea
{
    void SamplingBasedPlanner::Start(void)
    {
        m_proximityAuxCfg = GetCfgManager()->NewCfg();

        m_vidInit = AddVertex(GetCfgManager()->CopyCfg(*(GetPlannerProblem()->GetInitialCfg())));

        m_vidsGoal.clear();
        // auto cfgGoal = GetPlannerProblem()->GetGoalAcceptor()->GetAnAcceptableCfg();
        // if(cfgGoal != NULL)
        //     AddVertex(GetCfgManager()->CopyCfg(*cfgGoal));
    }
    
    bool SamplingBasedPlanner::IsSolved(void)
    {
        if(m_vidInit < 0)
            return false;

        for(int i = m_vidsGoal.size() - 1; i >= 0; --i)
            if(m_vidsGoal[i] >= 0 &&
              GetPlannerGraph()->AreVerticesPathConnected(m_vidInit, m_vidsGoal[i]))
                return true;
        return false;
    }
    
    bool SamplingBasedPlanner::GetSolution(PlannerSolution & sol)
    {
        m_graphSearchInfo.SetPlannerGraph(GetPlannerGraph());
        m_graphSearch.SetInfo(&m_graphSearchInfo);

        int        vidGoalFound = Constants::ID_UNDEFINED;
        const bool breakEarly   = false;

        if(m_graphSearch.AStar(m_vidInit, breakEarly, vidGoalFound))
        {
            sol.SetCost(m_graphSearch.GetPathCostFromStart(vidGoalFound));
            m_graphSearch.GetPathFromStart(vidGoalFound, *(sol.GetVertexSequence()));
            FromVertexSequenceToCfgs(sol);
            return true;
        }
        return false;
    }
    
    double SamplingBasedPlanner::ProximityDistFn(const int vid1, const int vid2, SamplingBasedPlanner * sbPlanner)
    {
        const Cfg *cfg1 = vid1 >= 0 ?
            sbPlanner->GetPlannerGraph()->GetVertex(vid1)->GetCfg() :
            sbPlanner->m_proximityAuxCfg;

        const Cfg *cfg2 = vid2 >= 0 ?
            sbPlanner->GetPlannerGraph()->GetVertex(vid2)->GetCfg() :
            sbPlanner->m_proximityAuxCfg;

        return sbPlanner->GetCfgDistance()->Distance(*cfg1, *cfg2);
    }

    int SamplingBasedPlanner::AddVertex(Cfg * const cfgNew)
    {
        PlannerVertex *vnew = NewVertex();
        vnew->SetCfg(cfgNew);

        const int vidNew = GetPlannerGraph()->AddVertex(vnew);
        auto goalCfg = GetPlannerProblem()->GetGoalAcceptor()->GetAnAcceptableCfg();
        double dist = GetCfgDistance()->Distance(*cfgNew,*goalCfg);

        m_proximityDataStructure.AddKey(vidNew);

        if(GetPlannerProblem()->GetGoalAcceptor()->IsAcceptable(*cfgNew))
        {
            m_vidsGoal.push_back(vidNew);
            vnew->MarkAsGoal(true);
        }
        else
            vnew->MarkAsGoal(false);

        return vidNew;
    }

    
    std::ostream& SamplingBasedPlanner::Print(std::ostream & out) const
    {
        auto  graph      = GetPlannerGraph();
        auto  cfgManager = GetCfgManager();
        
        const int nv = graph->GetNrVertices();
        
        out << nv << std::endl;
        for(int i = 0; i < nv; ++i)
            cfgManager->PrintCfg(out, *graph->GetVertex(i)->GetCfg());

        const int ne = graph->GetNrEdges();
        auto edges = graph->GetEdges();

        out << ne << std::endl;
        for(auto & iter : *edges)
        {
            auto edge = iter.second;
            out << edge->GetVertexId(PlannerEdge::INDEX_FROM) << " "
                << edge->GetVertexId(PlannerEdge::INDEX_TO) << " "
                << edge->GetCost(PlannerEdge::INDEX_FROM_TO) << " "
                << edge->GetVertexId(PlannerEdge::INDEX_TO_FROM) << std::endl;

            auto cfgs = edge->GetIntermediateCfgs();
            out << cfgs->size() << std::endl;
            for(auto & cfg : *cfgs)
                cfgManager->PrintCfg(out, *cfg);
            
        }
        
        return out;
    }
    
    
    std::istream& SamplingBasedPlanner::Read(std::istream & in)
    {
        auto graph      = GetPlannerGraph();
        auto cfgManager = GetCfgManager();

        std::unordered_map<int, int> map;
        int                          vid;
        int                          vidFrom;
        int                          vidTo;
        int                          vidFromNew;
        int                          vidToNew;
        int                          nv;
        int                          ne;
        int                          nrInter;
        Cfg                         *cfg;
        int                          nrAdded = 0;
        PlannerEdge                 *edge;
        double                       costFromTo;
        double                       costToFrom;
        EdgeCostEvaluator::Costs     costs;

        if(!(in >> nv))
        {
            Logger::m_out << "error SamplingBasedPlanner::Read : could not read number of cfgs" << std::endl;
            return in;
        }

        for(int i = 0; i < nv; ++i)
        {
            cfg = cfgManager->NewCfg();
            cfgManager->ReadCfg(in, *cfg);
            if(!in.good())
            {
                Logger::m_out << "error SamplingBasedPlanner::Read : could not read the " << i << "th cfg out of " << nv << " cfgs" << std::endl;
                return in;
            }

            vid = FindCfg(*cfg);
            if(vid >= 0)
                Logger::m_out << "warning SamplingBasedPlanner::Read : cfg " << i << "/" << nv << " already in planner graph (same as cfg " << vid << ")" << std::endl;
            else
                vid = AddVertex(cfg);

            if(vid >= 0)
                map.insert(std::make_pair(i, vid));
        }

        if(!(in >> ne))
        {
            Logger::m_out << "error SamplingBasedPlanner::Read : could not read number of edges" << std::endl;
            return in;
        }

        for(int i = 0; i < ne; ++i)
        {
            if(! (in >> vidFrom >> vidTo))
            {
                Logger::m_out << "error SamplingBasedPlanner::Read : could not read vertex ids for edge " << i << " out of " << ne << " edges" << std::endl;
                return in;
            }

            Reader::ReadDouble(in, costFromTo);
            if(!in.good())
            {
                Logger::m_out << "error SamplingBasedPlanner::Read : could not read cost for edge " << vidFrom << " " << vidTo << std::endl;
                return in;
            }

            Reader::ReadDouble(in, costToFrom);
            if(!in.good())
            {
                Logger::m_out << "error SamplingBasedPlanner::Read : could not read reverse cost for edge " << vidFrom << " " << vidTo << std::endl;
                return in;
            }

            if(!(in >> nrInter))
            {
                Logger::m_out << "error SamplingBasedPlanner::Read : could not read number of intermediate cfgs for edge " << vidFrom << " " << vidTo << std::endl;
                return in;
            }

            edge = NewEdge();
            for(int j = 0; j < nrInter; ++j)
            {
                cfg = cfgManager->NewCfg();
                cfgManager->ReadCfg(in, *cfg);
                if(!in.good())
                {
                    Logger::m_out << "error SamplingBasedPlanner::Read : could not read the " << j << "th intermediate cfg for edge " << vidFrom << " " << vidTo << std::endl;
                    delete edge;
                    return in;
                }
                edge->GetIntermediateCfgs()->push_back(cfg);
            }

            auto from = map.find(vidFrom);
            if(from != map.end())
            {
                auto to = map.find(vidTo);
                if(to != map.end())
                {
                    vidFromNew = from->second;
                    vidToNew   = to->second;

                    if(graph->FindEdge(vidFromNew, vidToNew) == NULL)
                    {
                        edge->SetFromToVertexIds(vidFromNew, vidToNew);
                        if(costFromTo == INFINITY || costToFrom == INFINITY)
                        {
                            costs = GetEdgeCostEvaluator()->EvaluateGivenFromToAndIntermediateCfgs(*(GetPlannerGraph()->GetVertex(vidFromNew)->GetCfg()),
                                                                                               *(GetPlannerGraph()->GetVertex(vidToNew)->GetCfg()),
                                                                                               *(edge->GetIntermediateCfgs()));
                            if(costFromTo == INFINITY)
                                costFromTo = costs.m_costFromTo;
                            else
                                costToFrom = costs.m_costFromTo;
                        }
                        edge->SetCosts(costToFrom, costToFrom);
                        GetPlannerGraph()->AddEdge(edge);
                        edge = NULL;
                    }
                }
            }
            if(edge)
                delete edge;

        }
    }
    
    int SamplingBasedPlanner::FindCfg(Cfg & cfg)
    {
        ProximityQuery<int> query;
        query.SetKey(Constants::ID_UNDEFINED);
        query.SetNrNeighbors(1);
        query.SetRange(Constants::EPSILON);
        GetCfgManager()->CopyCfg(*m_proximityAuxCfg, cfg);

        return m_proximityDataStructure.Neighbor(query);
    }

    void SamplingBasedPlanner::FromVertexSequenceToCfgs(PlannerSolution & sol)
    {
        auto      seq = sol.GetVertexSequence();
        const int n   = seq->size();
        int       vidFrom;
        int       vidTo;

        sol.ClearCfgs();
        for(int i = 0; i < n - 1; ++i)
        {
            vidFrom    = (*seq)[i];
            vidTo      = (*seq)[i + 1];
            auto edge  = GetPlannerGraph()->FindEdge(vidFrom, vidTo);
            auto inter = edge->GetIntermediateCfgs();
            const int nrInter = inter->size();

            sol.AddCfg(GetCfgManager()->CopyCfg(*(GetPlannerGraph()->GetVertex(vidFrom)->GetCfg())));
            if(edge->GetVertexId(PlannerEdge::INDEX_FROM) == vidFrom)
            for(int j = 0; j < nrInter; ++j)
                    sol.AddCfg(GetCfgManager()->CopyCfg(*(*inter)[j]));
            else
            for(int j = nrInter - 1; j >= 0; --j)
                    sol.AddCfg(GetCfgManager()->CopyCfg(*(*inter)[j]));
        }
    }
}
