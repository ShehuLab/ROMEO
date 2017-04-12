#include "Utils/DLHandler.hpp"
#include "Utils/PseudoRandom.hpp"
#include "Utils/Logger.hpp"
#include "Utils/Misc.hpp"

using namespace Antipatrea;

/**
 *@author Erion Plaku, Amarda Shehu
 *@brief Program to run other programs.
 *@remarks
 *  - First argument is the name of the program that should be run.
 *  - The other arguments are arguments to that program.
 *  - Runner looks for the name of the program in the list of the loaded symbols.
 *    - if it finds it, it runs the program passing along the arguments.
 *    - otherwise, it displays a warning message.
 *  - In order for Runner to be able to find the program, it should be of the form
 *   <center> <tt>extern "C" int ProgramName(int argc, char **argv)</tt></center>
 *   where <tt>ProgramName</tt> could be an valid function name.
 */
int main(int argc, char **argv)
{
    typedef int (*MainFcn) (int, char **);

    OutputFormat(Logger::m_out);

    RandomSeed();

    if(argc < 2)
	Logger::m_out << "usage: Runner <program_name> [list_of_program_args]" << std::endl;
    else
    {
	MainFcn fcn = (MainFcn) Antipatrea::DLHandler::GetSymbol(argv[1]);
	if(fcn)
	{
	    Logger::m_out << "Running program <" << argv[1] << ">" << std::endl << std::endl;
	    return fcn(argc - 1, &(argv[1]));
	}
	else
	    Logger::m_out << "Program <" << argv[1] << "> not found" << std::endl;
    }    

    return 0;
}
