#include "Utils/GManager.hpp"
#include "Utils/Constants.hpp"
#include "Utils/Params.hpp"
#include <cstdlib>
#include <cmath>
#include <cstring>
#include "Utils/Logger.hpp"

namespace Antipatrea
{
    GManager *m_gManager = NULL;
    
    void GManager::CallbackEventOnActiveMouseMove(int x, int y)
    {
	if(m_gManager)
	{		
	    if(m_gManager->HandleEventOnActiveMouseMove(x, y))
		glutPostRedisplay();	    
	    m_gManager->m_mousePrevX = x;
	    m_gManager->m_mousePrevY = y;
	}
    }
    
    void GManager::CallbackEventOnPassiveMouseMove(int x, int y)
    {
	if(m_gManager)
	{	
	    if(m_gManager->HandleEventOnPassiveMouseMove(x, y))
		glutPostRedisplay();	    
	    m_gManager->m_mousePrevX = x;
	    m_gManager->m_mousePrevY = y;
	}
    }
    
    void GManager::CallbackEventOnMouse(int button, int state, int x, int y)
    {
	if(m_gManager)
	{
	    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
	       m_gManager->HandleEventOnMouseLeftBtnDown(x, y))
		glutPostRedisplay();
	    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP &&
	       m_gManager->HandleEventOnMouseLeftBtnUp(x, y))
		glutPostRedisplay();
	    
	    m_gManager->m_mousePrevX = x;
	    m_gManager->m_mousePrevY = y;
	}
    }
    
    void GManager::CallbackEventOnTimer(int id)
    {
	if(m_gManager)
	{
	    m_gManager->HandleEventOnTimer();
	    glutTimerFunc(m_gManager->m_timer, CallbackEventOnTimer, id);
	    glutPostRedisplay();	    
	}
    }

    void GManager::CallbackEventOnIdle(void)
    {
	if(m_gManager && m_gManager->HandleEventOnIdle())
	    glutPostRedisplay();	    
	
    }
    
    void GManager::CallbackEventOnSpecialKeyPress(int key, int x, int y)
    {
	if(m_gManager && m_gManager->HandleEventOnSpecialKeyPress(key))
	    glutPostRedisplay();
    }
    
    void GManager::CallbackEventOnNormalKeyPress(unsigned char key, int x, int y)
    {
	if(m_gManager && m_gManager->HandleEventOnNormalKeyPress(key))
	    glutPostRedisplay();
    }
    

    void GManager::CallbackEventOnSpecialKeyUp(int key, int x, int y)
    {
	if(m_gManager && m_gManager->HandleEventOnSpecialKeyUp(key))
	    glutPostRedisplay();
    }
    
    void GManager::CallbackEventOnNormalKeyUp(unsigned char key, int x, int y)
    {
	if(m_gManager && m_gManager->HandleEventOnNormalKeyUp(key))
	    glutPostRedisplay();
    }
    
    
    void  GManager::CallbackEventOnDisplay(void)
    {
	if(m_gManager)
	{
	    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	    glClearDepth(1.0);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
	  
	    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();

	    glOrtho(m_gManager->m_values[INDEX_MINX],
		    m_gManager->m_values[INDEX_MAXX],
		    m_gManager->m_values[INDEX_MINY],
		    m_gManager->m_values[INDEX_MAXY],
		    m_gManager->m_values[INDEX_ORTHO_NEAR_PLANE],
		    m_gManager->m_values[INDEX_ORTHO_FAR_PLANE]);
	    
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();

	    glDisable(GL_TEXTURE_2D);
	    glDisable(GL_LIGHTING);
	    
	    m_gManager->HandleEventOnDisplay();
	    
	    glutSwapBuffers();	    
	}
    }
    
    GManager::GManager(void) 
    {
	m_idWindow      = -1;
	m_timer         = 5;
	m_mousePrevX    = 0;
	m_mousePrevY    = 0;	    
	m_flags         = 0;
	m_nrMenuItems   = 0;
	m_menuSelItem   = -1;
	
	m_values.resize(NR_INDICES);
	m_values[INDEX_MINX]                   = -40;
	m_values[INDEX_MINY]                   = -40;
	m_values[INDEX_MAXX]                   =  40;
	m_values[INDEX_MAXY]                   =  40;
	m_values[INDEX_ORTHO_NEAR_PLANE]       = -10;
	m_values[INDEX_ORTHO_FAR_PLANE]        =  10;
    }
    
    GManager::~GManager(void)
    {
	if(m_idWindow >= 0)
	    glutDestroyWindow(m_idWindow);	
    }
    
    void GManager::SetupFromParams(Params & params)
    {
	m_values[INDEX_MINX]                   = params.GetValueAsDouble("GraphicsMinX",                 m_values[INDEX_MINX]);
	m_values[INDEX_MINY]                   = params.GetValueAsDouble("GraphicsMinY",                 m_values[INDEX_MINY]);
	m_values[INDEX_MAXX]                   = params.GetValueAsDouble("GraphicsMaxX",                 m_values[INDEX_MAXX]);
	m_values[INDEX_MAXY]                   = params.GetValueAsDouble("GraphicsMaxY",                 m_values[INDEX_MAXY]);
	m_values[INDEX_ORTHO_NEAR_PLANE]       = params.GetValueAsDouble("GraphicsOrthoNearPlane",       m_values[INDEX_ORTHO_NEAR_PLANE]);
	m_values[INDEX_ORTHO_FAR_PLANE]        = params.GetValueAsDouble("GraphicsOrthoFarPlane",        m_values[INDEX_ORTHO_FAR_PLANE]);
	m_timer                                = params.GetValueAsInt("GraphicsOnTimerInterval", m_timer);
	
	m_gCamera.SetupFromParams(params);
    }
    
    
    void GManager::MainLoop(const char * const title, const int width, const int height)
    {	
	m_gManager = this;
	
	static int    argc = 1;	
	static char  *args = (char*)"args";
	
	glutInit(&argc, &args);    
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);    
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0); 
	m_idWindow = glutCreateWindow(title);	   	
	
	glutDisplayFunc(CallbackEventOnDisplay);
	glutMouseFunc(CallbackEventOnMouse);
	glutMotionFunc(CallbackEventOnActiveMouseMove);
	glutPassiveMotionFunc(CallbackEventOnPassiveMouseMove);
	glutIdleFunc(CallbackEventOnIdle);
	glutTimerFunc(0, CallbackEventOnTimer, 0); 
	glutKeyboardFunc(CallbackEventOnNormalKeyPress);
	glutSpecialFunc(CallbackEventOnSpecialKeyPress);
	glutKeyboardUpFunc(CallbackEventOnNormalKeyUp);
	glutSpecialUpFunc(CallbackEventOnSpecialKeyUp);

	MenuOptions();
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();	
    }
    
    bool GManager::HandleEventOnNormalKeyPress(const int key)
    {
	if(key == 27) //escape key
	{
	    exit(0);
	    return true;
	}
	else if(glutGetModifiers() & GLUT_ACTIVE_ALT)
	{
	}
	
	return false;
    }
    
    void GManager::Help(void)
    {
    }
    
    
    
    bool GManager::HandleEventOnSpecialKeyPress(const int key)
    {	
	if(key == GLUT_KEY_F1)
	{
	    Help();
	    return true;
	}
	
	return false;
    }
    
 
    void GManager::MousePosFromScreenToWorld(const int x, const int y, 
					     double * const wx,
					     double * const wy,
					     double * const wz)
    {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ=0;
	double px = 0, py = 0, pz = 0;
	
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );
	
	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &px, &py, &pz);
	
	if(wx)
	    *wx = px;
	if(wy)
	    *wy = py;
	if(wz)
	    *wz = pz;    
    }
    
    void GManager::CallbackEventOnMenu(int item)
    {
	if(m_gManager && m_gManager->HandleEventOnMenu(item))
	    glutPostRedisplay();
    }

    int GManager::CreateMenu(void)
    {
	return glutCreateMenu(CallbackEventOnMenu);
    }
    
    void GManager::SetMenu(const int menu)
    {
	glutSetMenu(menu);	    
    }
    
    void GManager::AddSubMenu(const char name[], const int menu)
    {
	glutAddSubMenu(name, menu);	    
    }
    
    void GManager::AddMenuEntry(const char name[], const int item)
    {
	glutAddMenuEntry(name, item);
    }
    
    void GManager::ChangeToMenuEntry(const int pos, const char name[], const int item)
    {
	glutChangeToMenuEntry(pos, name, item);	    
    }

}



