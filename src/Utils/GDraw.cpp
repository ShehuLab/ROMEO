#include "Utils/GDraw.hpp"
#include <cmath>

namespace Antipatrea
{
    void GDrawWireframe(const bool val)
    {
	if(val)
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
	    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
    }

    void GDrawColor(const double r, const double g, const double b)
    {
	glColor3d(r, g, b);
    }


    void GDrawLineWidth(const double w)
    {
	glLineWidth(w);
    }
    
    
//2D stuff	    
    void GDrawString2D(const char s[], const double x, const double y)
    {
	if(s)
	{
	    glRasterPos2d(x, y);
	    for(int i = 0; s[i] != '\0'; ++i)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
	}	
    }
    
    void GDrawPoint2D(const double x, const double y)
    {
	glBegin(GL_POINTS);
	glVertex3d(x, y, 0);
	glEnd();
    }
    void GDrawPoint2D(const double p[])
    {
	GDrawPoint2D(p[0], p[1]);
    }
    void GDrawPoints2D(const int n, const double pts[])
    {
	glBegin(GL_POINTS);
	for(int i = 0; i < n; ++i)
	    glVertex3d(pts[2 * i], pts[2 * i + 1], 0);
	glEnd();	    
    }
    
    void GDrawSegment2D(const double x1, const double y1,
			const double x2, const double y2)
    {
	glBegin(GL_LINES);
	glVertex3d(x1, y1, 0);
	glVertex3d(x2, y2, 0);
	glEnd();
    }
    void GDrawSegment2D(const double p1[], const double p2[])
    {
	GDrawSegment2D(p1[0], p1[1], p2[0], p2[1]);		
    }
    void GDrawSegment2D(const double p1p2[])
    {
	GDrawSegment2D(p1p2[0], p1p2[1], p1p2[2], p1p2[3]);		
    }
    
    void GDrawTriangle2D(const double x1, const double y1,
			 const double x2, const double y2,
			 const double x3, const double y3)
    {
	glNormal3d(0, 0, 1);		
	glBegin(GL_TRIANGLES);
	glVertex3d(x1, y1, 0);
	glVertex3d(x2, y2, 0);
	glVertex3d(x3, y3, 0);
	glEnd();
    }
    void GDrawTriangle2D(const double p1[],
			 const double p2[],
			 const double p3[])
	
    {
	GDrawTriangle2D(p1[0], p1[1], p2[0], p2[1], p3[0], p3[1]);
    }
    void GDrawTriangle2D(const double p1p2p3[])
    {
	GDrawTriangle2D(p1p2p3[0], p1p2p3[1], 
			p1p2p3[2], p1p2p3[3], 
			p1p2p3[4], p1p2p3[5]);
    }
    
    
    void GDrawAABox2D(const double minx, const double miny,
		      const double maxx, const double maxy)
    {
	glNormal3d(0, 0, 1);		
	glBegin(GL_QUADS);
	glVertex2d(minx, miny);
	glVertex2d(maxx, miny);
	glVertex2d(maxx, maxy);
	glVertex2d(minx, maxy);
	glEnd();
    }
    void GDrawAABox2D(const double min[],
		      const double max[])
    {
	GDrawAABox2D(min[0], min[1], max[0], max[1]);		
    }
    void GDrawAABox2D(const double minmax[])
    {
	GDrawAABox2D(minmax[0], minmax[1], minmax[2], minmax[3]);		
    }
    
    
    void GDrawQuad2D(const double ax, const double ay,
		     const double bx, const double by,
		     const double cx, const double cy,
		     const double dx, const double dy)
    {
	glNormal3d(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3d(ax, ay, 0);
	glVertex3d(bx, by, 0);
	glVertex3d(cx, cy, 0);
	glVertex3d(dx, dy, 0);
	glEnd();		
    }	    	    
    void GDrawQuad2D(const double a[],
		     const double b[],
		     const double c[],
		     const double d[])
    {
	GDrawQuad2D(a[0], a[1], b[0], b[1], c[0], c[1], d[0], d[1]);		
    }	    	    
    void GDrawQuad2D(const double abcd[])
    {
	GDrawQuad2D(abcd[0], abcd[1], abcd[2], abcd[3], 
		    abcd[4], abcd[5], abcd[6], abcd[7]);		
	
    }
    
    void GDrawCircle2D(const double cx, const double cy, const double r)
    {
	GDrawRegularPolygon2D(cx, cy, r, 30);
    }
    
    
    void GDrawRegularPolygon2D(const double cx, 
			       const double cy, 
			       const double r, 
			       const int nsides)
    {
	const double theta = 2 * M_PI / nsides;
	const double c     = cos(theta);
	const double s     = sin(theta);
	double       x     = 1;
	double       y     = 0;
	double       tmp   = 0.0;
	
	glNormal3d(0, 0, 1);		
	glBegin(GL_POLYGON);
	for(int i = 0; i <= nsides; i++)
	{		
	    glVertex3d(cx + r * x, cy + r * y, 0); 
	    tmp = x; x = c * tmp - s * y; y = s * tmp + c * y;		
	}
	glEnd();	
    }
    
}











