#ifndef Antipatrea__GManagerRunPlanner_HPP_
#define Antipatrea__GManagerRunPlanner_HPP_

#include "Setup/Setup.hpp"
#include "Utils/GManager.hpp"
#include "Utils/Flags.hpp"


namespace Antipatrea
{	
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief GUI for running a planner.
     */
    class GManagerRunPlanner : public GManager
    {
    public:
	GManagerRunPlanner(void);
	
	virtual ~GManagerRunPlanner(void)
	{
	    if(m_setup)
		delete m_setup;
	    
	}

	virtual void SetupFromParams(Params & params);
		
	virtual void HandleEventOnDisplay(void);
	
	virtual bool HandleEventOnNormalKeyPress(const int key);
		
	virtual bool HandleEventOnMenu(const int item);
	
	virtual void HandleEventOnTimer(void);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Draw the configuration.
	 */
	virtual void DrawCfg(Cfg & cfg) = 0;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Draw the edge connecting the two configurations.
	 */
	virtual void DrawEdge(Cfg & cfg1, Cfg & cfg2) = 0;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Pointer to the setup data structure, which contains pointers to
	 *       the planning problem, the motion planner and its components.
	 */
	Setup *m_setup;

    protected:
	virtual void MenuOptions(void);

	const char *m_plannerGraphFile;
	
	enum
	    {
		FLAG_PAUSE = 1
	    };
	
	Flags m_flags;
	int   m_solutionIndex;

        int m_idMenuMain;
	int m_idMenuPlanner;
	int MENU_ITEM_PLANNER_START;
	int MENU_ITEM_PLANNER_SOLVE;
	int MENU_ITEM_PLANNER_PRINT_GRAPH;
	int MENU_ITEM_PLANNER_READ_GRAPH;
	int MENU_ITEM_PLANNER_STATS;
	

	std::vector<Cfg*> m_cfgs;
    };
}

#endif
