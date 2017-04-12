#include "Components/CfgSamplers/CfgUniformSamplerInJointSpace.hpp"
#include "Utils/PseudoRandom.hpp"
#include <cmath>

namespace Antipatrea
{	
    bool CfgUniformSamplerInJointSpace::Sample(Cfg & cfg)
    {
	const int dim  = GetCfgManager()->GetDim();
	double   *vals = cfg.GetValues();
	
	for(int i = 0; i < dim; ++i)
	    vals[i] = RandomUniformReal(-M_PI, M_PI);
	cfg.SetValues(vals);
	
	return true;
    }
}
