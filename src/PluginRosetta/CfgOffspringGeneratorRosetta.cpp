#include "PluginRosetta/CfgOffspringGeneratorRosetta.hpp"

#include <iostream>
#include <fstream>
#include "Utils/PseudoRandom.hpp"
#include "Utils/Algebra2D.hpp"

namespace Antipatrea
{
    void CfgOffspringGeneratorRosetta::SetupFromParams(Params & params)
    {
		CfgOffspringGenerator::SetupFromParams(params);
		std::cout << " getting parms for Rosetta generator" << std::endl;
 		auto data = params.GetData(Constants::KW_CfgOffspringGeneratorRosetta);
		if(data && data->m_params)
		{
			m_fragmentFileName = data->m_params->GetValue(Constants::KW_MolecularStructureRosetta_DBDir);
		}
		else
		{
			std::cout << "Error: no fragment database defined.\n";
			exit(12);
		}

		LoadFragmentLibrary();
		PrintSummary();
    }

    void CfgOffspringGeneratorRosetta::GenerateOffspringCfg(Cfg & cfg)
    {
		auto cfgManager = GetCfgManager();
		auto mol        = GetMolecularStructureRosetta();
		const int dim   = cfgManager->GetDim(); //number of configuration dimensions;
		double *vals    = cfg.GetValues();      //values of the configuration

		const int fragmentSize = 3;
		auto &db = m_fragmentMap[fragmentSize];

		auto sampleAAPosition = RandomUniformInteger(0,db.NumberOfPositions() - 1 - fragmentSize);

		auto fragmentIndex = RandomUniformInteger(0,db.NumberOfSamples(sampleAAPosition)-1);

		std::cout << "Sample position:" << sampleAAPosition << " with fragment index:" << fragmentIndex << "\n";

		auto pCfg =  GetParentCfg();
		auto pCfgVals = pCfg->GetValues();
		memcpy(vals,pCfgVals,sizeof(double)*dim);

		//parent configuration: GetParentCfg()
		//step size: GetStep() [as a measure of how far/different the offspring should be from its parent]

		//add code to do generate the offspring
		for (auto i=0; i < fragmentSize;++i) {
			vals[3*(sampleAAPosition+i) + 0] = db.GetFragmentPhi(sampleAAPosition,fragmentIndex,i);
			vals[3*(sampleAAPosition+i) + 1] = db.GetFragmentPsi(sampleAAPosition,fragmentIndex,i);
			vals[3*(sampleAAPosition+i) + 2] = db.GetFragmentOmega(sampleAAPosition,fragmentIndex,i);

/*			std::cout << "Plugging in angles phi:" <<  db.GetFragmentPhi(sampleAAPosition,fragmentIndex,i)
				 << " psi:" <<  db.GetFragmentPsi(sampleAAPosition,fragmentIndex,i)
				 << " omg:" <<  db.GetFragmentOmega(sampleAAPosition,fragmentIndex,i) << std::endl;
*/
		}

		//don't forget at the end to save
			//so that cfg remembers that the values have been changed.
			//In such cases, it sets the energy to undefined as an indication that it may need to be computed again.

		cfg.SetValues(vals);
    }

    //
    void CfgOffspringGeneratorRosetta::LoadFragmentLibrary()
    {
    	Logger::m_out << "Loading fragment file:" << m_fragmentFileName << std::endl;

    	std::ifstream fragmentFile (m_fragmentFileName);
    	FragmentDB db;
    	std::string fileLine;
    	const std::string positionStr = " position:";
    	unsigned int fragmentLength = 0;

    	unsigned int pos = 0;
    	unsigned int expectedNeighbors = 0;
    	unsigned int neighbor = 0;
    	std::vector<double> phi;
    	std::vector<double> psi;
    	std::vector<double> omega;
    	std::string pdbID;
    	unsigned int aaOffset;

    	while (std::getline(fragmentFile,fileLine))
    	{  // when switching positions, the line starts with "position: x , neighbors: x"
    	    if (fileLine.compare(0,positionStr.size(),positionStr) == 0)
    	    {
				unsigned int newPosition;
				unsigned int newNeighbors;
				std::sscanf(fileLine.c_str(), " position: %u neighbors: %u",&newPosition, &newNeighbors);
				assert(newPosition == (pos + 1));
				assert(neighbor == expectedNeighbors);
				pos = newPosition;
				expectedNeighbors = newNeighbors;
				neighbor = 0;
    	    }
    	    // else if its a blank line, we are switching fragments
    	    else if (fileLine.length() == 0)
    	    {
    	        if (phi.size() > 0)
    	        {
					if (fragmentLength == 0)
							fragmentLength = phi.size();
					else
						assert(fragmentLength == phi.size());

					RosettaFragment f(pdbID,aaOffset,phi,psi,omega);
					db.Insert(pos-1,f); // we store things 0 .. n-1

					phi.clear();
					psi.clear();
					omega.clear();
					++neighbor;
					pdbID.clear();
    	        }
     	    }
    	    // we are adding to the previous fragment
    	    else
    	    {
    	        phi.push_back(0.);
				psi.push_back(0.);
				omega.push_back(0.);
				std::string thisPdbId;

				std::string ss;
				std::string chain;
				std::string aa;
				unsigned int aaOffset;
				std::istringstream aaStream(fileLine);
				aaStream >> thisPdbId >> chain >> aaOffset >> aa
					 >> ss >> phi.back() >> psi.back() >> omega.back();
				phi.back() = Algebra2D::DegreesToRadians(phi.back());
				psi.back() = Algebra2D::DegreesToRadians(psi.back());
				omega.back() = Algebra2D::DegreesToRadians(omega.back());
				if (pdbID.length() == 0)
					pdbID = thisPdbId;
    	    }
    	}

    	m_fragmentMap[fragmentLength] = db;
    }

}

