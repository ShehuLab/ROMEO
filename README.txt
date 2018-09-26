Installing ROMEO with Rosetta on Ubuntu
---------------------------------------

You will need to install the following packages
to make ROMEO work on a base Ubuntu installation.

Below are the apt-get commands to accomplish this:
   sudo apt-get install git
   sudo apt-get install g++
   sudo apt-get install cmake
   sudo apt-get install liblapack-dev
   sudo apt-get install sqlite3


Building ROMEO with Rosetta
-----------------------------

The Rosetta software can be downloaded from:
    https://www.rosettacommons.org/software.  
An academic license can be obtained by registering.  
First, download and build the Rosetta software.  The location
of the software is defined via environment variables: 

ROMEO_ROSETTA_DIR -- points to the base directory where Rosetta
                      is installed.  For example:
     export ROMEO_ROSETTA_DIR=/home/kmolloy/rosetta_src_2017.08.59291_bundle

You can then run CMake to build ROMEO.  If Rosetta is configured properly, 
you should see a line in the cmake output that looks like:

Rosetta plugin will be compiled.

Normally, the build directory is kept separate from the source 
directory.  So, lets assume that the following directories have been 
created for the source code and build area respectively:

        unzip/git clone ROMEO into /home/myuser which will create
               the /home/myuser/ROMEO directory.
	/home/myuser/ROMEO_bin will be used for building the exes.
 
Use the following commands to build ROMEO:
	cd /home/myuser/ROMEO_bin
	cmake /home/myuser/ROMEO
	make 


