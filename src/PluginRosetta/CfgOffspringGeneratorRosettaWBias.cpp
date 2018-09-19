#include "PluginRosetta/CfgOffspringGeneratorRosettaWBias.hpp"

#include <iostream>
#include <fstream>
#include "Utils/PseudoRandom.hpp"
#include "Utils/Algebra2D.hpp"

namespace Antipatrea
{
    void CfgOffspringGeneratorRosettaWBias::SetupFromParams(Params & params)
    {
        CfgOffspringGeneratorRosetta::SetupFromParams(params);
     }

    // Generate an offspring using Rosetta that is "close" to the target
    // structure specified.

    double CfgOffspringGeneratorRosettaWBias::GenerateAnOffspringCfg(Cfg & cfg)
    {
        auto cfgManager = GetCfgManager();
        auto mol        = GetMolecularStructureRosetta();
        const int dim   = cfgManager->GetDim(); //number of configuration dimensions;
        double *vals    = cfg.GetValues();      //values of the configuration

        auto fragmentNode = m_selector.Select();
        int fragmentSize = fragmentNode->GetKey();

        if (m_verboseFlag) {
        	std::cout<< "Fragment size selected is:" << fragmentSize << std::endl;
        }

        auto &db = m_fragmentMap[fragmentSize];

        // auto sampleAAPosition = RandomUniformInteger(0,db.NumberOfPositions() - 1 - (fragmentSize+1));
        auto sampleAAPosition = RandomUniformInteger(0,db.NumberOfPositions() - 1);
        auto fragmentIndex = RandomUniformInteger(0,db.NumberOfSamples(sampleAAPosition)-1);

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

    void CfgOffspringGeneratorRosettaWBias::GenerateOffspringCfg(Cfg & cfg)
    {
        auto cfgManager = GetCfgManager();

        //don't forget at the end to save
        //so that cfg remembers that the values have been changed.
        //In such cases, it sets the energy to undefined as an indication that it may need to be computed again.
        Cfg *workCfg = cfgManager->NewCfg();

        double smallDist = 999999.99;

        std::cout << "Generating 10 offspring and will pick the closest" << std::endl;

        for (unsigned int i(0);i < 10;++i) {   //KMDEBUG
                double dist = GenerateAnOffspringCfg(*workCfg);
                //KMDEBUG std::cout << "offspring:" << i << " dist:" << dist << "\n";
                if (dist < smallDist) {
                        cfgManager->CopyCfg(cfg,*workCfg);
                        smallDist = dist;
                }
        }
        //KMDEBUG std::cout << "using offstring with dist:" << smallDist << "\n";

        cfgManager->DeleteCfg(workCfg);
    }


}
