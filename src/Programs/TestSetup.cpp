#include "Setup/Setup.hpp"
#include "Utils/Misc.hpp"
#include "Utils/Logger.hpp"
#include "Utils/Timer.hpp"
#include "Utils/Stats.hpp"
#include "Utils/DLHandler.hpp"
#include <iostream>
#include <fstream>

using namespace Antipatrea;

/**
 *@author Erion Plaku, Amarda Shehu
 *@brief Setup the planning problem, planner and its components and then test them.
 *
 *@remarks
 *  - User provides a parameter file which specifies
 *     - the planning problem, the planner components, and the planner that should be used;
 *     - parameter values for the components and the planner;
 *     - name of setup that should be used, e.g., SetupPoint2D, SetupChain2D;
 *     - see params.txt for an example.
 *  - The function
 *     - create the planner and components based on the parameter file;
 *  - User should add code to test the different components.
 */
extern "C" int TestSetup(int argc, char **argv)
{
    if(argc < 2)
	Logger::m_out << "usage: ./bin/Runner TestSetup fnameParams" << std::endl << std::endl;
    
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
	Logger::m_out << "error TestSetup: unknown setup <" << setupName <<">" << std::endl;
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
    
    Logger::m_out << "...running test function from setup" << std::endl;
    setup->Test();
    Logger::m_out << "...concluded test function from setup" << std::endl;
    
    delete setup;
    
    return 0;
}
