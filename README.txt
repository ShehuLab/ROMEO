sudo apt-get install freeglut3-dev 

Building CodeBio with Rosetta
-----------------------------

The Rosetta software can be downloaded from:
    https://www.rosettacommons.org/software.  
An academic license can be obtained by registering.  
First, download and build the Rosetta software.  The location
of the software is defined via environment variables: 

RUNNER_ROSETTA_DIR -- points to the base directory where Rosetta
                      is installed.  For example:
     export RUNNER_ROSETTA_DIR=/home/kmolloy/rosetta_src_2017.08.59291_bundle

You can then run CMake to build CodeBio.  Normally, the build directory
is kept separate from the source directory.  So, lets assume that the
following directories have been created for the  source code and build area
respectively:
	/home/myuser/CodeBio
	/home/myuser/CodeBioBin
 
You could issue the following commands to build CodeBio:
	cd /home/myuser/CodeBioBin
	cmake /home/myuser/CodeBio
	make 

