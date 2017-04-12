#ifndef Antipatrea__GManagerRunPlanner2D_HPP_
#define Antipatrea__GManagerRunPlanner2D_HPP_

#include "Programs/GManagerRunPlanner.hpp"
#include "Plugin2D/Setup2D.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief GUI for running a planner operating in a 2D scene.
     */
    class GManagerRunPlanner2D : public GManagerRunPlanner
    {
    public:
	GManagerRunPlanner2D(void);
	
	virtual ~GManagerRunPlanner2D(void)
	{
	    if(m_setup)
		delete m_setup;
	    
	}

	virtual void SetupFromParams(Params & params);

	virtual void HandleEventOnDisplay(void);
	
	virtual bool HandleEventOnMouseLeftBtnDown(const int x, const int y);
	
	virtual bool HandleEventOnMouseLeftBtnUp(const int x, const int y);
	
	virtual bool HandleEventOnActiveMouseMove(int x, int y);
		
	virtual bool HandleEventOnMenu(const int item);
		
    protected:
	virtual void MenuOptions(void);
	
	int m_idMenuScene;	
	int MENU_ITEM_SCENE_ADD_OBSTACLE;
	int MENU_ITEM_SCENE_EDIT_OBSTACLE;
	int MENU_ITEM_SCENE_MOVE_OBSTACLE;
	int MENU_ITEM_SCENE_REMOVE_OBSTACLE;
	int MENU_ITEM_SCENE_SAVE;
	int MENU_ITEM_SCENE_LOAD;
	int m_sceneSelObstacle;

	const char *m_sceneFile;
	
    };
}

#endif
