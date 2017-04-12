#ifndef Antipatrea__Constants_HPP_
#define Antipatrea__Constants_HPP_

#include <cmath>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Keywords and default values.
     */ 
    namespace Constants
    {
	const int NEITHER_PARALLEL_NOR_ANTI_PARALLEL = 0;
	const int PARALLEL                           = 1;
	const int ANTI_PARALLEL                      = 2;
		
	const double EPSILON         = ldexp(1.0, -36);
	const double EPSILON_SQUARED = ldexp(1.0, -72);
	const double SQRT_EPSILON    = ldexp(1.0, -18);
	
	const double RAD2DEG         = 180 / M_PI;
	const double DEG2RAD         = M_PI / 180;

	const int ID_UNDEFINED       = -1;
	
	const char KW_Grid[]       = "Grid";
	const char KW_GridNrDims[] = "NrDims";
	const char KW_GridMin[]    = "Min";
	const char KW_GridMax[]    = "Max";
	const char KW_GridDims[]   = "Dims";

	const char KW_ReadFromFile[] = "ReadFromFile";
	const char KW_WriteToFile[]  = "WriteToFile";
	const char KW_UseFile[]      = "UseFile";
	
    }	
}

#endif
    
    
    
    







