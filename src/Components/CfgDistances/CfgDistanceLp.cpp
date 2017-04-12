#include "Components/CfgDistances/CfgDistanceLp.hpp"
#include <cmath>

namespace Antipatrea
{	
    double CfgDistanceLp::Distance(const Cfg & cfg1, const Cfg & cfg2)
    {
	const int     dim   = GetCfgManager()->GetDim();
	const double *vals1 = cfg1.GetValues();
	const double *vals2 = cfg2.GetValues();
	double        s     = 0.0;

	for(int i = 0; i < dim; ++i)
	{
	    const double d = fabs(GetSignedDistanceBetweenTwoValues()->SignedDistance(vals1[i], vals2[i]));
	    s += pow(d, m_exponent);
	    
	}

	return pow(s, 1.0 / m_exponent);
	
    }
}
