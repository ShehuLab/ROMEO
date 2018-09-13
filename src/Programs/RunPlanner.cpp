#include "Setup/Setup.hpp"
#include "PluginRosetta/SetupRosetta.hpp"
#include "Utils/Misc.hpp"
#include "Utils/Logger.hpp"
#include "Utils/Timer.hpp"
#include "Utils/Stats.hpp"
#include "Utils/DLHandler.hpp"
#include <iostream>
#include <fstream>

#include <Components/CfgAcceptors/CfgAcceptorBasedOnDistance.hpp>

using namespace Antipatrea;

/**
 *@author Erion Plaku, Amarda Shehu
 *@brief Setup the planning problem, planner and its components and then run the planner.
 *
 *@remarks
 *  - User provides a parameter file which specifies
 *     - the planning problem, the planner components, and the planner that should be used;
 *     - parameter values for the components and the planner;
 *     - name of setup that should be used, e.g., SetupPoint2D, SetupChain2D;
 *     - see params.txt for an example.
 *  - The function
 *     - create the planner and components based on the parameter file;
 *     - runs the planner and append solution statistics to the statistics file;
 *     - prints out the solution obtained by the planner.
 */
extern "C" int RunPlanner(int argc, char **argv)
{
    if(argc < 2)
        Logger::m_out << "usage: ./bin/Runner RunPlanner fnameParams" << std::endl << std::endl;
    
    Timer::Clock clk;
    
    Params *params = Params::GetSingleton();
    
    if(argc >= 2)
    {
		params->ReadFromFile(argv[1]);
		Logger::m_out << "begin parameters (as provided in the input file)" << std::endl;
		params->Print(Logger::m_out);
		Logger::m_out << "end parameters (as provided in the input file)" << std::endl << std::endl;
    }
    
    Setup      *setup         = NULL;
    const char *setupName     = params->GetValue(Constants::KW_UseSetup, Constants::KW_SetupPoint2D);
    std::string fullName      = (std::string) "Create" + setupName;
    auto        fnCreateSetup = (CreateSetupFn) DLHandler::GetSymbol(fullName.c_str());
    
    if(fnCreateSetup)
		setup = fnCreateSetup();
    else
    {
		Logger::m_out << "error RunPlanner: unknown setup <" << setupName <<">" << std::endl;
		return 0;
    }
    
    setup->Prepare(*params);

    
    Logger::m_out << "begin parameters (possibly extended by " << setupName << ")" << std::endl;
    params->Print(Logger::m_out);
    Logger::m_out << "end parameters (possibly extended by " << setupName << ")" << std::endl << std::endl;

    Logger::m_out << "begin CheckSetup" << std::endl;
    setup->CheckSetup();
    Logger::m_out << "end CheckSetup" << std::endl << std::endl;

    Logger::m_out << "begin Info" << std::endl;
    setup->Info("  ");
    Logger::m_out << "end Info" << std::endl << std::endl;
    
    const char *statsFileName = Constants::VAL_RunPlanner_StatsFile;
    int         maxNrRuns     = Constants::VAL_RunPlanner_MaxNrRuns;
    int         maxNrFailures = Constants::VAL_RunPlanner_MaxNrFailures;
    double      tmax          = Constants::VAL_RunPlanner_MaxRuntime;
    int         tint          = Constants::VAL_RunPlanner_IntervalRuntime;
    auto        data          = params->GetData(Constants::KW_RunPlanner);

    if(data && data->m_params)
    {
		statsFileName = data->m_params->GetValue(Constants::KW_UseStatsFile, statsFileName);
		maxNrRuns     = data->m_params->GetValueAsInt(Constants::KW_MaxNrRuns, maxNrRuns);
		maxNrFailures = data->m_params->GetValueAsInt(Constants::KW_MaxNrFailures, maxNrFailures);
		tmax          = data->m_params->GetValueAsDouble(Constants::KW_MaxRuntime, tmax);
		tint          = data->m_params->GetValueAsDouble(Constants::KW_IntervalRuntime, tint);
}
    
    
    int           nrRuns        = 0;
    int           nrFailures    = 0;
    int           solved        = 0;
    double        trun          = 0.0;
    double        cost          = -1.0;
    
    Logger::m_out << "StatsFile       " << statsFileName << std::endl
                  << "MaxNrRuns       " << maxNrRuns << std::endl
                  << "MaxNrFailures   " << maxNrFailures << std::endl
                  << "MaxRuntime      " << tmax << std::endl
                  << "IntervalRuntime " << tint << std::endl;
    
    std::fstream fs(statsFileName, std::fstream::in);

    if(fs.is_open())
    {
                while(fs >> solved >> trun >> cost)
                {
                        ++nrRuns;
                        if(solved == 0)
                        ++nrFailures;
                }
                fs.close();
    }
    
    Logger::m_out << "NrRuns         " << nrRuns << std::endl
                  << "NrFailures     " << nrFailures << std::endl << std::endl;
    
    if((maxNrRuns >= 0 && nrRuns >= maxNrRuns) || nrFailures >= maxNrFailures)
    {
		if(nrRuns >= maxNrRuns)
				Logger::m_out << "...planner has already been run " << nrRuns <<" times" << std::endl;
		else
				Logger::m_out << "...planner has failed to solve " << nrFailures << " instances. What's the point?" << std::endl;
		return 0;
    }

    auto *planner = setup->GetSamplingBasedPlanner();
    
    Logger::m_out << "...run " << (1 + nrRuns) << "/" <<  maxNrRuns << std::endl;
    
    Timer::Start(clk);
    planner->Start();
    const double tstart = Timer::Elapsed(clk);
    Stats::GetSingleton()->AddValue(Constants::KW_Runtime_PlannerStart, tstart);
    Logger::m_out <<  "...planner initialized [solved = " << planner->IsSolved() << "]" << std::endl;

    
    bool        readGraphWhenPlannerStarts = false;
    bool        printGraphWhenPlannerEnds  = false;
    const char *plannerGraphFile           = Constants::VAL_PlannerGraphFile;

    data = params->GetData(Constants::KW_PlannerGraph);
    if(data && data->m_params)
    {
		readGraphWhenPlannerStarts = data->m_params->GetValueAsBool(Constants::KW_ReadWhenPlannerStarts, readGraphWhenPlannerStarts);
		printGraphWhenPlannerEnds  = data->m_params->GetValueAsBool(Constants::KW_PrintWhenPlannerEnds,  printGraphWhenPlannerEnds);
		plannerGraphFile           = data->m_params->GetValue(Constants::KW_UseFile, plannerGraphFile);
    }

    if(readGraphWhenPlannerStarts)
    {
		Logger::m_out << "...reading planner graph from file " << plannerGraphFile << std::endl;
		planner->ReadFromFile(plannerGraphFile);
		Logger::m_out << "...done" << std::endl;
    }
        

    Timer::Start(clk);
    while(Timer::Elapsed(clk) < tmax && planner->IsSolved() == false)
    {
		Logger::m_out << "...running planner for " << tint << " seconds [tmax = " << tmax << "]" << std::endl;

		planner->Solve(tint);

		Logger::m_out << "...not done" << std::endl
						  << "[nrVertices    = " << planner->GetPlannerGraph()->GetNrVertices() << "] " << std::endl
						  << "[nrEdges       = " << planner->GetPlannerGraph()->GetNrEdges() << "] " << std::endl
						  << "[nrComponents  = " << planner->GetPlannerGraph()->GetComponents()->GetNrComponents() << "]" << std::endl
						  << "[runtime       = " << Timer::Elapsed(clk) << "]" << std::endl;
		auto goalAcceptor = planner->GetPlannerProblem()->GetGoalAcceptor();

		if (dynamic_cast<CfgAcceptorBasedOnDistance *>(goalAcceptor))
		{
			auto distGoalAcceptor = dynamic_cast<CfgAcceptorBasedOnDistance *>(goalAcceptor);
			Logger::m_out << "[nearestToGoal = " << distGoalAcceptor->GetMinDistanceAboveThresolhold() << std::endl;
		}

		Logger::m_out << std::endl;
    }
    trun = Timer::Elapsed(clk);
    Stats::GetSingleton()->AddValue(Constants::KW_Runtime_PlannerSolve, trun - tstart);

    if(planner->IsSolved())
    {
		planner->GetSolution(*(setup->GetPlannerSolution()));
		solved = 1;
		cost   = setup->GetPlannerSolution()->GetCost();
    }
    else
    {
		solved = 0;
		cost   = -1;
    }

    Logger::m_out << "...done [solved = " << solved <<"] [trun = " << trun << "] [cost = " << cost << "]" << std::endl << std::endl;
    fs.open(statsFileName, std::fstream::app);
    if(fs.is_open())
    {
		OutputFormat(fs);
		fs << solved << " " << trun << " " << cost << std::endl;
		fs.close();
    }
    
    if (dynamic_cast<SetupRosetta*>(setup))
            dynamic_cast<SetupRosetta*>(setup)->PlannerCfgs();

    std::string cmd(statsFileName);
    cmd += "_extended";
    std::ofstream out(cmd, std::ios::app);
    OutputFormat(out);
    Stats::GetSingleton()->Print(out);
    out.close();
    
    Logger::m_out << "...summary stats written to " <<  statsFileName << std::endl
                  << "...extended stats written to " << cmd << std::endl;

    cmd = ((std::string) statsFileName) + "_sol" + std::to_string(nrRuns);
    setup->GetPlannerSolution()->PrintToFile(cmd.c_str());
    
    Logger::m_out << "...solution written to " << cmd << std::endl;

    if(printGraphWhenPlannerEnds)
    {
		Logger::m_out << "...writing planner graph to file " << plannerGraphFile << std::endl;
		planner->PrintToFile(plannerGraphFile);
		Logger::m_out << "...done" << std::endl;
    }
        

    delete setup;
    
    return 0;
}
