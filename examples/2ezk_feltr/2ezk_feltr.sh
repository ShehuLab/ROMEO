#!/bin/bash 
##########################################################
##     name:    2ezk_feltr.sh
##  purpose:    Demonstrate ROMEO using the FeLTR
##              planner to generate low-energy,native-like
##              decoy configurations for PDB ID 2EZK.
##
##    input:    --> paramters (contained in this script)
##              --> Fragment libraries (generated from Robetta,
##                  at http://robetta.bakerlab.org/)
##              --> PDB file for 2ezk (obtained from 
##                  https://www.rcsb.org)
##
##    notes:
##       Before running this script, make sure you setup the
##       environment variable COMMON_ROMEO_SETUP so that the
##       script can locate the software, pdb files, fragment
##       libraries, etc.  You need to edit this file for
##       each installation.
##
########################################################### 


if [ "${COMMON_ROMEO_SETUP:-NOTSET}" == "NOTSET" ]
then
  echo The COMMON_ROMEO_SETUP environment variable must be set.
  echo exiting
  exit 99
fi

if [ ! -r ${COMMON_ROMEO_SETUP} ]
then
  echo COMMON_ROMEO_SETUP is set to a file that can
  echo not be read: ${COMMON_ROMEO_SETUP}
  exit 99
fi


. ${COMMON_ROMEO_SETUP:-NOTSET}

check_romeo_setup

SETUP=2ezk_feltr


## Runner currently wants its own
## directory to run it (and creates a directory called data)
## In order to allow multiple versions to run simultaneously, 
## we create a randomly generated name to work in based
## on the PID of this process.

LWORKDIR=${WORKDIR}/${SETUP}_$$
mkdir ${LWORKDIR}
cd ${LWORKDIR}


PARMFILE=${LWORKDIR}/${SETUP}.txt
LOGFILE=${LWORKDIR}/${SETUP}.log
MAKEPDBLOGFILE=${LWORKDIR}/${SETUP}_makepdb.log

## Lets make the parmfile

cat << EOF > ${PARMFILE}

### 
### Explore low-energy configurations of 2ezk
###

UseSetup SetupRosetta
UsePlanner FELTR
UseCfgManager CfgManager
UseCfgAcceptor CfgAcceptorBasedOnFixedMMC
UseGoalAcceptor GoalAcceptorBasedOnDistance
UseCfgOffspringGenerator CfgOffspringGeneratorRosetta
UseCfgDistance CfgDistanceAtomRMSD
UseSignedDistanceBetweenTwoValues SignedDistanceBetweenTwoAngles
UseEdgeCostEvaluator EdgeCostEvaluatorBasedOnDistance
UseCfgSampler CfgUniformSamplerInJointSpace
UseCfgImprover CfgImproverDoNothing
UseCfgProjector CfgProjectorUSREnergy

CfgAcceptorBasedOnFixedMMC { FixedAcceptDeltaE 10.0 
                             FixedAcceptProb .1 }

TreeSamplingBasedPlanner
{
  SampleValidTargetCfg false
  ExtendMaxNrSteps     1
  ExtendReachedTargetDistanceThreshold 0.1
  GoalBias 0.00
}

RunPlanner
{
  ## MaxRuntime    3660.0   # one hour simulation
  MaxRuntime      60.0   # one minute simulation
  IntervalRunTime 30.0
}

GoalAcceptorBasedOnDistance { DistanceThreshold 1.0 }

#PluginRosetta
MolecularStructureRosetta
{
  DBDir    ${ROSETTADB}
  CfgStartExtended true
  CfgGoal  ${PDBDIR}/2ezk.pdb
  WeightFile score3.wts
}


CfgOffspringGeneratorRosetta
{
  FragmentFiles [ ${PDBDIR}/2ezk.rosettahomolog.frag3
                  ${PDBDIR}/2ezk.rosettahomolog.frag9 ]

  FragmentProbabilities [ 0.3 0.7 ]
}

#Components/CfgAcceptors


#Components/EdgeCostEvaluators
EdgeCostEvaluator { }
EdgeCostEvaluatorBasedOnDistance { }
EdgeCostEvaluatorBasedOnEnergy { }

#Planners
PlannerGraph
{
  #UseFile data/PlannerGraph.txt
  ReadWhenPlannerStarts false
  PrintWhenPlannerEnds true
}

EOF
#####


echo working from `pwd`
mkdir data
${RP} RunPlanner  ${PARMFILE} > ${LOGFILE}
rprc=$?

######
######
######
######
######

if [ ${rprc} -ne 0 ]
then
  echo Planner had errors
  exit 99
fi

RESULTS_DIR=${LWORKDIR}/data
## move the log files into the results directory as well

## Now run the expanded to make PDB files

if [ -r ${RESULTS_DIR}/planner.stats_sol0 ]
then
  echo results found processing into PDBs in results dir ${RESULTS_DIR}
  PDBPARMFILE=makepdb_parms.txt
  cat << EOF > ${PDBPARMFILE}
      RosettaDB ${ROSETTADB}
      PDBModel ${PDBDIR}/2ezk.pdb
      SolutionFileName ${RESULTS_DIR}/planner.stats_sol0
      PDBOutputDir ${RESULTS_DIR}
EOF
  ${MAKEPDB}  ${PDBPARMFILE} > ${MAKEPDBLOGFILE}
fi

