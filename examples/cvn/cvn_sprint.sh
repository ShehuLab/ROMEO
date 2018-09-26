#!/bin/bash
##########################################################
##     name:    cvn_sprint.sh
##  purpose:    Demonstrate ROMEO using a lightweight
##              version of the SPRINT method to find an
##              energetically feasible path between
##              two states of cvn: 2ezm and 1l5e.  
##
##    input:    --> paramters (contained in this script)
##              --> Fragment libraries (generated from Robetta,
##                  at http://robetta.bakerlab.org/)
##              --> PDB file for each state (obtained from
##                  https://www.rcsb.org)
##              --> special energy weight file score3NoRg.wts
##                  This is a copy of the score3.wts file
##                  with the weights for rg and cenpack 
##                  terms set to zero.  This file (score3norg.wts)
##                  is within this directory, but must be copied
##                  to the Rosetta weights directory, which
##                  is typically in main/database/scoring/weights
##                  underneath the Rosetta installation.
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

SETUP=cvn_sprint
LWORKDIR=${WORKDIR}/${SETUP}_$$

mkdir ${LWORKDIR}
cd ${LWORKDIR}

PARMFILE=${LWORKDIR}/${SETUP}.txt
LOGFILE=${LWORKDIR}/${SETUP}.log
MAKEPDBLOGFILE=${LWORKDIR}/${SETUP}_makepdb.log


## Lets make the parmfile
cat << EOF > ${PARMFILE}

### 
### Find Path between 2ezm and 1l5e
###
UseSetup SetupRosetta
UsePlanner Sprint
UseCfgManager CfgManager
UseCfgAcceptor CfgAcceptorBasedOnEnergy
UseGoalAcceptor GoalAcceptorBasedOnDistance
UseCfgOffspringGenerator CfgOffspringGeneratorRosetta
UseCfgDistance CfgDistanceAtomRMSD
UseSignedDistanceBetweenTwoValues SignedDistanceBetweenTwoAngles
UseEdgeCostEvaluator EdgeCostEvaluatorBasedOnDistance
UseCfgSampler CfgUniformSamplerInJointSpace
UseCfgImprover CfgImproverDoNothing
UseCfgProjector CfgProjectorDeltaR

Sprint
{
  RegionExp 1
}

RunPlanner
{
  MaxRuntime   300.0  ## run 5 minutes for a demo, typicall run longer
  MaxTreeSize 500000
}

#PluginRosetta
MolecularStructureRosetta
{
  DBDir    ${ROSETTADB}
  CfgStart ${PDBDIR}/2ezm_mod_relax.pdb
  CfgGoal  ${PDBDIR}/1l5e_mod_relax.pdb
  WeightFile score3NoRg.wts
}


CfgOffspringGenerator { VerboseFlag false }

CfgOffspringGeneratorRosetta
{
  FragmentFiles [ ${PDBDIR}/2ezm.RosettaHomolog.frag3
                  ${PDBDIR}/2ezm.RosettaHomolog.frag9 ]

  FragmentProbabilities [ 0.7 0.3 ]
  DistanceTol 3.0
  NumberToGenerate 50
}

#Components/CfgAcceptors

## make almost any configuration acceptable
CfgAcceptorBasedOnEnergy   { EnergyThreshold 80.0 }

GoalAcceptorBasedOnDistance { DistanceThreshold 5.0 VerboseFlag true }
#GoalAcceptorBasedOnEnergy { EnergyThreshold -100.0 }

#Components/CfgDistances

CfgDistance { }
CfgDistanceLp { Exponent 2 }

#Planners
TreeSamplingBasedPlanner
{
  SampleValidTargetCfg false
  ExtendMaxNrSteps 5
  ExtendReachedTargetDistanceThreshold 0.1
  GoalBias 0.95
}

PlannerGraph
{
#UseFile data/PlannerGraph.txt
ReadWhenPlannerStarts false
PrintWhenPlannerEnds true
}

SamplingBasedPlanner { OneStepDistance 0.1 } 

RRT { }


EOF
#####

mkdir data

rm ${LOGFILE} > /dev/null 2>&1 

echo Running planner.....
${RP} RunPlanner  ${PARMFILE} > ${LOGFILE}
rprc=$?

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
      PDBModel ${PDBDIR}/2ezm_mod_relax.pdb
      SolutionFileName ${RESULTS_DIR}/planner.stats_sol0
      PDBOutputDir ${RESULTS_DIR}
EOF
  ${MAKEPDB}  ${PDBPARMFILE} > ${MAKEPDBLOGFILE}
fi
