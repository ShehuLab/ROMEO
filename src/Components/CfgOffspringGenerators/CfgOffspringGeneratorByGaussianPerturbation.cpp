#include "Components/CfgOffspringGenerators/CfgOffspringGeneratorByGaussianPerturbation.hpp"
#include "Utils/PseudoRandom.hpp"
#include "Utils/Misc.hpp"

namespace Antipatrea
{        
    void CfgOffspringGeneratorByGaussianPerturbation::GenerateOffspringCfg(Cfg & cfg)
    {
        const int        nrDims = GetCfgManager()->GetDim();
        double          *vals   = cfg.GetValues();
        const double     std    = GetStep();
        int              nrToPerturb;
        std::vector<int> perm;

        GetCfgManager()->CopyValues(vals, GetParentCfg()->GetValues());
                
        if(m_maxNrValuesToPerturb > nrDims)
            m_maxNrValuesToPerturb = nrDims;
        if(m_minNrValuesToPerturb > m_maxNrValuesToPerturb)
            m_minNrValuesToPerturb = m_maxNrValuesToPerturb;
        nrToPerturb = RandomUniformInteger(m_minNrValuesToPerturb, m_maxNrValuesToPerturb);
        if(m_consecutive)
        {
            const int start = RandomUniformInteger(0, m_maxNrValuesToPerturb - nrToPerturb);
            perm.resize(nrToPerturb);
            for(int i = 0; i < nrToPerturb; ++i)
                vals[start + i] = RandomGaussianReal(GetParentCfg()->GetValues()[start + i], std);
        }
        else
        {
            perm.resize(nrDims);
            for(int i = 0; i < nrDims; ++i)
                perm[i] = i;
            PermuteItems<int>(perm, nrToPerturb);
            for(int i = 0; i < nrToPerturb; ++i)
                vals[perm[i]] = RandomGaussianReal(GetParentCfg()->GetValues()[perm[i]], std);
        }
        
        cfg.SetValues(vals);
    }
}
