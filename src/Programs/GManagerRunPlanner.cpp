#include "Programs/GManagerRunPlanner.hpp"
#include "Utils/Constants.hpp"
#include "Utils/Algebra2D.hpp"
#include "Utils/Timer.hpp"
#include "Utils/Stats.hpp"
#include "Utils/Logger.hpp"

namespace Antipatrea
{
    void GManagerRunPlanner::SetupFromParams(Params & params)
    {
	GManager::SetupFromParams(params);
	auto data = params.GetData(Constants::KW_PlannerGraph);
	if(data && data->m_params)
	    m_plannerGraphFile = data->m_params->GetValue(Constants::KW_UseFile, m_plannerGraphFile);
    }
    

    GManagerRunPlanner::GManagerRunPlanner(void) : GManager(),
						   m_flags(0)
    {
	m_plannerGraphFile = Constants::VAL_PlannerGraphFile;
	
	m_setup = NULL;
	
	m_solutionIndex    = Constants::ID_UNDEFINED;
	m_idMenuMain       = Constants::ID_UNDEFINED;
	m_idMenuPlanner    = Constants::ID_UNDEFINED;
	
	MENU_ITEM_PLANNER_START              = ++m_nrMenuItems;
	MENU_ITEM_PLANNER_SOLVE              = ++m_nrMenuItems;
	MENU_ITEM_PLANNER_PRINT_GRAPH        = ++m_nrMenuItems;
	MENU_ITEM_PLANNER_READ_GRAPH         = ++m_nrMenuItems;
	MENU_ITEM_PLANNER_STATS              = ++m_nrMenuItems;
	
	
	m_mapKeysToMenuItems.insert(std::make_pair((int) '1', MENU_ITEM_PLANNER_START));
	m_mapKeysToMenuItems.insert(std::make_pair((int) '2', MENU_ITEM_PLANNER_SOLVE));
	m_mapKeysToMenuItems.insert(std::make_pair((int) '3', MENU_ITEM_PLANNER_PRINT_GRAPH));
	m_mapKeysToMenuItems.insert(std::make_pair((int) '4', MENU_ITEM_PLANNER_READ_GRAPH));
	m_mapKeysToMenuItems.insert(std::make_pair((int) '5', MENU_ITEM_PLANNER_STATS));
    } 
	
    void GManagerRunPlanner::HandleEventOnDisplay(void)
    {
	GManager::HandleEventOnDisplay();
	
	
	GDrawColor(0.4, 0.4, 0.4);
	for(int i = m_cfgs.size() - 1; i >= 0; --i)
	    DrawCfg(*(m_cfgs[i]));
	
	//draw planner graph
	PlannerGraph *g = m_setup->GetSamplingBasedPlanner()->GetPlannerGraph();
	const int     nv= g->GetNrVertices();
	for(int i = 0; i < nv; ++i)
	    DrawCfg(*(g->GetVertex(i)->GetCfg()));
	
	GDrawLineWidth(1.0);
	GDrawColor(1.0, 0.0, 0.0);
	for(int i = 0; i < nv; ++i)
	{
	    auto v     = g->GetVertex(i);
	    auto conns = v->GetConnections();
	    
	    for(auto & vidTo : *conns)
	    {
		if(i < vidTo)
		    DrawEdge(*(v->GetCfg()),
			     *(g->GetVertex(vidTo)->GetCfg()));
	    }
	}

	GDrawColor(0.0, 1.0, 0.0);
	DrawCfg(*(m_setup->GetSamplingBasedPlanner()->GetPlannerProblem()->GetInitialCfg()));
	
	GDrawColor(1.0, 0.0, 0.0);
	auto cfg = m_setup->GetSamplingBasedPlanner()->GetPlannerProblem()->GetGoalAcceptor()->GetAnAcceptableCfg();
	if(cfg)
	    DrawCfg(*cfg);

	if(m_solutionIndex >= 0 && m_solutionIndex < m_setup->GetPlannerSolution()->GetNrCfgs())
	{
	    GDrawColor(1.0, 0.0, 1.0);
	    DrawCfg(*(m_setup->GetPlannerSolution()->GetCfg(m_solutionIndex)));
	}
	
    }
    
    bool GManagerRunPlanner::HandleEventOnNormalKeyPress(const int key)
    {
	auto cur = m_mapKeysToMenuItems.find(key);
	if(cur != m_mapKeysToMenuItems.end())
	    HandleEventOnMenu(cur->second);
	else if((key == '+' || key == '-') && m_setup->GetPlannerSolution()->GetNrCfgs() > 0)
	{
	    if(key == '+')
		++m_solutionIndex;
	    else
		--m_solutionIndex;
	    
	    if(m_solutionIndex < 0)
		m_solutionIndex = 0;
	    else if(m_solutionIndex >= m_setup->GetPlannerSolution()->GetNrCfgs())
		m_solutionIndex = m_setup->GetPlannerSolution()->GetNrCfgs() - 1;
	}
	else if(key == 'z' && m_setup->GetPlannerSolution()->GetNrCfgs() > 0)
	    m_solutionIndex = 0;
	else if(key == 'p')
	    m_flags = FlipFlag(m_flags, FLAG_PAUSE);
	
	
	return GManager::HandleEventOnNormalKeyPress(key);	    
    }
    
    
    bool GManagerRunPlanner::HandleEventOnMenu(const int item)
    {
	Timer::Clock clk;
	int          solved;
	double       cost;
	double       trun;
	
	m_menuSelItem = item;
	if(item == MENU_ITEM_PLANNER_START)
	{
	    Timer::Start(clk);
	    m_setup->GetSamplingBasedPlanner()->Start();
	    Stats::GetSingleton()->AddValue(Constants::KW_Runtime_PlannerStart, Timer::Elapsed(clk));
	    Logger::m_out <<  "...planner initialized [solved = " << m_setup->GetSamplingBasedPlanner()->IsSolved() << "]" << std::endl;
	    
	    return true;
	}
	else if(item == MENU_ITEM_PLANNER_SOLVE && m_setup->GetSamplingBasedPlanner()->IsSolved() == false)
	{
	    Logger::m_out << "...running planner for " << Constants::VAL_RunPlanner_IntervalRuntime << " seconds" << std::endl;
	    
	    Timer::Start(clk);
	    m_setup->GetSamplingBasedPlanner()->Solve(Constants::VAL_RunPlanner_IntervalRuntime);
	    Stats::GetSingleton()->AddValue(Constants::KW_Runtime_PlannerSolve, Timer::Elapsed(clk));

	    if(m_setup->GetSamplingBasedPlanner()->IsSolved())
	    {
	   	m_setup->GetSamplingBasedPlanner()->GetSolution(*(m_setup->GetPlannerSolution()));
		solved = 1;
		cost   = m_setup->GetPlannerSolution()->GetCost();
	    }
	    else
	    {
		solved = 0;
		cost   = -1;
	    }
	    trun =
		Stats::GetSingleton()->GetValue(Constants::KW_Runtime_PlannerStart) +
		Stats::GetSingleton()->GetValue(Constants::KW_Runtime_PlannerSolve);
	    
	    Logger::m_out << "[nrVertices   = " << m_setup->GetSamplingBasedPlanner()->GetPlannerGraph()->GetNrVertices() << "] " << std::endl
			  << "[nrEdges      = " << m_setup->GetSamplingBasedPlanner()->GetPlannerGraph()->GetNrEdges() << "] " << std::endl
			  << "[nrComponents = " << m_setup->GetSamplingBasedPlanner()->GetPlannerGraph()->GetComponents()->GetNrComponents() << "]" << std::endl
			  << "[solved       = " << solved << "]" << std::endl
		          << "[runtime      = " << trun << "]" << std::endl
		          << "[cost         = " << cost << "]" << std::endl;
	    
	    return true;
	}
	else if(item == MENU_ITEM_PLANNER_PRINT_GRAPH)
	{
	    Logger::m_out << "...begin writing planner graph to file <" << m_plannerGraphFile << ">" << std::endl;
	    m_setup->GetSamplingBasedPlanner()->PrintToFile(m_plannerGraphFile);
	    Logger::m_out << "...done" << std::endl;
	}	
	else if(item == MENU_ITEM_PLANNER_READ_GRAPH)
	{
	    Logger::m_out << "...begin reading planner graph from file <" << m_plannerGraphFile << ">" << std::endl;
	    m_setup->GetSamplingBasedPlanner()->ReadFromFile(m_plannerGraphFile);
	    Logger::m_out << "...done" << std::endl;
	}
	else if(item == MENU_ITEM_PLANNER_STATS)
	    Stats::GetSingleton()->Print(Logger::m_out);
	else
	    return GManager::HandleEventOnMenu(item);
    }
    
    
    void GManagerRunPlanner::HandleEventOnTimer(void)
    {
	if(HasFlag(m_flags, FLAG_PAUSE))
	    return;
	
	if(m_setup->GetPlannerSolution()->GetNrCfgs() > 0)
	{
	    ++m_solutionIndex;
	    if(m_solutionIndex >= m_setup->GetPlannerSolution()->GetNrCfgs())
		m_solutionIndex = m_setup->GetPlannerSolution()->GetNrCfgs() - 1;
	}
    }

    void GManagerRunPlanner::MenuOptions(void)
    {
	m_idMenuPlanner = CreateMenu();
	AddMenuEntry("1. Start", MENU_ITEM_PLANNER_START);
	AddMenuEntry("2. Solve", MENU_ITEM_PLANNER_SOLVE);
	AddMenuEntry("3. Print planner graph", MENU_ITEM_PLANNER_PRINT_GRAPH);
	AddMenuEntry("4. Read planner graph", MENU_ITEM_PLANNER_READ_GRAPH);
	AddMenuEntry("5. Display stats", MENU_ITEM_PLANNER_STATS);
	
	m_idMenuMain = CreateMenu();
	AddSubMenu("Planner", m_idMenuPlanner);
    }
}

