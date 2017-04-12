#ifndef  Antipatrea__GManager_HPP_
#define  Antipatrea__GManager_HPP_

#include "Utils/GCamera.hpp"
#include "Utils/GDraw.hpp"
#include "Utils/Flags.hpp"
#include "Utils/Params.hpp"
#include <vector>
#include <unordered_map>

namespace Antipatrea
{
    class GManager
    {
    public:
	GManager(void);
	
	virtual ~GManager(void);

	virtual void SetupFromParams(Params & params);
	
	virtual void Help(void);
	
	virtual void MainLoop(const char * const title, 
			      const int width = 1000, 
			      const int height=  600);
	
	static void MousePosFromScreenToWorld(const int x, const int y, 
					      double * const wx,
					      double * const wy,
					      double * const wz);
	
	virtual void HandleEventOnTimer(void)
	{
	}
	
	virtual void HandleEventOnDisplay(void)
	{
	}
	
	virtual bool HandleEventOnMouseLeftBtnDown(const int x, const int y)
	{
	    return false;
	}
	virtual bool HandleEventOnMouseLeftBtnUp(const int x, const int y)
	{
	    return false;
	}
	
	virtual bool HandleEventOnActiveMouseMove(const int x, const int y)
	{
	    return false;
	}
	
		
	virtual bool HandleEventOnPassiveMouseMove(const int x, const int y)
	{
	    return false;
	}
	
	virtual bool HandleEventOnNormalKeyPress(const int key);

	virtual bool HandleEventOnSpecialKeyPress(const int key);

	virtual bool HandleEventOnNormalKeyUp(const int key)
	{
	    return false;
	}
	
	virtual bool HandleEventOnSpecialKeyUp(const int key)
	{
	    return false;
	}
	
	virtual bool HandleEventOnIdle(void)
	{
	    return false;
	}

	
	virtual bool HandleEventOnMenu(const int item)
	{
	    return true;
	}
	

	enum
	    {
		INDEX_MINX,
		INDEX_MINY,
		INDEX_MAXX,
		INDEX_MAXY,
		INDEX_ORTHO_NEAR_PLANE,
		INDEX_ORTHO_FAR_PLANE,
		NR_INDICES
	    };
	
	virtual double GetValue(const int i)
	{
	    return m_values[i];
	}
	
	virtual void SetValue(const int i, const double val)
	{
	    m_values[i] = val;
	}
	

    protected:
	virtual void MenuOptions(void)
	{
	}
	
	static void CallbackEventOnActiveMouseMove(int x, int y);
	static void CallbackEventOnPassiveMouseMove(int x, int y);
	static void CallbackEventOnMouse(int button, int state, int x, int y);
	static void CallbackEventOnTimer(int id);
	static void CallbackEventOnMenu(int item);
	static void CallbackEventOnSpecialKeyPress(int key, int x, int y);
	static void CallbackEventOnNormalKeyPress(unsigned char key, int x, int y);
	static void CallbackEventOnSpecialKeyUp(int key, int x, int y);
	static void CallbackEventOnNormalKeyUp(unsigned char key, int x, int y);

	static void CallbackEventOnDisplay(void);
	static void CallbackEventOnIdle(void);

	
	static int  CreateMenu(void);
	static void SetMenu(const int menu);	    
	static void AddSubMenu(const char name[], const int item);	    
	static void AddMenuEntry(const char name[], const int item);	   
	static void ChangeToMenuEntry(const int pos, const char name[], const int item);
	int           m_idWindow;
	int           m_timer;
	int           m_mousePrevX;
	int           m_mousePrevY;
	GCamera       m_gCamera;
	Flags         m_flags;
	int           m_nrMenuItems;
	int           m_menuSelItem;	
	std::unordered_map<int, int> m_mapKeysToMenuItems;

	
	std::vector<double> m_values;
    };
}

#endif
