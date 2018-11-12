##########################################################
##     name:    common_romeo_setup.sh
##  purpose:    Setup environment variables to allow the 
##              example scripts to operate.           
##
##     notes:   The environment variable ROMEO_ROSETTA_DIR
##              should be set in the user's .bashrc file
##              per the README.txt instructions
##
###########################################################

##
## Lines below need to be customized for each installation
##

export ROSETTADB=${ROMEO_ROSETTA_DIR}/main/database
export ROMEO_BIN=~/code/ROMEO/build/bin

##
## Lines below should NOT need to be changed
##

export RP=${ROMEO_BIN}/romeo
export MAKEPDB=${ROMEO_BIN}/MakePDBsFromSolution
export WORKDIR=`pwd`
export PDBDIR=~/code/ROMEO/ROMEO/examples/pdbs

check_romeo_setup()
{
  if [ ! -x ${RP} ]
  then
    echo Can not find executable image for Runner.
    echo script looked here: ${RP}
    exit 99
  fi


}
