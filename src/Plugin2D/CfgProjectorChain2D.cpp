#include "Plugin2D/CfgProjectorChain2D.hpp"

namespace Antipatrea
{
    void CfgProjectorChain2D::Project(Cfg & cfg, double proj[])
    {
	Chain2D   *chain = GetChain2D();
	const int  n     = chain->GetNrLinks();

	chain->CopyJointValues(cfg.GetValues());
	chain->FK();
	
	proj[0] = chain->GetLinkEndX(n - 1);
	proj[1] = chain->GetLinkEndY(n - 1);
    }
    
}
