#include "Plugin2D/GManagerRunPlannerPoint2D.hpp"
#include "Plugin2D/GManagerRunPlannerChain2D.hpp"
#include "Utils/Logger.hpp"
#include <iostream>

using namespace Antipatrea;

/**
 *@author Erion Plaku, Amarda Shehu
 *@brief Setup the planning problem, planner and its components and then run the planner
 *       using a graphical interface
 *
 *@remarks
 *  - User provides a parameter file which specifies
 *     - the planning problem, the planner components, and the planner that should be used;
 *     - parameter values for the components and the planner;
 *     - name of setup that should be used, e.g., SetupPoint2D, SetupChain2D;
 *     - see params.txt for an example.
 *  - The function creats the planner and components based on the parameter file.
 *  - It also provides a graphical interface to run the planner and view the solution.
 */
extern "C" int GRunPlanner(int argc, char **argv)
{
    if(argc < 2)
	Logger::m_out <<  "usage: ./bin/Runner GRunPlanner fnameParams" << std::endl << std::endl;

    Params *params = Params::GetSingleton();
    
    if(argc >= 2)
    {
	params->ReadFromFile(argv[1]);
	Logger::m_out << "begin parameters (as provided in the input file)" << std::endl;
	params->Print(Logger::m_out);
	Logger::m_out << "end parameters (as provided in the input file)" << std::endl << std::endl;
    }

    GManagerRunPlanner *gManager  = NULL;
    const char *setupName         = params->GetValue(Constants::KW_UseSetup, Constants::KW_SetupPoint2D);
    
    if(StrSameContent(setupName, Constants::KW_SetupChain2D))
	gManager = new GManagerRunPlannerChain2D();
    else
	gManager = new GManagerRunPlannerPoint2D();

    gManager->m_setup->Prepare(*params);
    gManager->SetupFromParams(*params);
  
    Logger::m_out << "begin parameters (possibly extended by " << setupName << ")" << std::endl;
    params->Print(Logger::m_out);
    Logger::m_out << "end parameters (possibly extended by " << setupName << ")" << std::endl << std::endl;
    
    Logger::m_out << "begin CheckSetup" << std::endl;
    gManager->m_setup->CheckSetup();
    Logger::m_out << "end CheckSetup" << std::endl << std::endl;
    
    
    Logger::m_out << "begin Info" << std::endl;
    gManager->m_setup->Info("  ");
    Logger::m_out << "end Info" << std::endl << std::endl;
    
    
    gManager->MainLoop("GRunPlanner", 
		      Antipatrea::Params::GetSingleton()->GetValueAsInt("GraphicsResolutionX", 1280),
		      Antipatrea::Params::GetSingleton()->GetValueAsInt("GraphicsResolutionY", 720));
 
    return 0;
}
