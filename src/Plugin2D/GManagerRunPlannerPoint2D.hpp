#ifndef Antipatrea__GManagerRunPlannerPoint2D_HPP_
#define Antipatrea__GManagerRunPlannerPoint2D_HPP_

#include "Plugin2D/GManagerRunPlanner2D.hpp"
#include "Plugin2D/SetupPoint2D.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief GUI for running a planner for a point robot operating in a 2D scene.
     */
    class GManagerRunPlannerPoint2D : public GManagerRunPlanner2D
    {
    public:
        GManagerRunPlannerPoint2D(void) : GManagerRunPlanner2D()
        {
            m_setup = new SetupPoint2D();
        }
        
        virtual ~GManagerRunPlannerPoint2D(void)
        {
        }
        
        virtual void DrawCfg(Cfg & cfg)
        {
            GDrawCircle2D(cfg.GetValues(), 0.2);
        }

        virtual void DrawEdge(Cfg & cfg1, Cfg & cfg2)
        {
            GDrawSegment2D(cfg1.GetValues(), cfg2.GetValues());
        }
    };
}

#endif
