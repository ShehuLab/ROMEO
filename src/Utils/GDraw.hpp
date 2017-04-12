#ifndef Antipatrea__GDraw_HPP_
#define Antipatrea__GDraw_HPP_

#ifdef __APPLE__
#include <GLUT/glut.h>
#elif defined _WIN32
#include "Utils/glutForWindows.h"
#else
#include <GL/glut.h>
#endif

namespace Antipatrea
{
   	
    void GDrawWireframe(const bool val);
  
    void GDrawColor(const double r, const double g, const double b);
    
    static inline void GDrawColor(const double * const rgb)
    {
	GDrawColor(rgb[0], rgb[1], rgb[2]);
    }

    void GDrawLineWidth(const double w);
   
    void GDrawString2D(const char s[], const double x, const double y);
    
    void GDrawPoint2D(const double x, const double y);
    
    void GDrawPoint2D(const double p[]);
    
    void GDrawPoints2D(const int n, const double pts[]);
    
    void GDrawSegment2D(const double x1, const double y1,
			const double x2, const double y2);
    
    void GDrawSegment2D(const double p1[], const double p2[]);
    
    void GDrawSegment2D(const double p1p2[]);
    
    void GDrawTriangle2D(const double x1, const double y1,
			 const double x2, const double y2,
			 const double x3, const double y3);
    
    void GDrawTriangle2D(const double p1[],
			 const double p2[],
			 const double p3[]);
    
    void GDrawTriangle2D(const double p1p2p3[]);
    
    void GDrawAABox2D(const double minx, const double miny,
		      const double maxx, const double maxy);
    
    void GDrawAABox2D(const double min[], const double max[]);
    
    void GDrawAABox2D(const double minmax[]);
    
    void GDrawQuad2D(const double ax, const double ay,
		     const double bx, const double by,
		     const double cx, const double cy,
		     const double dx, const double dy);
    
    void GDrawQuad2D(const double a[],
		     const double b[],
		     const double c[],
		     const double d[]);
    
    void GDrawQuad2D(const double abcd[]);
    
    
    void GDrawCircle2D(const double cx, const double cy, const double r);
    
    static inline
    void GDrawCircle2D(const double center[2], const double r)
    {
	GDrawCircle2D(center[0], center[1], r);
    }
    
    void GDrawRegularPolygon2D(const double cx, const double cy, const double r, const int nsides);
    
    static inline
    void GDrawRegularPolygon2D(const double center[2], const double r, const int nsides)
    {
	GDrawRegularPolygon2D(center[0], center[1], r, nsides);		
    }
    
    

}

#endif









