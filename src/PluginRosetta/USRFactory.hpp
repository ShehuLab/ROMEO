/*
 * USRFactory.hpp
 *
 *  Created on: Oct 3, 2017
 *      Author: kmolloy
 */

#ifndef Antipatrea__USRFACTORY_HPP_
#define Antipatrea__USRFACTORY_HPP_

#include "Utils/Vector3Algebra.hpp"
#include "Utils/point.hpp"
#include <assert.h>

namespace Antipatrea
{
    #define UNDEF_COORD -999999

    #define MAX_NR_REFERENCES 4
    #define MAX_NR_MOMENTA    3

    const point undefPoint(UNDEF_COORD, UNDEF_COORD, UNDEF_COORD);

    //highest similarity:    1.0
    //highest dissimilarity: 0.0
    static inline
    double usrSimilarity(const double* p1, const double* p2, const int size)
    {
        assert(p1 && p2 && size > 0);

	    double d = 0.0;
	    for(int i = 0; i < size; ++i)
	        d += fabs(p2[i] - p1[i]); //Manhattan distance

	    return 1.0/(1.0 + 1.0/size * d);
    }

    /*
     * the n-th central moment of a distribution x is:
     *  mu_n = E[(x - mu)^n]
     * terminology: mean, variance, skewness, kurtosis
     */
    /**
    	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
    	 *@brief  Produce the Ultrafast Shape Recognization (USR)
    	 *        coordinates.
    	 *
    	 *@remarks
    	 *   usingctd --  centroid of the points
    	 *   usingcst --  point closest to the ctd (centroid)
		 * 	 usingfct --  further point from ctd
		 * 	 usingftf --  further point from ftf
    	 */
    class USRFactory
    {
    public:

	    USRFactory();

	    virtual ~USRFactory(void);

	    int GetCoordsSize()  {return coordsSize;}

	    point GetPointAtIndex (const int i)
	    {
	        return i >= 0 ? configCoords[i] : undefPoint;
	    }

	    point GetCtd()
	    {
	        return ctd;
	    }

	    point GetCst()
	    {
	        return GetPointAtIndex(indexOfcst);
	    }

	    point GetFct()
	    {
	        return GetPointAtIndex(indexOffct);
	    }

	    point GetFtf()
	    {
	        return GetPointAtIndex(indexOfftf);
	    }


	    void outputReferences(FILE* outFp)
	    {
	        outputCtd(outFp);
	        if(indexOfcst >= 0) outputCst(outFp);
	        if(indexOffct >= 0) outputFct(outFp);
	        if(indexOfftf >= 0) outputFtf(outFp);
	    }

	    void outputCtd(FILE* outFp) { outputPoint(outFp, ctd); }
	    void outputCst(FILE* outFp) { outputPoint(outFp, configCoords[indexOfcst]); }
	    void outputFct(FILE* outFp) { outputPoint(outFp, configCoords[indexOffct]); }
	    void outputFtf(FILE* outFp) { outputPoint(outFp, configCoords[indexOfftf]); }

    	//main function
	    bool ProjectConfigCoordsToMomenta(point* configCoords, const int coordsSize,
					                      bool usingctd, bool usingcst,
					                      bool usingfct, bool usingftf,
					                      int nrMomenta,
					                      double* projState, int projSize);

	    //specialized projection functions
    	bool ProjectConfigCoordsToFirstMomenta(point* configCoords, const int coordsSize,
					                           bool usingctd, bool usingcst,
					                           bool usingfct, bool usingftf,
					                           int nrMomenta,
					                           double* projState, int projSize);

	bool ProjectConfigCoordsToMomentaInOrder(point* configCoords, const int coordsSize,
						 bool usingctd, bool usingcst,
						 bool usingfct, bool usingftf,
						 int nrMomenta,
						 double* projState, int projSize);

	bool appendMomenta(char* outUSRFilename, double* projState, int projSize)
	{
	    FILE* fp = fopen(outUSRFilename, "a");
	    if(!fp)
		return false;

	    for(int i = 0; i < projSize; i++)
		fprintf(fp, "%4.6f ", projState[i]);
	    fprintf(fp, "\n");

	    fflush(fp);
	    fclose(fp);
	    return true;
	}

    protected:

	void computeUSRReferences();
	void computeDsFromReferences();
	void computeMomenta();

	void computeDsFromctd(); void computeMomentaFromctd();
	void computeDsFromcst(); void computeMomentaFromcst();
	void computeDsFromfct(); void computeMomentaFromfct();
	void computeDsFromftf(); void computeMomentaFromftf();

	void outputPoint(FILE* outFp, point pt)
	{
	    assert(outFp);
	    fprintf(outFp, "%4.6f %4.6f %4.6f", pt.x, pt.y, pt.z);
	    fprintf(outFp, "\n");
	}

	int coordsSize;
	point* configCoords;

	//reference points for the set
	point ctd;    //centroid
	int indexOfcst; //index in pointSet of closest point to ctd
	int indexOffct; //index in pointSet of farthest point from ctd
	int indexOfftf; //index in pointSet of farthest point from fct

	//distribution of point distances from each of the reference locations
	double* dsFromctd, *dsFromcst, *dsFromfct, *dsFromftf;

	//momenta for each above distribution
	//assume three momenta for each - Vector3_t
	Vector3Algebra::Vector3_t momentaFromctd, momentaFromcst, momentaFromfct, momentaFromftf;

	void computeDsFromReference(int indexOfReferencePoint,
				    double* dsFromReferencePoint);

	void computeMomentaOfDistribution(double* ds, double* momenta);
	double computeMean(double* ds);
	double computeVariance(double* ds, double mean);
	double computeSkewness(double* ds, double variance);
    };

}

#endif /* Antipatrea__USRFACTORY_HPP_ */
