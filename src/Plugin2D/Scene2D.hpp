#ifndef Antipatrea__Scene2D_HPP_
#define Antipatrea__Scene2D_HPP_

#include "Components/Component.hpp"
#include "Utils/Reader.hpp"
#include "Utils/Writer.hpp"
#include <vector>

namespace Antipatrea
{
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Scene2D is defined by its bounding box and the 2D obstacles it contains.
     *
     *@remarks
     * - The obstacles in the scene are disks (defined by their center and radius). 
     */
    class Scene2D : public Component,
		    public Reader,
		    public Writer
    {
    public:
	Scene2D(void) : Component(),
			Reader(),
			Writer()
	{
	    m_bbox[0] = -10;
	    m_bbox[1] = -10;
	    m_bbox[2] =  10;
	    m_bbox[3] =  10;
	}
	
	virtual ~Scene2D(void)
	{
	}

	
	virtual void Info(const char prefix[]) const
	{
	    Component::Info(prefix);
	    Logger::m_out << prefix << " BoundingBox and Obstacles = " << std::endl;
	    Print(Logger::m_out);
	    
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 * 
	 *@remarks
	 * - Reads the scene from the specified file (keywords Constants::KW_ReadFromFile).
	 * - It uses the parameter group associated with the keyword Constants::KW_Scene2D.
	 * - The parameter values can be specified in a text file as, for example,
	 *     <center><tt>Chain2D { NrLinks 8 LinkLength 1.0 }</tt></center>
	 */
	virtual void SetupFromParams(Params & params);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the number of the obstacles.
	 */
	virtual int GetNrObstacles(void) const
	{
	    return m_obstacles.size() / 3;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the x-coordinate of the center of the <tt>i</tt>th obstacle.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the 
	 *   range <tt>0</tt> to <tt>GetNrObstacles() - 1</tt>. 
	 */
	virtual double GetObstacleCenterX(const int i) const
	{
	    return m_obstacles[3 * i];
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the y-coordinate of the center of the <tt>i</tt>th obstacle.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the 
	 *   range <tt>0</tt> to <tt>GetNrObstacles() - 1</tt>. 
	 */
	virtual double GetObstacleCenterY(const int i) const
	{
	    return m_obstacles[3 * i + 1];
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the radius of the <tt>i</tt>th obstacle.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the 
	 *   range <tt>0</tt> to <tt>GetNrObstacles() - 1</tt>. 
	 */
	virtual double GetObstacleRadius(const int i) const
	{
	    return m_obstacles[3 * i + 2];
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the x-coordinate of the center of the <tt>i</tt>th obstacle.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the 
	 *   range <tt>0</tt> to <tt>GetNrObstacles() - 1</tt>. 
	 */
	virtual void SetObstacleCenterX(const int i, const double x)
	{
	    m_obstacles[3 * i] = x;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the y-coordinate of the center of the <tt>i</tt>th obstacle.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the 
	 *   range <tt>0</tt> to <tt>GetNrObstacles() - 1</tt>. 
	 */
	virtual void SetObstacleCenterY(const int i, const double y)
	{
	    m_obstacles[3 * i + 1] = y;
	}

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the radius of the <tt>i</tt>th obstacle.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the 
	 *   range <tt>0</tt> to <tt>GetNrObstacles() - 1</tt>. 
	 */
	virtual void SetObstacleRadius(const int i, const double r)
	{
	    m_obstacles[3 * i + 2] = r;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the center and radius of the <tt>i</tt>th obstacle.
	 *
	 *@remarks
	 * - Function does not check if <tt>i</tt> is in the 
	 *   range <tt>0</tt> to <tt>GetNrObstacles() - 1</tt>. 
	 */
	virtual void SetObstacle(const int i, const double cx, const double cy, const double r)
	{
	    SetObstacleCenterX(i, cx);
	    SetObstacleCenterX(i, cy);
	    SetObstacleCenterX(i, r);
	}
	

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Add a disk obstacle with radius <tt>r</tt> centered at <tt>(cx, cy)</tt>.
	 */
	virtual void AddObstacle(const double cx, const double cy, const double r)
	{
	    m_obstacles.push_back(cx);
	    m_obstacles.push_back(cy);
	    m_obstacles.push_back(r);
	}

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Remove the <tt>i</tt>th obstacle.
	 */
	virtual void RemoveObstacle(const int i)
	{
	    const int last = GetNrObstacles() - 1;

	    if(last < 0 || i < 0 || i > last)
		return;
	    

	    m_obstacles[3 * i    ] = m_obstacles[3 * last];
	    m_obstacles[3 * i + 1] = m_obstacles[3 * last + 1];
	    m_obstacles[3 * i + 2] = m_obstacles[3 * last + 2];

	    m_obstacles.pop_back();
	    m_obstacles.pop_back();
	    m_obstacles.pop_back();
	}
	

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Return true iff the point <tt>p</tt> is inside an obstacle.
	 */
	virtual bool IsPointInCollision(const double p[]) const;

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Return true iff the segment <tt>(p1, p2)</tt> is inside or intersects an obstacle.
	 */
	virtual bool IsSegmentInCollision(const double p1[], const double p2[]) const;

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Find the obstacle closest to the point <tt>p</tt>.
	 *
	 *@remarks
	 * - Function also computes the distance from <tt>p</tt> to the closest obstacle. 
	 */
	virtual int NearestObstacleToPoint(const double p[], double & d) const;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get a pointer to the bounding box.
	 *
	 *@remarks
	 * - The first two values represent the coordinates of the lowest-left corner (minimum).
	 * - The last two values represent the coordinates of the top-right corner (maximum).
 	 */
	virtual const double* GetBoundingBox(void) const
	{
	    return m_bbox;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get a pointer to the bounding box.
	 *
	 *@remarks
	 * - The first two values represent the coordinates of the lowest-left corner (minimum).
	 * - The last two values represent the coordinates of the top-right corner (maximum).
 	 */
	virtual double* GetBoundingBox(void)
	{
	    return m_bbox;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Print the bounding box and the obstacles to the output stream.
 	 */
	virtual std::ostream& Print(std::ostream & out) const;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Read the bounding box and the obstacles from the input stream.
 	 */
	virtual std::istream& Read(std::istream & in);

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Draw the bounding box and the obstacles.
 	 */
	virtual void Draw(void);
	
    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Disk obstacles.
	 *
	 *@remarks
	 * - The center and radius of the <tt>i</tt>th obstacle are stored in positions
	 *   <tt>3i, 3i + 1, 3i + 2</tt>, respectively.
 	 */
	std::vector<double> m_obstacles;

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Bounding box of the scene.
	 *
	 *@remarks
	 * - m_bbox[0]: minimum x-coordinate.
	 * - m_bbox[1]: minimum y-coordinate.
	 * - m_bbox[2]: maximum x-coordinate.
	 * - m_bbox[3]: maximum y-coordinate.
 	 */
	double m_bbox[4];
	
    };
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to Scene2D.
     */	 
     ClassContainer(Scene2D, m_scene2D);
    
}

#endif
