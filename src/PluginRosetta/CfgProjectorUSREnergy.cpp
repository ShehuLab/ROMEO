#include <vector>

#include "PluginRosetta/CfgProjectorUSREnergy.hpp"
#include "PluginRosetta/MolecularStructureRosetta.hpp"

namespace Antipatrea
{
    void CfgProjectorUSREnergy::SetupFromParams(Params & params)
    {
    	CfgProjector::SetupFromParams(params);

        auto data = params.GetData(Constants::KW_CfgProjectorUSREnergy);
        if(data && data->m_params)
        {
        	//setup any other parameters that you may have
            //see src/Components/CfgDistances/CfgDistanceLp.hpp for an example
	    }
    }

    void CfgProjectorUSREnergy::Project(const Cfg & cfg, double proj[])
    {
    	auto cfgManager = GetCfgManager();
    	auto mol        = GetMolecularStructureRosetta();

    	std::vector<point> xyz = mol->GetAtomPositions(cfg);
    	bool usingctd = true;
    	bool usingcst = false;
    	bool usingfct = true;
    	bool usingftf = true;

    	usrFactory.ProjectConfigCoordsToFirstMomenta(&(xyz[0]),
    			xyz.size(),
                usingctd, usingcst,
                usingfct, usingftf,
                MAX_NR_MOMENTA,
                proj, m_dim-1);

    	proj[m_dim - 1] = cfg.GetEnergy();

    }
}
