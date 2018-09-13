#include "PluginRosetta/CfgSamplerRosetta.hpp"

namespace Antipatrea
{
    void CfgSamplerRosetta::SetupFromParams(Params & params)
    {
        CfgSampler::SetupFromParams(params);

        auto data = params.GetData(Constants::KW_CfgSamplerRosetta);
        if(data && data->m_params)
        {
            //setup any other parameters that you may have
            //see src/Components/CfgDistances/CfgDistanceLp.hpp for an example
        }
    }

    bool CfgSamplerRosetta::Sample(Cfg & cfg)
    {
        auto cfgManager = GetCfgManager();
        auto mol        = GetMolecularStructureRosetta();
        const int dim   = cfgManager->GetDim(); //number of configuration dimensions;
        double   *vals  = cfg.GetValues(); //values of the configuration


        for (unsigned int i=0;i < mol->GetNrResidues();++i)
        {
            vals[i*3 + 0] =  RandomUniformReal(-M_PI, M_PI);
            vals[i*3 + 1] =  RandomUniformReal(-M_PI, M_PI);
            vals[i*3 + 2] =  M_PI;
        }

        //don't forget at the end to save
        //so that cfg remembers that the values have been changed.
        //In such cases, it sets the energy to undefined as an indication that it may need to be computed again.

        cfg.SetValues(vals);

        
        //true: sample was successfully generated
        //false: failed to generate a sample
        return true;
    }

}
