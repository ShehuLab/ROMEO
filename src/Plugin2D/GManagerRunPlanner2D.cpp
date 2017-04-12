#include "Plugin2D/GManagerRunPlanner2D.hpp"
#include "Utils/Constants.hpp"
#include "Utils/Algebra2D.hpp"

namespace Antipatrea
{	
    void GManagerRunPlanner2D::SetupFromParams(Params & params)
    {
	GManagerRunPlanner::SetupFromParams(params);
	auto data = params.GetData(Constants::KW_Scene2D);
	if(data && data->m_params)
	    m_sceneFile = data->m_params->GetValue(Constants::KW_UseFile, m_sceneFile);
    }
    

    GManagerRunPlanner2D::GManagerRunPlanner2D(void) : GManagerRunPlanner()
    {
	m_sceneFile = Constants::VAL_SceneFile;
	
	m_sceneSelObstacle = Constants::ID_UNDEFINED;
	m_idMenuScene      = Constants::ID_UNDEFINED;
	
	MENU_ITEM_SCENE_ADD_OBSTACLE         = ++m_nrMenuItems;
	MENU_ITEM_SCENE_EDIT_OBSTACLE        = ++m_nrMenuItems;
	MENU_ITEM_SCENE_MOVE_OBSTACLE        = ++m_nrMenuItems;
	MENU_ITEM_SCENE_SAVE                 = ++m_nrMenuItems;
	MENU_ITEM_SCENE_LOAD                 = ++m_nrMenuItems;
	
	m_mapKeysToMenuItems.insert(std::make_pair((int) 'a', MENU_ITEM_SCENE_ADD_OBSTACLE));
	m_mapKeysToMenuItems.insert(std::make_pair((int) 'e', MENU_ITEM_SCENE_EDIT_OBSTACLE));
	m_mapKeysToMenuItems.insert(std::make_pair((int) 'm', MENU_ITEM_SCENE_MOVE_OBSTACLE));
	m_mapKeysToMenuItems.insert(std::make_pair((int) 'r', MENU_ITEM_SCENE_REMOVE_OBSTACLE));
    	m_mapKeysToMenuItems.insert(std::make_pair((int) 's', MENU_ITEM_SCENE_SAVE));
    	m_mapKeysToMenuItems.insert(std::make_pair((int) 'l', MENU_ITEM_SCENE_LOAD));
    }
	
    void GManagerRunPlanner2D::HandleEventOnDisplay(void)
    {
	Scene2D *scene = dynamic_cast<Setup2D*>(m_setup)->GetScene2D();

	const double *bbox = scene->GetBoundingBox();
	const double off   = 0.002;
	
	SetValue(INDEX_MINX, bbox[0] - off * (bbox[2] - bbox[0]));
	SetValue(INDEX_MINY, bbox[1] - off * (bbox[3] - bbox[1]));
	SetValue(INDEX_MAXX, bbox[2] + off * (bbox[2] - bbox[0]));
	SetValue(INDEX_MAXY, bbox[3] + off * (bbox[3] - bbox[1]));

	GManagerRunPlanner::HandleEventOnDisplay();

	GDrawColor(0.0, 0.0, 1.0);
	scene->Draw();
    }
    
    bool GManagerRunPlanner2D::HandleEventOnMouseLeftBtnDown(const int x, const int y)
    {
	double p[3];
	double dmin;

	Scene2D *scene = dynamic_cast<Setup2D*>(m_setup)->GetScene2D();
	
	if(m_menuSelItem == MENU_ITEM_SCENE_ADD_OBSTACLE)
	{
	    MousePosFromScreenToWorld(x, y, &p[0], &p[1], &p[2]);
	    scene->AddObstacle(p[0], p[1], 1.0);
	    m_menuSelItem = Constants::ID_UNDEFINED;
	    return true;
	}
	else if(m_menuSelItem == MENU_ITEM_SCENE_REMOVE_OBSTACLE)
	{
	    MousePosFromScreenToWorld(x, y, &p[0], &p[1], &p[2]);
	    m_sceneSelObstacle = scene->NearestObstacleToPoint(p, dmin);
	    if(m_sceneSelObstacle >= 0 && dmin <= scene->GetObstacleRadius(m_sceneSelObstacle))
		scene->RemoveObstacle(m_sceneSelObstacle);
	    m_sceneSelObstacle = Constants::ID_UNDEFINED;
	    m_menuSelItem      = Constants::ID_UNDEFINED;
	    return true;
	}
	else if(m_menuSelItem == MENU_ITEM_SCENE_EDIT_OBSTACLE ||
		m_menuSelItem == MENU_ITEM_SCENE_MOVE_OBSTACLE)
	{
	    MousePosFromScreenToWorld(x, y, &p[0], &p[1], &p[2]);
	    m_sceneSelObstacle = scene->NearestObstacleToPoint(p, dmin);
	    if(m_sceneSelObstacle >= 0 && dmin > scene->GetObstacleRadius(m_sceneSelObstacle))
	    {
		m_sceneSelObstacle = Constants::ID_UNDEFINED;
		m_menuSelItem      = Constants::ID_UNDEFINED;
	    }
	    return true;
	}
	else
	    return GManagerRunPlanner::HandleEventOnMouseLeftBtnDown(x, y);
    }
    
    bool GManagerRunPlanner2D::HandleEventOnMouseLeftBtnUp(const int x, const int y)
    {
	double p[3];
	double dmin;
	
	if((m_menuSelItem == MENU_ITEM_SCENE_EDIT_OBSTACLE ||
	    m_menuSelItem == MENU_ITEM_SCENE_MOVE_OBSTACLE) && m_sceneSelObstacle >= 0)
	{
	    m_sceneSelObstacle = Constants::ID_UNDEFINED;
	    m_menuSelItem      = Constants::ID_UNDEFINED;
	    return true;
	}
	else
	    return GManagerRunPlanner::HandleEventOnMouseLeftBtnDown(x, y);
    }
    
    bool GManagerRunPlanner2D::HandleEventOnActiveMouseMove(int x, int y)
    {
	double p[3];
	double c[2];
	
	Scene2D *scene = dynamic_cast<Setup2D*>(m_setup)->GetScene2D();
		
	if(m_menuSelItem == MENU_ITEM_SCENE_EDIT_OBSTACLE && m_sceneSelObstacle >= 0)
	{
	    c[0] = scene->GetObstacleCenterX(m_sceneSelObstacle);
	    c[1] = scene->GetObstacleCenterY(m_sceneSelObstacle);
	    
	    MousePosFromScreenToWorld(x, y, &p[0], &p[1], &p[2]);
	    scene->SetObstacleRadius(m_sceneSelObstacle, Algebra2D::PointDistance(p, c));
	    return true;
	}
	else if(m_menuSelItem == MENU_ITEM_SCENE_MOVE_OBSTACLE && m_sceneSelObstacle >= 0)
	{
	    MousePosFromScreenToWorld(x, y, &p[0], &p[1], &p[2]);
	    scene->SetObstacleCenterX(m_sceneSelObstacle, p[0]);
	    scene->SetObstacleCenterY(m_sceneSelObstacle, p[1]);
	    return true;
	}
	else
	    return GManagerRunPlanner::HandleEventOnActiveMouseMove(x, y);
	
    }
    
    
    
    bool GManagerRunPlanner2D::HandleEventOnMenu(const int item)
    {
	Scene2D *scene = dynamic_cast<Setup2D*>(m_setup)->GetScene2D();
	
	m_menuSelItem = item;
	if(item == MENU_ITEM_SCENE_EDIT_OBSTACLE)
	{
	    m_sceneSelObstacle = Constants::ID_UNDEFINED;
	    return true;
	}
	else if(item == MENU_ITEM_SCENE_MOVE_OBSTACLE)
	{
	    m_sceneSelObstacle = Constants::ID_UNDEFINED;
	    return true;
	}	
	else if(item == MENU_ITEM_SCENE_REMOVE_OBSTACLE)
	{
	    m_sceneSelObstacle = Constants::ID_UNDEFINED;
	    return true;
	}
	else if(item == MENU_ITEM_SCENE_SAVE)
	{
	    scene->PrintToFile(m_sceneFile);
	    return true;
	}
	else if(item == MENU_ITEM_SCENE_LOAD)
	{
	    scene->ReadFromFile(m_sceneFile);
	    return true;
	}
	else
	    return GManagerRunPlanner::HandleEventOnMenu(item);
    }
    
    void GManagerRunPlanner2D::MenuOptions(void)
    {
	m_idMenuScene = CreateMenu();
	AddMenuEntry("a. Add obstacle",    MENU_ITEM_SCENE_ADD_OBSTACLE);
	AddMenuEntry("e. Edit radius obstacle",   MENU_ITEM_SCENE_EDIT_OBSTACLE);
	AddMenuEntry("m. Move obstacle",   MENU_ITEM_SCENE_MOVE_OBSTACLE);
	AddMenuEntry("r. Remove obstacle", MENU_ITEM_SCENE_REMOVE_OBSTACLE);
	AddMenuEntry("s. Save", MENU_ITEM_SCENE_SAVE);
 	AddMenuEntry("l. Load",   MENU_ITEM_SCENE_LOAD);


	GManagerRunPlanner::MenuOptions();
	
	AddSubMenu("Scene", m_idMenuScene);
    }
}

