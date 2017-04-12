#ifndef Antipatrea__GManagerRunPlannerChain2D_HPP_
#define Antipatrea__GManagerRunPlannerChain2D_HPP_

#include "Plugin2D/GManagerRunPlanner2D.hpp"
#include "Plugin2D/SetupChain2D.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief GUI for running a planner for a chain operating in a 2D scene.
     */
    class GManagerRunPlannerChain2D : public GManagerRunPlanner2D
    {
    public:
	GManagerRunPlannerChain2D(void) : GManagerRunPlanner2D()
	{
	    m_setup = new SetupChain2D();
	}
	
	virtual ~GManagerRunPlannerChain2D(void)
	{
	}

	virtual void DrawCfg(Cfg & cfg)
	{
	    dynamic_cast<SetupChain2D*>(m_setup)->GetChain2D()->CopyJointValues(cfg.GetValues());
	    dynamic_cast<SetupChain2D*>(m_setup)->GetChain2D()->FK();
	    dynamic_cast<SetupChain2D*>(m_setup)->GetChain2D()->Draw();
	}
	
	virtual void DrawEdge(Cfg & cfg1, Cfg & cfg2)
	{
	    const int n = dynamic_cast<SetupChain2D*>(m_setup)->GetChain2D()->GetNrLinks();
	    
	    dynamic_cast<SetupChain2D*>(m_setup)->GetChain2D()->CopyJointValues(cfg1.GetValues());
	    dynamic_cast<SetupChain2D*>(m_setup)->GetChain2D()->FK();
	    
	    const double x1 = dynamic_cast<SetupChain2D*>(m_setup)->GetChain2D()->GetLinkEndX(n-1);
	    const double y1 = dynamic_cast<SetupChain2D*>(m_setup)->GetChain2D()->GetLinkEndY(n-1);
	    
	    dynamic_cast<SetupChain2D*>(m_setup)->GetChain2D()->CopyJointValues(cfg2.GetValues());
	    dynamic_cast<SetupChain2D*>(m_setup)->GetChain2D()->FK();
	    
	    const double x2 = dynamic_cast<SetupChain2D*>(m_setup)->GetChain2D()->GetLinkEndX(n-1);
	    const double y2 = dynamic_cast<SetupChain2D*>(m_setup)->GetChain2D()->GetLinkEndY(n-1);
	    
	    GDrawSegment2D(x1, y1, x2, y2);
	    
	}
	
    };
}

#endif
