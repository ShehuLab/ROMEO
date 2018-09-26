#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
import sys

if len(sys.argv) < 3 or len(sys.argv) > 4 :
    print("args: infile outpdffile energyOfGoal")
    sys.exit(99)

solFileName  = sys.argv[1]
outFileName  = sys.argv[2]

solFile      = open(solFileName)
solLines     = solFile.readlines()

solFile.close()

## build an array of energy values 

energyPoints = []
## first line of file is the cost
## second line number of nodes in the solution (minus the goal)
## third line is the set of vertices in the solution
## not sure about 4th line
## now sets of 2 lines (1st line coords, second energy)
for i in range(5,len(solLines),2):
    energyPoints.append( (float) (solLines[i].rstrip()))

if len(sys.argv) == 4:
    energyPoints.append((float)(sys.argv[3]))

plt.plot(range(len(energyPoints)),energyPoints,lw=2,color="red")

plt.xlabel('Node in Path',fontsize=20)
plt.ylabel('Energy (score3 norg)',fontsize=20)


plt.savefig(sys.argv[2],dpi=400,bbox_inches='tight', pad_inches=0.05)
