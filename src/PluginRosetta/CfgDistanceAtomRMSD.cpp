#include "PluginRosetta/CfgDistanceAtomRMSD.hpp"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "boost/format.hpp"

// LaPACK functions for computing SVD
extern "C"
{
  // svd
  void dgesvd_(char*, char*, int*, int*, double*, int*, double*, double*, int*, double*,
               int*, double*, int*, int*);

  void dgeev_(char*, char*, int*, double*, int*, double*, double*, double*, int*, double*, int*, double*, int*, int*);
}

namespace Antipatrea
{        
/*
 * RMSD.  Takes two vectors, each representing a degree of freedom, and
 *        they should be divisable by 3 (x,y,z for each point).
 *        n is the size of these two vectors.
 */
    double RMSD(const double *x,
                const double *y,
                const int n)
    {
        //Check that 2 input iterators are the same length
        //make sure that the output iterators are large enough to contain all info
        if (n % 3 != 0)
        {
          std::cout << "RMSD error. Passed vector length:" << n
                    << " is not divisable by 3." << std::endl;
          exit(125);
        }

        int i;
        double comx[3], comy[3], C[9] = {}, w[n];
        int three=3;

        // compute centers of mass
        comx[0]=comx[1]=comx[2]=comy[0]=comy[1]=comy[2]=0.;
        for (i=0; i<n; i+=3)
        {
            comx[0]+=x[i]; comx[1]+=x[i+1]; comx[2]+=x[i+2];
            comy[0]+=y[i]; comy[1]+=y[i+1]; comy[2]+=y[i+2];
        }
        comx[0]*=3./n; comx[1]*=3./n; comx[2]*=3./n;
        comy[0]*=3./n; comy[1]*=3./n; comy[2]*=3./n;

        double xAlign[n];
        double yAlign[n];

        // compute covariance matrix
        for (i=0; i<n; i+=3)
        {
            xAlign[i] = x[i]-comx[0]; xAlign[i+1] = x[i+1]-comx[1]; xAlign[i+2] = x[i+2]-comx[2];
            w[i] = y[i]-comy[0]; w[i+1] = y[i+1]-comy[1]; w[i+2] = y[i+2]-comy[2];
            C[0] += xAlign[i]*w[i];   C[1] += xAlign[i]*w[i+1];   C[2] += xAlign[i]*w[i+2];
            C[3] += xAlign[i+1]*w[i]; C[4] += xAlign[i+1]*w[i+1]; C[5] += xAlign[i+1]*w[i+2];
            C[6] += xAlign[i+2]*w[i]; C[7] += xAlign[i+2]*w[i+1]; C[8] += xAlign[i+2]*w[i+2];
        }

        // compute SVD of C
        int lwork=30, info=0;
        double S[3], U[9], VT[9], work[30];
        char a='A';
        dgesvd_(&a,&a,&three,&three,C,&three,S,U,&three,VT,&three,work,&lwork,
                &info);

        // compute rotation: rot=U*VT
        double rot[9];
        rot[0] = U[0]*VT[0] + U[3]*VT[1] + U[6]*VT[2];
        rot[1] = U[1]*VT[0] + U[4]*VT[1] + U[7]*VT[2];
        rot[2] = U[2]*VT[0] + U[5]*VT[1] + U[8]*VT[2];
        rot[3] = U[0]*VT[3] + U[3]*VT[4] + U[6]*VT[5];
        rot[4] = U[1]*VT[3] + U[4]*VT[4] + U[7]*VT[5];
        rot[5] = U[2]*VT[3] + U[5]*VT[4] + U[8]*VT[5];
        rot[6] = U[0]*VT[6] + U[3]*VT[7] + U[6]*VT[8];
        rot[7] = U[1]*VT[6] + U[4]*VT[7] + U[7]*VT[8];
        rot[8] = U[2]*VT[6] + U[5]*VT[7] + U[8]*VT[8];
        // make sure rot is a proper rotation, check determinant
        if ((rot[1]*rot[5]-rot[2]*rot[4])*rot[6]
            + (rot[2]*rot[3]-rot[0]*rot[5])*rot[7]
            + (rot[0]*rot[4]-rot[1]*rot[3])*rot[8] < 0)
        {
            rot[0] -= 2*U[6]*VT[2]; rot[1] -= 2*U[7]*VT[2]; rot[2] -= 2*U[8]*VT[2];
            rot[3] -= 2*U[6]*VT[5]; rot[4] -= 2*U[7]*VT[5]; rot[5] -= 2*U[8]*VT[5];
            rot[6] -= 2*U[6]*VT[8]; rot[7] -= 2*U[7]*VT[8]; rot[8] -= 2*U[8]*VT[8];
        }

        // compute rmsd
        double dist=0;
        for (i=0; i<n; i+=3)
        {
            yAlign[i]   = rot[0]*w[i] + rot[1]*w[i+1] + rot[2]*w[i+2];
            yAlign[i+1] = rot[3]*w[i] + rot[4]*w[i+1] + rot[5]*w[i+2];
            yAlign[i+2] = rot[6]*w[i] + rot[7]*w[i+1] + rot[8]*w[i+2];
            double dx = (yAlign[i] - xAlign[i]);
            double dy = (yAlign[i + 1] - xAlign[i + 1]);
            double dz = (yAlign[i + 2] - xAlign[i + 2]);
            dist += (dx*dx + dy*dy + dz*dz);
        }
        // rsmd too large => return INF

        if (dist!=HUGE_VAL)
        {
            dist = std::sqrt(dist*3./n);
        }

        return dist;
    }

    double CfgDistanceAtomRMSD::DistancePrint(const Cfg & cfg1, const Cfg & cfg2)
    {
        double rmsd = Distance(cfg1,cfg2);
        ++distCalcs;
        if (distCalcs == 0 || 1 ) // for debugging
        {
            auto mol = GetMolecularStructureRosetta();

            std::string distCalcNumber = (boost::format("%06d") % distCalcs).str();

            std::string fName_1 = "distCalcDebug_" + distCalcNumber + "_01.pdb";
            std::string fName_2 = "distCalcDebug_" + distCalcNumber + "_02.pdb";
            std::string fName_rmsd = "distCalcDebug_" + distCalcNumber + "_rmsd.txt";



            mol->WriteScoredPDBFile(cfg1,fName_1.c_str());

            mol->WriteScoredPDBFile(cfg2,fName_2.c_str());

            std::ofstream rmsdFile;
            rmsdFile.open(fName_rmsd.c_str());
            rmsdFile << rmsd << "\n";
            rmsdFile.close();

            // create file with our rmsd so we can compare
        }
        return(rmsd);
    }


    double CfgDistanceAtomRMSD::Distance(const Cfg & cfg1, const Cfg & cfg2)
    {
        auto cfgM = GetCfgManager();

        auto mol = GetMolecularStructureRosetta();
        const double *vals1 = cfg1.GetValues();
        std::vector<point> cfg1Atoms = mol->GetAtomPositions(cfg1);

        const double *vals2 = cfg2.GetValues();
        std::vector<point> cfg2Atoms = mol->GetAtomPositions(cfg2);

        // we need 3 DOFs (x,y,z) for each atom returned.
        unsigned int atomCount = cfg1Atoms.size();
        unsigned int dim = atomCount *3;

        double atomPositionsCfg1[dim];
        double atomPositionsCfg2[dim];
        for (auto i =0; i < atomCount;++i)
        {
            atomPositionsCfg1[i*3]      = cfg1Atoms[i].x;
            atomPositionsCfg1[i*3 + 1]  = cfg1Atoms[i].y;
            atomPositionsCfg1[i*3 + 2]  = cfg1Atoms[i].z;

            atomPositionsCfg2[i*3]      = cfg2Atoms[i].x;
            atomPositionsCfg2[i*3 + 1]  = cfg2Atoms[i].y;
            atomPositionsCfg2[i*3 + 2]  = cfg2Atoms[i].z;
        }

        double rmsd = RMSD(atomPositionsCfg1,atomPositionsCfg2,dim);

        return (rmsd);
    }
}
