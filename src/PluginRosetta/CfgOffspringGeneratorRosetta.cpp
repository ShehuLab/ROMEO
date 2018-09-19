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

        auto data = params.GetData(Constants::KW_CfgOffspringGeneratorRosetta);
        if(data && data->m_params)
        {
        	m_offspringToGenerate =
        	    data->m_params->GetValueAsInt(Constants::KW_OffspringRosetta_NumberToGenerate,
        	    		                Constants::VAL_OffspringRosetta_NumberToGenerate);

        	m_distanceTol =
        			data->m_params->GetValueAsInt(Constants::KW_OffspringRosetta_DistanceTol,
        			        	    		      Constants::VAL_OffspringRosetta_DistanceTol);
        	auto dataFragment
				= data->m_params->GetData(Constants::KW_OffspringRosetta_FragmentFiles);
        	if(dataFragment && dataFragment->m_values.size() > 0) {
                for(auto & val : dataFragment->m_values) {
                	m_fragmentFileNames.push_back(val->c_str());
                	std::cout << "Adding fragment file:" << m_fragmentFileNames.back() << std::endl;
                }
        	}
        	dataFragment = data->m_params->GetData(Constants::KW_OffspringRosetta_FragmentProbabilities);
        	if(dataFragment && dataFragment->m_values.size() > 0) {
				for(auto & val : dataFragment->m_values) {
					m_fragmentProbs.push_back(StrToDouble(val->c_str()));
					std::cout << "Adding fragment prob:" << m_fragmentProbs.back() << std::endl;
				}
			}
        }
        else
		{
            std::cout << "Error: no fragment database defined.\n";
            exit(12);
		}

        for (int i = 0;i < m_fragmentFileNames.size();++i)
        {
            unsigned int fragmentSize =
            		LoadFragmentLibrary(m_fragmentFileNames[i],m_fragmentProbs[i]);
            auto fragmentNode = m_selector.Create();
            fragmentNode->SetKey(fragmentSize);
            fragmentNode->SetWeight(m_fragmentProbs[i]);
            m_selector.Insert(fragmentNode);
        }
        PrintSummary();
    }

    // Generate an offspring using Rosetta that is "close" to the target
    // structure specified.

    double CfgOffspringGeneratorRosetta::GenerateAnOffspringCfg(Cfg & cfg)
    {
        auto cfgManager = GetCfgManager();
        auto mol        = GetMolecularStructureRosetta();
        const int dim   = cfgManager->GetDim(); //number of configuration dimensions;
        double *vals    = cfg.GetValues();      //values of the configuration

        auto fragmentNode = m_selector.Select();
        int fragmentSize = fragmentNode->GetKey();

        if (m_verboseFlag)
        {
        	std::cout<< "Fragment size selected is:" << fragmentSize << std::endl;
        }

        auto &db = m_fragmentMap[fragmentSize];

        auto sampleAAPosition = RandomUniformInteger(0,db.NumberOfPositions() - 1);
        auto fragmentIndex    = RandomUniformInteger(0,db.NumberOfSamples(sampleAAPosition)-1);

        if (m_verboseFlag)
        {
        	std::cout << "SAMPLE_pos:" << sampleAAPosition << ":findex:" << fragmentIndex
        			<< " maxposition:" << db.NumberOfPositions() - 1 << std::endl;
        }

        /*
                  << ":1phi:" << db.GetFragmentPhi(sampleAAPosition,fragmentIndex,0)
                  << ":1phideg:" << Algebra2D::RadiansToDegrees(db.GetFragmentPhi(sampleAAPosition,fragmentIndex,0))
                  << ":1psi:" << db.GetFragmentPsi(sampleAAPosition,fragmentIndex,0)
                  << ":1omg:" << db.GetFragmentOmega(sampleAAPosition,fragmentIndex,0)
                  << ":2phi:" << db.GetFragmentPhi(sampleAAPosition,fragmentIndex,1)
                  << ":2phideg:" << Algebra2D::RadiansToDegrees(db.GetFragmentPhi(sampleAAPosition,fragmentIndex,1))
                  << ":2psi:" << db.GetFragmentPsi(sampleAAPosition,fragmentIndex,1)
                  << ":2omg:" << db.GetFragmentOmega(sampleAAPosition,fragmentIndex,1)
                  << ":3phi:" << db.GetFragmentPhi(sampleAAPosition,fragmentIndex,2)
                  << ":3phideg:" << Algebra2D::RadiansToDegrees(db.GetFragmentPhi(sampleAAPosition,fragmentIndex,2))
                  << ":3psi:" << db.GetFragmentPsi(sampleAAPosition,fragmentIndex,2)
                  << ":3omg:" << db.GetFragmentOmega(sampleAAPosition,fragmentIndex,2)  << "\n"; */

        auto pCfg =  GetParentCfg();
        auto pCfgVals = pCfg->GetValues();
        memcpy(vals,pCfgVals,sizeof(double)*dim);


        for (auto i=0; i < fragmentSize;++i)
        {
            vals[3*(sampleAAPosition+i) + 0] = db.GetFragmentPhi(sampleAAPosition,fragmentIndex,i);
            vals[3*(sampleAAPosition+i) + 1] = db.GetFragmentPsi(sampleAAPosition,fragmentIndex,i);
            vals[3*(sampleAAPosition+i) + 2] = db.GetFragmentOmega(sampleAAPosition,fragmentIndex,i);

    /*      std::cout << "Plugging in angles phi:" <<  db.GetFragmentPhi(sampleAAPosition,fragmentIndex,i)
                      << " psi:" <<  db.GetFragmentPsi(sampleAAPosition,fragmentIndex,i)
                     << " omg:" <<  db.GetFragmentOmega(sampleAAPosition,fragmentIndex,i) << std::endl;
    */
        }

        cfg.SetValues(vals); //KMDEBUG Don't think this is necessary since the ptr to the double array has not changed

        auto d = GetCfgDistance();
        double dist = d->Distance(*m_cfgTarget,cfg);
        return (dist);
    }

    void CfgOffspringGeneratorRosetta::GenerateOffspringCfg(Cfg & cfg)
    {
        auto cfgManager = GetCfgManager();

        //don't forget at the end to save
        //so that cfg remembers that the values have been changed.
        //In such cases, it sets the energy to undefined as an indication that it may need to be computed again.
        Cfg *workCfg = cfgManager->NewCfg();

        double smallDist = 999999.99;
        unsigned int distanceTolRejected = 0;

        for (unsigned int i(0);i < m_offspringToGenerate ;++i)
        {
			double distToTarget = GenerateAnOffspringCfg(*workCfg);

			if (distToTarget < smallDist)
			{
				bool distanceOK = true;
				if (m_distanceTol > 0.0)
				{
				// measure dist from parent and accept if within tolerance
					auto d = GetCfgDistance();
					double distFromParent =
							d->Distance((*GetParentCfg()),*workCfg);

					if (distFromParent > m_distanceTol)
					{
						distanceOK = false;
						distanceTolRejected++;
					}
				}
				if (distanceOK)
				{
					cfgManager->CopyCfg(cfg,*workCfg);
					smallDist = distToTarget;
				}
			}
        }

        cfgManager->DeleteCfg(workCfg);
        std::cout << "OFFSPRING_DISTANCE_TOL_REJECT:" << distanceTolRejected << std::endl;

    }

    //
    unsigned int CfgOffspringGeneratorRosetta::LoadFragmentLibrary(std::string fragmentFileName,
    		                                                       double fragmentProbability)
    {
        Logger::m_out << "Loading fragment file:" << fragmentFileName << std::endl;

        std::ifstream fragmentFile (fragmentFileName);
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

                /* std::cout << "read in phi:" << phi.back() << ":psi:" << psi.back()
                                  << ":omg:" << omega.back(); */

                phi.back() = Algebra2D::DegreesToRadians(phi.back());
                psi.back() = Algebra2D::DegreesToRadians(psi.back());
                omega.back() = Algebra2D::DegreesToRadians(omega.back());
                /* std::cout << ":phirad:" << phi.back() << ":psirad:" << psi.back()
                                  << ":omgrad:" << omega.back() << "\n"; */

                if (pdbID.length() == 0)
                        pdbID = thisPdbId;
            }
        }

        std::cout << "Fragment library loaded:" << db.NumberOfPositions() << " positions\n";

        m_fragmentMap[fragmentLength] = db;
        return(fragmentLength);
    }

}
