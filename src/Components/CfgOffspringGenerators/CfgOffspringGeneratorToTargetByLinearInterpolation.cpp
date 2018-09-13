#include "Components/CfgOffspringGenerators/CfgOffspringGeneratorToTargetByLinearInterpolation.hpp"
#include <cmath>

namespace Antipatrea
{        
    void CfgOffspringGeneratorToTargetByLinearInterpolation::GenerateOffspringCfg(Cfg & cfg)
    {
        const double  t        = GetStep();
        const int     dim      = GetCfgManager()->GetDim();
        const double *valsFrom = GetParentCfg()->GetValues();
        const double *valsTo   = GetTargetCfg()->GetValues();
        double       *vals     = cfg.GetValues();
        
        for(int i = 0; i < dim; ++i)
            vals[i] = valsFrom[i] + t * GetSignedDistanceBetweenTwoValues()->SignedDistance(valsFrom[i], valsTo[i]);
        cfg.SetValues(vals);
    }
}
