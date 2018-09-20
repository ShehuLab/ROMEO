#include "PluginRosetta/SetupRosetta.hpp"
#include "Components/CfgAcceptors/CfgAcceptorBasedOnDistance.hpp"
#include "PluginRosetta/CfgDistanceAtomRMSD.hpp"
#include "boost/format.hpp"
#include <fstream>
#include <iostream>

// rosetta include files
#include <devel/init.hh>

extern "C" Antipatrea::Setup* CreateSetupRosetta(void)
{
    return new Antipatrea::SetupRosetta();
}

namespace Antipatrea
{
    void SetupRosetta::Prepare(Params & params)
    {
        params.SetValue(Constants::KW_UseGoalAcceptor, Constants::KW_GoalAcceptorBasedOnDistance);
        params.SetValue(Constants::KW_UseSignedDistanceBetweenTwoValues, Constants::KW_SignedDistanceBetweenTwoAngles);
        params.SetValue(Constants::KW_UseCfgForwardKinematics, Constants::KW_MolecularStructureRosetta);
        params.SetValue(Constants::KW_UseCfgEnergyEvaluator, Constants::KW_CfgEnergyEvaluatorRosetta);
        params.SetValue(Constants::KW_UseCfgImprover, Constants::KW_CfgImproverRosetta);

        params.SetValue(Constants::KW_UseCfgSampler,Constants::KW_CfgSamplerRosetta);

        NewInstances(params);
        SetupPointers();
        SetupFromParams(params);

        auto data = params.GetData(Constants::KW_MolecularStructureRosetta);
        if (!data || ! data->m_params)
        {
            Logger::m_out << "keyword required in parameter file:"
                              << Constants::KW_MolecularStructureRosetta << std::endl;
            exit(125);
        }

        const char *rosettaDBDir = data->m_params->GetValue(Constants::KW_MolecularStructureRosetta_DBDir);
        const char *energyWeightFile =
                        data->m_params->GetValue(Constants::KW_MolecularStructureRosetta_WeightFile);

        const char *pdbDir =  data->m_params->GetValue(Constants::KW_SetupRosetta_PDBOutputDir,".");

        pdbOutputDirectory = pdbDir;

        RosettaInit(rosettaDBDir);
        // Users can optionally supply a goal conformation without a
        // start conformation.  In this case, the start structure is the
        // extended conformation
        const char  *startPDBFileName
                = data->m_params->GetValue(Constants::KW_MolecularStructureRosetta_CfgStart);
        const char  *goalPDBFileName
                = data->m_params->GetValue(Constants::KW_MolecularStructureRosetta_CfgGoal);

        bool startExtended = data->m_params->GetValueAsBool(
                                        Constants::KW_MolecularStructureRosetta_CfgStartExtended,false);


        auto mol = GetMolecularStructureRosetta();

        auto energyEval = GetCfgEnergyEvaluator();

        mol->CreateEnergyFunction(energyWeightFile);

        //set start & goal
        Cfg *cfgInit = NULL;

        Cfg *cfgGoal = NULL;

        if (startPDBFileName)
        {
            Logger::m_out << "Loading starting config from file:" << startPDBFileName << std::endl;
            GetMolecularStructureRosetta()->LoadPDBFile(startPDBFileName);
            GetCfgManager()->SetDim(GetMolecularStructureRosetta()->GetNrResidues() * 3);
            cfgInit = GetCfgManager()->NewCfg();
            mol->SetCfgDOFs(*cfgInit);

            double energy = energyEval->EvaluateEnergy(*cfgInit);
            cfgInit->SetEnergy(energy);
            Logger::m_out << "energy of start pdb is:" << cfgInit->GetEnergy() << std::endl;
        }


        if (goalPDBFileName)
        {
            Logger::m_out << "Loading goal configuration file:" << goalPDBFileName << std::endl;
            mol->LoadPDBFile(goalPDBFileName);
            GetCfgManager()->SetDim(GetMolecularStructureRosetta()->GetNrResidues() * 3);

            cfgGoal = GetCfgManager()->NewCfg();
            GetMolecularStructureRosetta()->SetCfgDOFs(*cfgGoal); // required?

            double energy = energyEval->EvaluateEnergy(*cfgGoal);
            cfgGoal->SetEnergy(energy);
            Logger::m_out << "energy of goal pdb is:" << cfgGoal->GetEnergy() << std::endl;
        }

        if (startExtended)
        {
            if (!goalPDBFileName)
            {
                Logger::m_out <<  "To use extended, goal config must be set" << std::endl;
                exit(99);
            }

            cfgInit = GetCfgManager()->NewCfg();

            mol->SetExtented(*cfgInit);

            double energy = energyEval->EvaluateEnergy(*cfgInit);
            cfgInit->SetEnergy(energy);

            Logger::m_out << "Using the extended conformation for the starting"
                          << " with energy:" << cfgInit->GetEnergy() << std::endl;
        }

        GetPlannerProblem()->SetInitialCfg(cfgInit);
        GetPlannerProblem()->SetGoalCfg(cfgGoal);

        auto goalAcceptor = dynamic_cast<GoalAcceptorBasedOnDistance*>(GetGoalAcceptor());
        if(goalAcceptor)
            goalAcceptor->SetTargetCfg(cfgGoal);

        auto d = GetCfgDistance();

        if (! d->CheckSetup())
        {
                Logger::m_out <<" error in distance setup" << std::endl;
        }

        if (cfgGoal)
        {
            double startGoalDistance = d->Distance(*cfgInit,*cfgGoal);
            Logger::m_out << "Distance between start and goal structures is:"
                              << startGoalDistance << std::endl;
        }

        if (dynamic_cast<CfgProjectorDeltaR *>(GetCfgProjector()))
        	dynamic_cast<CfgProjectorDeltaR *>(GetCfgProjector())->PostSetup();

    }

    void SetupRosetta::Test(void)
    {
        Setup::Test();
        //add code here to test whatever you would like
        //at this point, all the components have been setup so you can use them
    }

    void SetupRosetta::RosettaInit(const char DBDir[])
    {
        // Rosetta init call takes a C function style call list

        char ** rosetta_argv = (char **) malloc(sizeof(char*) * 3);
        char DBDirCopy[350];
        strcpy(DBDirCopy,DBDir);
        rosetta_argv[0] = (char*)"";
        rosetta_argv[1] = (char*)"-database";
        rosetta_argv[2] = DBDirCopy;

        devel::init(3,rosetta_argv);
        // free (rosetta_argv);
    }

    void SetupRosetta::PlannerCfgs()
    {
        auto goalAcceptor = dynamic_cast<GoalAcceptorBasedOnDistance*>(GetGoalAcceptor());
        if(goalAcceptor)
        {
            std::vector<Cfg *> closest = goalAcceptor->GetNClosest();
            auto mol = GetMolecularStructureRosetta();
            unsigned int counter = 0;
            for (auto c : closest)
            {
                std::string fileName = pdbOutputDirectory + "/closestPDB" + (boost::format("%02d") % counter).str() + ".pdb";
                std::cout << "filename:" << fileName << "\n";
                mol->WritePDBFile(*c,fileName.c_str());
                ++counter;
            }
        }
    }
}
