#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

#include "Utils/Reader.hpp"
#include "Utils/Algebra2D.hpp"
// rosetta include files
#include <devel/init.hh>
#include <core/pose/Pose.hh>
#include <core/import_pose/import_pose.hh>
#include <core/scoring/ScoreFunction.hh>

/**
 *@author Kevin Molloy
 *@brief Take configuration solution file and translation each configuration into a PDB file.
 *@remarks
 *  - First argument is the name of the solution file to be read.
 */

using namespace Antipatrea;

void MakePDBFile(core::pose::Pose &p,unsigned int cfgNumber,
		 double angles[], double energyScore)
{
//	std::cout<< "-----------------------------" << std::endl;
	for (unsigned int r=0;r < p.total_residue();++r)
	{
//		std:: cout << "Setting res:" << r << " phi to:" << angles[r*3 + 0]  << " psi:" << angles[r*3 + 1] << " omg:" << angles[r*3 + 2] << std::endl;

		p.set_phi(r+1,Algebra2D::RadiansToDegrees(angles[r*3 + 0]));
		p.set_psi(r+1,Algebra2D::RadiansToDegrees(angles[r*3 + 1]));
		p.set_omega(r+1,Algebra2D::RadiansToDegrees(angles[r*3 + 2]));
	}
//	std::cout<< "-----------------------------" << std::endl;

	std::ostringstream ss;
	ss << std::setw(4) << std::setfill('0') << cfgNumber;
	std::string cfgNumberStr(ss.str());
	std::string pdbFileName = "/tmp/sol_pdb_" + cfgNumberStr + ".pdb";
	std::string energyFileName = "/tmp/sol_energyProfile.dat";
	//std::cout << "Writing PDB file:" << pdbFileName << std::endl;
	std::ofstream energyData(energyFileName,std::ios::app);
	energyData << energyScore << std::endl;
	energyData.close();
	p.dump_pdb(pdbFileName);
}

int main(int argc, char **argv)
{
    if(argc < 3)
    {
	std::cout << "usage: MakePDBsFromSolution <pdbModel> <solutionFile>" << std::endl;
	exit(99);
    }

     //initialize SetupRosetta
    char ** rosetta_argv = (char **) malloc(sizeof(char*) * 3);
    char DBDirCopy[350];
    // strcpy(DBDirCopy,DBDir);

    rosetta_argv[0] = (char*)"";
    rosetta_argv[1] = (char*)"-database";
    //rosetta_argv[2] = DBDirCopy;
    rosetta_argv[2] = (char*)"/home/kmolloy/biotools/rosetta_src_2017.08.59291_bundle/main/database";

    devel::init(3,rosetta_argv);
    std::cout <<"Rosetta initialized" << std::endl;

    core::pose::Pose p;
    core::import_pose::centroid_pose_from_pdb(p,argv[1]);

    unsigned int residueCount = p.total_residue();

    std::ifstream cfgFile(argv[2]);
    double solutionCost;
    Reader::ReadDouble(cfgFile, solutionCost);

    double vertexCount;
    Reader::ReadDouble(cfgFile, vertexCount);

    for (unsigned int i=0;i < vertexCount;++i)
    {
	    double dummy;
	    Reader::ReadDouble(cfgFile, dummy);
    }

    double cfgCount;
    Reader::ReadDouble(cfgFile, cfgCount);

    for (unsigned int i=0;i < cfgCount;++i)
    {
	    double angles[residueCount*3];
	    for (unsigned int r=0;r < residueCount*3;++r)
	    {
		    Reader::ReadDouble(cfgFile,angles[r]);
	    }
	    double energyScore;
	    Reader::ReadDouble(cfgFile,energyScore);
	    MakePDBFile(p,i,angles,energyScore);
    }
    std::cout << "wrote  " << cfgCount << " configurations/PDB files." << std::endl;
    return 0;
}
