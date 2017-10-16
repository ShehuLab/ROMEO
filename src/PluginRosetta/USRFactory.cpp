/*
 * USRFactory.cpp
 *
 *  Created on: Oct 3, 2017
 *      Author: kmolloy
 */


#include "PluginRosetta/USRFactory.hpp"
#include "Utils/Vector3Algebra.hpp"

namespace Antipatrea
{
using namespace Antipatrea::Vector3Algebra;
//----------------------------------------------------------------------------
USRFactory::USRFactory()
{
	configCoords = NULL;
	coordsSize = 0;

	ctd.x = ctd.y = ctd.z = 0.0;
	indexOfcst = indexOffct = indexOfftf = -1;

	//these depend on size of configuration
	dsFromctd = dsFromcst = dsFromfct = dsFromftf = NULL;

	momentaFromctd = Vector3Alloc();
	momentaFromcst = Vector3Alloc();
	momentaFromfct = Vector3Alloc();
	momentaFromftf = Vector3Alloc();
}
//----------------------------------------------------------------------------
USRFactory :: ~USRFactory(void)
{
	coordsSize = 0;
	indexOfcst = indexOffct = indexOfftf = -1;

	if(dsFromctd) free(dsFromctd);
	if(dsFromcst) free(dsFromcst);
	if(dsFromfct) free(dsFromfct);
	if(dsFromftf) free(dsFromftf);

	if(momentaFromctd) Vector3Free(momentaFromctd);
	if(momentaFromcst) Vector3Free(momentaFromcst);
	if(momentaFromfct) Vector3Free(momentaFromfct);
	if(momentaFromftf) Vector3Free(momentaFromftf);
}
//----------------------------------------------------------------------------
void USRFactory :: computeUSRReferences()
{
	ctd.set(0.0,0.0,0.0);
	for(int i = 0; i < coordsSize; i++)
	{
	    ctd.x += configCoords[i].x;
	    ctd.y += configCoords[i].y;
	    ctd.z += configCoords[i].z;
	}
	ctd.x /= coordsSize; ctd.y /= coordsSize; ctd.z /= coordsSize;

	//testing
	//outputPoint(stdout, ctd) ;

	double euclideanD = 0., closestD = 999999., farthestD = 0.;
	for(int i = 0; i < coordsSize; i++)
	{
	    euclideanD = 0;
	    euclideanD += (configCoords[i].x - ctd.x) * (configCoords[i].x - ctd.x);
	    euclideanD += (configCoords[i].y - ctd.y) * (configCoords[i].y - ctd.y);
	    euclideanD += (configCoords[i].z - ctd.z) * (configCoords[i].z - ctd.z);

	    if(euclideanD < closestD)
	    {
		closestD = euclideanD;
		indexOfcst = i;
	    }

	    if(euclideanD > farthestD)
	    {
		farthestD = euclideanD;
		indexOffct = i;
	    }
	}

	farthestD = 0.;
	for(int i = 0; i < coordsSize; i++)
	{
	    euclideanD = 0.;

	    euclideanD += (configCoords[i].x - configCoords[indexOffct].x) *
		(configCoords[i].x - configCoords[indexOffct].x);
	    euclideanD += (configCoords[i].y - configCoords[indexOffct].y) *
		(configCoords[i].y - configCoords[indexOffct].y);
	    euclideanD += (configCoords[i].z - configCoords[indexOffct].z) *
		(configCoords[i].z - configCoords[indexOffct].z);

	    if(euclideanD > farthestD)
	    {
		farthestD = euclideanD;
		indexOfftf = i;
	    }
	}
}
//----------------------------------------------------------------------------
void USRFactory :: computeDsFromReferences()
{
	computeDsFromctd();
	computeDsFromcst();
	computeDsFromfct();
	computeDsFromftf();
}
//----------------------------------------------------------------------------
void USRFactory :: computeDsFromctd()
{
	if(!dsFromctd)
	    dsFromctd = (double*) calloc(coordsSize, sizeof(double));

	for(int i = 0; i < coordsSize; i++)
	{
	    dsFromctd[i] = 0.0;

	    dsFromctd[i] += ( (configCoords[i].x - ctd.x) *
			      (configCoords[i].x - ctd.x) );
	    dsFromctd[i] += ( (configCoords[i].y - ctd.y) *
			      (configCoords[i].y - ctd.y) );
	    dsFromctd[i] += ( (configCoords[i].z - ctd.z) *
			      (configCoords[i].z - ctd.z) );

	    dsFromctd[i] = sqrt(dsFromctd[i]);
	}

    }
//----------------------------------------------------------------------------
void USRFactory :: computeDsFromcst()
{
	if(indexOfcst >= 0)
	{
	    if(!dsFromcst)
		dsFromcst = (double*) calloc(coordsSize, sizeof(double));
	    computeDsFromReference(indexOfcst, dsFromcst);
	}
}
//----------------------------------------------------------------------------
void USRFactory :: computeDsFromfct()
{
	if(indexOffct >= 0)
	{
	    if(!dsFromfct)
		dsFromfct = (double*) calloc(coordsSize, sizeof(double));
	    computeDsFromReference(indexOffct, dsFromfct);
	}
}
//----------------------------------------------------------------------------
void USRFactory :: computeDsFromftf()
{
	if(indexOfftf >= 0)
	{
	    if(!dsFromftf)
		dsFromftf = (double*) calloc(coordsSize, sizeof(double));
	    computeDsFromReference(indexOfftf, dsFromftf);
	}
}
//----------------------------------------------------------------------------
void USRFactory :: computeDsFromReference(int indexOfReferencePoint,
					      double* dsFromReferencePoint)
{
	if(indexOfReferencePoint < 0 || !dsFromReferencePoint)
	    return;

	for(int i = 0; i < coordsSize; i++)
	{
	    dsFromReferencePoint[i] = 0;
	    dsFromReferencePoint[i] += ( (configCoords[i].x - configCoords[indexOfReferencePoint].x) *
					 (configCoords[i].x - configCoords[indexOfReferencePoint].x) );
	    dsFromReferencePoint[i] += ( (configCoords[i].y - configCoords[indexOfReferencePoint].y) *
					 (configCoords[i].y - configCoords[indexOfReferencePoint].y) );
	    dsFromReferencePoint[i] += ( (configCoords[i].z - configCoords[indexOfReferencePoint].z) *
					 (configCoords[i].z - configCoords[indexOfReferencePoint].z) );

	    dsFromReferencePoint[i] = sqrt(dsFromReferencePoint[i]);
	}

    }
//----------------------------------------------------------------------------
void USRFactory :: computeMomenta()
{
	computeMomentaFromctd();
	computeMomentaFromcst();
	computeMomentaFromfct();
	computeMomentaFromftf();
}
//----------------------------------------------------------------------------
    void USRFactory :: computeMomentaFromctd()
    {
	computeMomentaOfDistribution(dsFromctd, momentaFromctd);
    }

    void USRFactory :: computeMomentaFromcst()
    {
	computeMomentaOfDistribution(dsFromcst, momentaFromcst);
    }

    void USRFactory :: computeMomentaFromfct()
    {
	computeMomentaOfDistribution(dsFromfct, momentaFromfct);
    }

    void USRFactory :: computeMomentaFromftf()
    {
	computeMomentaOfDistribution(dsFromftf, momentaFromftf);
    }
//----------------------------------------------------------------------------
void USRFactory :: computeMomentaOfDistribution(double* ds, Vector3_t momenta)
{
	if(!ds)
	    return;

	Vector3Zero(momenta);
	momenta[0] = computeMean(ds);
	momenta[1] = computeVariance(ds, momenta[0]);
	momenta[2] = computeSkewness(ds, momenta[0]);
    }
//----------------------------------------------------------------------------
double USRFactory :: computeMean(double* ds)
{
	double mean = 0.0;
	for(int i = 0; i < coordsSize; i++)
	    mean += ds[i];
	mean /= coordsSize;
	return mean;
}
//----------------------------------------------------------------------------
double USRFactory :: computeVariance(double* ds, double mean)
{
	double variance = 0.0;
	for(int i = 0; i < coordsSize; i++)
	    variance += ( (ds[i] - mean) * (ds[i] - mean) );
	variance /= coordsSize;
	return variance;
}
//----------------------------------------------------------------------------
double USRFactory :: computeSkewness(double* ds, double mean)
{
	double skewness = 0.0;
	for(int i = 0; i < coordsSize; i++)
	    skewness += ( (ds[i] - mean) * (ds[i] - mean) * (ds[i] - mean) );
	skewness /= coordsSize;
	return skewness;
}
//----------------------------------------------------------------------------
//main driver function of this factory
bool USRFactory :: ProjectConfigCoordsToMomenta(point* configCoords, const int coordsSize,
						    bool usingctd, bool usingcst,
						    bool usingfct, bool usingftf,
						    int nrMomenta,
						    double* projState, int projSize)
{
	if(!configCoords || coordsSize <= 0)
	    return false;

	this->configCoords = configCoords;
	this->coordsSize   = coordsSize;

	computeUSRReferences();
	computeDsFromReferences();
	computeMomenta();

	int i = 0;
	if(usingctd)
	    for(int j = 0; j < nrMomenta && i < projSize; j++)
		projState[i++] = momentaFromctd[j];

	if(usingcst)
	    for(int j = 0; j < nrMomenta && i < projSize; j++)
		projState[i++] = momentaFromcst[j];

	if(usingfct)
	    for(int j = 0; j < nrMomenta && i < projSize; j++)
		projState[i++] = momentaFromfct[j];

	if(usingftf)
	    for(int j = 0; j < nrMomenta && i < projSize; j++)
		projState[i++] = momentaFromftf[j];

	return true;
}
//----------------------------------------------------------------------------
//specialized driver function of this factory
bool USRFactory :: ProjectConfigCoordsToFirstMomenta(point* configCoords, const int coordsSize,
							 bool usingctd, bool usingcst,
							 bool usingfct, bool usingftf,
							 int nrMomenta,
							 double* projState, int projSize)
{
	if(!configCoords || coordsSize <= 0)
	    return false;

	this->configCoords = configCoords;
	this->coordsSize   = coordsSize;

	computeUSRReferences();
	computeDsFromReferences();
	computeMomenta();

	int i = 0;
	if(usingctd && i < projSize)
	    projState[i++] = momentaFromctd[0];

	if(usingcst & i < projSize)
	    projState[i++] = momentaFromcst[0];

	if(usingfct && i < projSize)
	    projState[i++] = momentaFromfct[0];

	if(usingftf && i < projSize)
	    projState[i++] = momentaFromftf[0];

	return true;
}
//----------------------------------------------------------------------------
    //specifically for projections in Exploration
bool USRFactory :: ProjectConfigCoordsToMomentaInOrder(
		                       point* configCoords, const int coordsSize,
							   bool usingctd, bool usingcst,
							   bool usingfct, bool usingftf,
							   int nrMomenta,
							   double* projState, int projSize)
{
	if(!configCoords || coordsSize <= 0)
	    return false;

	this->configCoords = configCoords;
	this->coordsSize   = coordsSize;

	computeUSRReferences();
	computeDsFromReferences();
	computeMomenta();

	if(projSize == 1)
	    projState[0] = momentaFromctd[0];

	if(projSize == 2)
	{
	    projState[0] = momentaFromctd[0];
	    projState[1] = momentaFromfct[0];
	}

	if(projSize == 3)
	{
	    projState[0] = momentaFromctd[0];
	    projState[1] = momentaFromfct[0];
	    projState[2] = momentaFromftf[0];
	}

	if(projSize == 4)
	{
	    projState[0] = momentaFromctd[0];
	    projState[1] = momentaFromcst[0];
	    projState[2] = momentaFromfct[0];
	    projState[3] = momentaFromftf[0];
	}

	if(projSize == 5)
	{
	    projState[0] = momentaFromctd[0];
	    projState[1] = momentaFromcst[0];
	    projState[2] = momentaFromfct[0];
	    projState[3] = momentaFromftf[0];

	    projState[4] = momentaFromctd[1];
	}

	if(projSize == 6)
	{
	    projState[0] = momentaFromctd[0];
	    projState[1] = momentaFromfct[0];
	    projState[2] = momentaFromftf[0];

	    projState[3] = momentaFromctd[1];
	    projState[4] = momentaFromfct[1];
	    projState[5] = momentaFromftf[1];
	}

	if(projSize == 7)
	{
	    projState[0] = momentaFromctd[0];
	    projState[1] = momentaFromcst[0];
	    projState[2] = momentaFromfct[0];
	    projState[3] = momentaFromftf[0];

	    projState[4] = momentaFromctd[1];
	    projState[5] = momentaFromfct[1];
	    projState[6] = momentaFromftf[1];
	}

	if(projSize == 8)
	{
	    projState[0] = momentaFromctd[0];
	    projState[1] = momentaFromcst[0];
	    projState[2] = momentaFromfct[0];
	    projState[3] = momentaFromftf[0];

	    projState[4] = momentaFromctd[1];
	    projState[5] = momentaFromcst[1];
	    projState[6] = momentaFromfct[1];
	    projState[7] = momentaFromftf[1];
	}

	if(projSize == 9)
	{
	    projState[0] = momentaFromctd[0];
	    projState[1] = momentaFromfct[0];
	    projState[2] = momentaFromftf[0];

	    projState[3] = momentaFromctd[1];
	    projState[4] = momentaFromfct[1];
	    projState[5] = momentaFromftf[1];

	    projState[6] = momentaFromctd[2];
	    projState[7] = momentaFromfct[2];
	    projState[8] = momentaFromftf[2];
	}

	if(projSize == 10)
	{
	    projState[0] = momentaFromctd[0];
	    projState[1] = momentaFromcst[0];
	    projState[2] = momentaFromfct[0];
	    projState[3] = momentaFromftf[0];

	    projState[4] = momentaFromctd[1];
	    projState[5] = momentaFromfct[1];
	    projState[6] = momentaFromftf[1];

	    projState[7] = momentaFromctd[2];
	    projState[8] = momentaFromfct[2];
	    projState[9] = momentaFromftf[2];
	}

	if(projSize == 11)
	{
	    projState[0] = momentaFromctd[0];
	    projState[1] = momentaFromcst[0];
	    projState[2] = momentaFromfct[0];
	    projState[3] = momentaFromftf[0];

	    projState[4] = momentaFromctd[1];
	    projState[5] = momentaFromcst[1];
	    projState[6] = momentaFromfct[1];
	    projState[7] = momentaFromftf[1];

	    projState[8]  = momentaFromctd[2];
	    projState[9]  = momentaFromfct[2];
	    projState[10] = momentaFromftf[2];
	}

	if(projSize == 12)
	{
	    projState[0] = momentaFromctd[0];
	    projState[1] = momentaFromcst[0];
	    projState[2] = momentaFromfct[0];
	    projState[3] = momentaFromftf[0];

	    projState[4] = momentaFromctd[1];
	    projState[5] = momentaFromcst[1];
	    projState[6] = momentaFromfct[1];
	    projState[7] = momentaFromftf[1];

	    projState[8]  = momentaFromctd[2];
	    projState[9]  = momentaFromcst[2];
	    projState[10] = momentaFromfct[2];
	    projState[11] = momentaFromftf[2];
	}

	if(projSize > 12)
	    return false;

	return true;
}
//----------------------------------------------------------------------------


}
