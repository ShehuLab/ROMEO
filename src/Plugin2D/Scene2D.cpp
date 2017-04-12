#include "Plugin2D/Scene2D.hpp"
#include "Setup/Defaults.hpp"
#include "Utils/Algebra2D.hpp"
#include "Utils/Geometry.hpp"
#include "Utils/Logger.hpp"
#include "Utils/GDraw.hpp"

namespace Antipatrea
{	
    void Scene2D::SetupFromParams(Params & params)
    {
	Component::SetupFromParams(params);

	auto data = params.GetData(Constants::KW_Scene2D);
	if(data && data->m_params)
	{
	    const char *fname = data->m_params->GetValue(Constants::KW_UseFile);
	    if(fname)
		ReadFromFile(fname);
	}
    }
    

    bool Scene2D::IsPointInCollision(const double p[]) const
    {
	const int n = GetNrObstacles();
	double    c[2];
	
	for(int i = 0; i < n; ++i)
	{
	    c[0] = GetObstacleCenterX(i);
	    c[1] = GetObstacleCenterY(i);
	    if(Algebra2D::PointDistance(c, p) <= GetObstacleRadius(i))
		return true;
	}
	return false;
    }
    
	
    bool Scene2D::IsSegmentInCollision(const double p1[], const double p2[]) const
    {
	const int n = GetNrObstacles();
	double    r;
	double    c[2];
	double    pmin[2];
		
	for(int i = 0; i < n; ++i)
	{
	    c[0] = GetObstacleCenterX(i);
	    c[1] = GetObstacleCenterY(i);
	    r    = GetObstacleRadius(i);
	    if(DistanceSquaredPointSegment2D(c, p1, p2, pmin) <= r * r)
		return true;
	}
	return false;
    }

    int Scene2D::NearestObstacleToPoint(const double p[], double & dmin) const
    {
	const int n = GetNrObstacles();
	double    d;
	double    c[2];
	int       imin = -1;
	
	
	dmin = INFINITY;
	for(int i = 0; i < n; ++i)
	{
	    c[0] = GetObstacleCenterX(i);
	    c[1] = GetObstacleCenterY(i);
	    d    = Algebra2D::PointDistance(c, p);

	    if(d < dmin)
	    {
		dmin = d;
		imin = i;
	    }
	    
	}
	return imin;
    }

    std::ostream& Scene2D::Print(std::ostream & out) const
    {
	out << m_bbox[0] << " " << m_bbox[1] << " " << m_bbox[2] << " " << m_bbox[3] << std::endl
	    << GetNrObstacles() << std::endl;
	for(int i = 0; i < GetNrObstacles(); ++i)
	    out << GetObstacleCenterX(i) << " "
		<< GetObstacleCenterY(i) << " "
		<< GetObstacleRadius(i) << std::endl;
	return out;
    }

    std::istream& Scene2D::Read(std::istream &in)
    {
	int    n;
	double x;
	double y;
	double r;

	in >> m_bbox[0] >> m_bbox[1] >> m_bbox[2] >> m_bbox[3] >> n;
	if(!in.good())
	    return in;
	
	for(int i = 0; i < n; ++i)
	{
	    in >> x >> y >> r;
	    if(!in.good())
		return in;
	    AddObstacle(x, y, r);
	}
    }
    
    void Scene2D::Draw(void)
    {
	const int n = GetNrObstacles();
	for(int i = 0; i < n; ++i)
	    GDrawCircle2D(GetObstacleCenterX(i),
			  GetObstacleCenterY(i),
			  GetObstacleRadius(i));
	
    }
    
}
