#ifndef Antipatrea__Defaults_HPP_
#define Antipatrea__Defaults_HPP_

#include "Utils/Definitions.hpp"
#include <cmath>
#include <limits>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Keywords and default values.
     */
    namespace Constants
    {
        //CfgManagers
        const char KW_UseCfgManager[]  = "UseCfgManager";
        const char KW_CfgManager[]     = "CfgManager";
        const char KW_Dim[]            = "Dim";
        
        //CfgAcceptors
        const char KW_UseCfgAcceptor[]             = "UseCfgAcceptor";
        const char KW_CfgAcceptor[]                = "CfgAcceptor";
        const char KW_CfgAcceptorBasedOnDistance[] = "CfgAcceptorBasedOnDistance";
        const char KW_CfgAcceptorBasedOnEnergy[]   = "CfgAcceptorBasedOnEnergy";
        const char KW_CfgAcceptorBasedOnMMC[]      = "CfgAcceptorBasedOnMMC";
        const char KW_CfgAcceptorBasedOnFixedMMC[] = "CfgAcceptorBasedOnFixedMMC";

        const char KW_EnergyThreshold[]            = "EnergyThreshold";
        const char KW_DistanceThreshold[]          = "DistanceThreshold";
        const char KW_MCCTemperature[]             = "Temperature";
        const char KW_MCCTemperatureAdjRate[]      = "TemperatureAdjRate";
        const char KW_FixedAcceptDeltaE[]          = "FixedAcceptDeltaE";
        const char KW_FixedAcceptProb[]            = "FixedAcceptProb";

        const double VAL_CfgAcceptorBasedOnMMC_Temperature         = 20.0;
        const double VAL_CfgAcceptorBasedOnMMC_TemperatureAdjRate  = 0.1;

        
        const double VAL_CfgAcceptorBasedOnEnergy_EnergyThreshold     = INFINITY;
        const double VAL_CfgAcceptorBasedOnDistance_DistanceThreshold = 0.2;
        
        //GoalAcceptors
        const char KW_UseGoalAcceptor[]             = "UseGoalAcceptor";
        const char KW_GoalAcceptor[]                = "GoalAcceptor";
        const char KW_GoalAcceptorBasedOnDistance[] = "GoalAcceptorBasedOnDistance";
        const char KW_GoalAcceptorBasedOnEnergy[]   = "GoalAcceptorBasedOnEnergy";
        const char KW_VerboseFlag[]                  = "VerboseFlag";
        
        const double VAL_GoalAcceptorBasedOnDistance_DistanceThreshold = VAL_CfgAcceptorBasedOnDistance_DistanceThreshold;
        const double VAL_GoalAcceptorBasedOnEnergy_EnergyThreshold     = INFINITY;
        
        const double VAL_Boltzman_K         = 0.0019872041;
        //SignedDistanceBetweenTwoValues
        const char KW_UseSignedDistanceBetweenTwoValues[] = "UseSignedDistanceBetweenTwoValues";
        const char KW_SignedDistanceBetweenTwoValues[]    = "SignedDistanceBetweenTwoValues";
        const char KW_SignedDistanceBetweenTwoAngles[]    = "SignedDistanceBetweenTwoAngles";
        const char KW_SignedDistanceBetweenTwoNumbers[]   = "SignedDistanceBetweenTwoNumbers";
        
        //CfgDistance
        const char KW_UseCfgDistance[] = "UseCfgDistance";
        const char KW_CfgDistance[]    = "CfgDistance";
        const char KW_CfgDistanceLp[]  = "CfgDistanceLp";
        const char KW_Exponent[]       = "Exponent";
        const char KW_CfgDistanceAtomRMSD[] = "CfgDistanceAtomRMSD";
        
        const int VAL_CfgDistanceLp_Exponent = 2;
                
        //CfgEnergyEvaluators
        const char KW_UseCfgEnergyEvaluator[]          = "UseCfgEnergyEvaluator";
        const char KW_CfgEnergyEvaluator[]             = "CfgEnergyEvaluator";
        const char KW_CfgEnergyEvaluatorInJointSpace[] = "CfgEnergyEvaluatorInJointSpace";
        
        //CfgImprovers    
        const char KW_UseCfgImprover[]       = "UseCfgImprover";
        const char KW_CfgImprover[]          = "CfgImprover";
        const char KW_CfgImproverDoNothing[] = "CfgImproverDoNothing";
        
        
        //CfgOffspringGenerators
        const char KW_UseCfgOffspringGenerator[] = "UseCfgOffspringGenerator";
        const char KW_CfgOffspringGenerator[]    = "CfgOffspringGenerator";
        const char KW_Step[]                     = "Step";

        const char KW_CfgOffspringGeneratorTowardTarget[]                  = "CfgOffspringGeneratorTowardTarget";
        const char KW_CfgOffspringGeneratorToTarget[]                      = "CfgOffspringGeneratorToTarget";
        const char KW_CfgOffspringGeneratorToTargetByLinearInterpolation[] = "CfgOffspringGeneratorToTargetByLinearInterpolation";

        const char KW_CfgOffspringGeneratorByGaussianPerturbation[] = "CfgOffspringGeneratorByGaussianPerturbation";
        const char KW_Consecutive[]                                 = "Consecutive";
        const char KW_MinNrValuesToPerturb[]                        = "MinNrValuesToPerturb";
        const char KW_MaxNrValuesToPerturb[]                        = "MaxNrValuesToPerturb";

        const double VAL_CfgOffspringGenerator_Step = 0.1;
        
        const bool VAL_CfgOffspringGeneratorByGaussianPerturbation_Consecutive          = false;
        const int  VAL_CfgOffspringGeneratorByGaussianPerturbation_MinNrValuesToPerturb = 1;
        const int  VAL_CfgOffspringGeneratorByGaussianPerturbation_MaxNrValuesToPerturb = std::numeric_limits<int>::max();
        
        
        //CfgSamplers
        const char KW_UseCfgSampler[]                 = "UseCfgSampler";
        const char KW_CfgSampler[]                    = "CfgSampler";
        const char KW_CfgUniformSamplerInJointSpace[] = "CfgUniformSamplerInJointSpace";
        
        //CfgForwardKinematics
        const char KW_UseCfgForwardKinematics[] = "UseCfgForwardKinematics";
        const char KW_CfgForwardKinematics[]    = "CfgForwardKinematics";
                 
        //CfgProjector
        const char KW_UseCfgProjector[] = "UseCfgProjector";
        const char KW_CfgProjector[]    = "CfgProjector";

        //EdgeCostEvaluators
        const char KW_UseEdgeCostEvaluator[]             = "UseEdgeCostEvaluator";
        const char KW_EdgeCostEvaluator[]                = "EdgeCostEvaluator";
        const char KW_EdgeCostEvaluatorBasedOnDistance[] = "EdgeCostEvaluatorBasedOnDistance";
        const char KW_EdgeCostEvaluatorBasedOnEnergy[]   = "EdgeCostEvaluatorBasedOnEnergy";

        
        //PlannerGraph
        const char KW_UsePlannerGraph[]      = "UsePlannerGraph";
        const char KW_PlannerGraph[]         = "PlannerGraph";
        const char KW_ReadWhenPlannerStarts[]= "ReadWhenPlannerStarts";
        const char KW_PrintWhenPlannerEnds[] = "PrintWhenPlannerEnds";
        const char VAL_PlannerGraphFile[]    = "data/PlannerGraph.txt";

        //PlannerProblem
        const char KW_UsePlannerProblem[] = "UsePlannerProblem";
        const char KW_PlannerProblem[]    = "PlannerProblem";

        //PlannerSolution
        const char KW_UsePlannerSolution[] = "UsePlannerSolution";
        const char KW_PlannerSolution[]    = "PlannerSolution";

        
        //Planner
        const char KW_UsePlanner[] = "UsePlanner";
        const char KW_Planner[]    = "Planner";

        //SamplingBasedPlanner
        const char KW_SamplingBasedPlanner[] = "SamplingBasedPlanner";
        const char KW_OneStepDistance[]      = "OneStepDistance";
        
        const double VAL_SamplingBasedPlanner_OneStepDistance = 0.1;
        
        //PRM
        const char KW_PRM[]                         = "PRM";
        const char KW_NrNeighbors[]                 = "NrNeighbors";
        const char KW_BatchSizeToGenerateVertices[] = "BatchSizeToGenerateVertices";
        const char KW_ProbAllowCycles[]             = "ProbAllowCycles";

        const int    VAL_PRM_NrNeighbors                 = 10;
        const int    VAL_PRM_BatchSizeToGenerateVertices = 100;
        const double VAL_PRM_ProbAllowCycles             = 0.0;

        //TreeSamplingBasedPlanner
        const char KW_TreeSamplingBasedPlanner[]             = "TreeSamplingBasedPlanner";
        const char KW_ExtendMaxNrSteps[]                     = "ExtendMaxNrSteps";
        const char KW_ExtendReachedTargetDistanceThreshold[] = "ExtendReachedTargetDistanceThreshold";
        const char KW_GoalBias[]                             = "GoalBias";
        const char KW_SampleValidTargetCfg[]                 = "SampleValidTargetCfg";
        
        const int    VAL_Tree_ExtendMaxNrSteps                     = 100;
        const double VAL_Tree_ExtendReachedTargetDistanceThreshold = 0.1;
        const double VAL_Tree_GoalBias                             = 0.05;
        
        //EST
        const char KW_EST[]                     = "EST";
        const char KW_NeighborhoodRadius[]      = "NeighborhoodRadius";
        const double VAL_EST_NeighborhoodRadius = 2.0;

        //FELTR
        const char   KW_FELTR[]                    = "FELTR";
        const char   KW_FELTR_CELLGRID_GRAN[]    = "CellGridGranularity";
        const char   KW_FELTR_ENERGYGRID_GRAN[]    = "EnergyGridGranularity";
        const char   KW_FELTR_ENERGYGRID_MIN[]     = "EnergyGridMin";
        const char   KW_FELTR_ENERGYGRID_MAX[]     = "EnergyGridMax";
        const int    VAL_EnergyGrid_Granularity    = 100;
        const double VAL_EnergyGrid_Min            = -50.0;
        const double VAL_EnergyGrid_Max            = 200.0;
        const int    VAL_CellGrid_Granularity      = 30;

        //SPRINT
        const char   KW_SPRINT[]                    = "Sprint";
        const char   KW_SPRINT_REGION_EXP[]         = "RegionExp";

        const int    VAL_SPRINT_REGION_EXP          = 2;

        //RRT
        const char KW_RRT[] = "RRT";
        
        //PGT
        const char KW_PGT[] = "PGT";
        
        //Running the planner
        const char KW_RunPlanner[]      = "RunPlanner";
        const char KW_UseStatsFile[]    = "UseStatsFile";
        const char KW_MaxNrRuns[]       = "MaxNrRuns";
        const char KW_MaxRuntime[]      = "MaxRuntime";
        const char KW_MaxNrFailures[]   = "MaxNrFailures";
        const char KW_IntervalRuntime[] = "IntervalRuntime";
        const char KW_MaxTreeSize[]     = "MaxTreeSize";
        const char KW_ReadInitAndGoalCfgsFromFile[] = "ReadInitAndGoalCfgsFromFile";
        
        const char         VAL_RunPlanner_StatsFile[]     = "data/planner.stats";
        const int          VAL_RunPlanner_MaxNrRuns       = 10;
        const int          VAL_RunPlanner_MaxNrFailures   = 5;
        const double       VAL_RunPlanner_MaxRuntime      = 40.0;
        const double       VAL_RunPlanner_IntervalRuntime = 30.0;
        const unsigned int VAL_RunPlanner_MaxTreeSize     = 100000;
        
        //Stats
        const char KW_Runtime_PlannerSolve[]         = "Runtime_PlannerSolve";
        const char KW_Runtime_PlannerStart[]         = "Runtime_PlannerStart";
        const char KW_Runtime_PRM_GenerateVertices[] = "Runtime_PRM_GenerateVertices";
        const char KW_Runtime_PRM_GenerateEdges[]    = "Runtime_PRM_GenerateEdges";





        //Setup
        const char KW_UseSetup[]    = "UseSetup";
        

        //Plugin2D
        const char KW_Scene2D[]       = "Scene2D";
        const char VAL_SceneFile[]    = "data/scene.txt";
        
        const char KW_SetupPoint2D[]        = "SetupPoint2D";
        const char KW_CfgSamplerPoint2D[]   = "CfgSamplerPoint2D";
        const char KW_CfgAcceptorPoint2D[]  = "CfgAcceptorPoint2D";
        const char KW_CfgProjectorPoint2D[] = "CfgProjectorPoint2D";

        const char   KW_SetupChain2D[]        = "SetupChain2D";
        const char   KW_Chain2D[]             = "Chain2D";
        const char   KW_NrLinks[]             = "NrLinks";
        const char   KW_LinkLength[]          = "LinkLength";
        const int    VAL_Chain2D_NrLinks      = 8; 
        const double VAL_Chain2D_LinkLength   = 1.0;
        const char   KW_CfgAcceptorChain2D[]  = "CfgAcceptorChain2D";
        const char   KW_CfgProjectorChain2D[] = "CfgProjectorChain2D";
        
        //PluginRosetta
        const char   KW_SetupRosetta[]                      = "SetupRosetta";
        const char   KW_MolecularStructureRosetta[]         = "MolecularStructureRosetta";
        const char   KW_MolecularStructureRosetta_DBDir[]   = "DBDir";
        const char   KW_OffspringRosetta_FragmentFiles[]         = "FragmentFiles";
        const char   KW_OffspringRosetta_FragmentProbabilities[] = "FragmentProbabilities";
        const char   KW_OffspringRosetta_NumberToGenerate[]      = "NumberToGenerate";
        const char   KW_OffspringRosetta_DistanceTol[]      = "DistanceTol";
        const int    VAL_OffspringRosetta_NumberToGenerate  = 10;
        const double VAL_OffspringRosetta_DistanceTol       = 0.0;

        const char KW_MolecularStructureRosetta_OffspringToGenerate[] = "OffspringToGenerate";
        const char KW_MolecularStructureRosetta_WeightFile[]   = "WeightFile";
        const char KW_SetupRosetta_PDBOutputDir[]   = "PDBOutputDir";

        const char KW_MolecularStructureRosetta_CfgStart[] = "CfgStart";
        const char KW_MolecularStructureRosetta_CfgStartExtended[] = "CfgStartExtended";

        const char KW_MolecularStructureRosetta_CfgGoal[] = "CfgGoal";
        const char KW_CfgSamplerRosetta[]            = "CfgSamplerRosetta";
        const char KW_CfgImproverRosetta[]           = "CfgImproverRosetta";
        const char KW_CfgOffspringGeneratorRosetta[] = "CfgOffspringGeneratorRosetta";
        const char KW_CfgOffspringGeneratorRosettaWBias[] = "CfgOffspringGeneratorRosettaWBias";
        const char KW_CfgOffspringGeneratorRosetta_FragmentLib[] = "FragmentLib";
        const char KW_CfgEnergyEvaluatorRosetta[]    = "CfgEnergyEvaluatorRosetta";
        const char KW_CfgProjectorUSREnergy[]        = "CfgProjectorUSREnergy";
        const char KW_CfgProjectorDeltaR[]           = "CfgProjectorDeltaR";
    }
}

#endif
