#!/bin/bash

CODEBIO_BIN=../../build/bin
RP=${CODEBIO_BIN}/Runner
MAKEPDB=${CODEBIO_BIN}/MakePDBsFromSolution
DEMODIR=`pwd`

SETUP=metenk_params_prm
PARMFILE=${DEMODIR}/${SETUP}.txt
LOGFILE=${DEMODIR}/${SETUP}.log
MAKEPDBLOGFILE=${DEMODIR}/${SETUP}_makepdb.log

if [ -d ${DEMODIR}/data ]
then
  echo removing old data directory ${DEMODIR}/data
  rm -r ${DEMODIR}/data
fi

if [ -d ${DEMODIR}/${SETUP} ]
then
  rm -r ${DEMODIR}/${SETUP}
fi

mkdir data

${RP} RunPlanner  ${PARMFILE} > ${LOGFILE}

RESULTS_DIR=${DEMODIR}/${SETUP}
mv data ${RESULTS_DIR}

## Now run the expanded to make PDB files

if [ -r ${RESULTS_DIR}/planner.stats_sol0 ]
then
  echo results found processing into PDBs
  rm /tmp/sol_pdb_*.pdb > /dev/null 2>&1
  ${MAKEPDB}  ${DEMODIR}/metenk_min1.pdb ${RESULTS_DIR}/planner.stats_sol0 > ${MAKEPDBLOGFILE}
fi

