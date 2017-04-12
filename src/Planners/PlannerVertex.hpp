#ifndef Antipatrea__PlannerVertex_HPP_
#define Antipatrea__PlannerVertex_HPP_

#include "Planners/PlannerEdge.hpp"
#include "Components/CfgManagers/Cfg.hpp"
#include "Utils/DisjointSet.hpp"
#include "Utils/Constants.hpp"
#include <unordered_set>

namespace Antipatrea
{	
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Vertex representation in the graph (roadmap or tree)
     *       constructed by a sampling-based motion planner.
     *
     *@remarks
     * - Each vertex has the following:
     *    - a configuration;
     *    - a flag to indicate whether or not it is a goal vertex, i.e.,
     *      the associated configuration satisfies the goal;
     *    - a set of edges;
     *    - a pointer to a disjoint-set element used to keep track
     *      of the connected components in the planner graph;
     *    - a set of attempts representing the attempts (successful or failed)  the planner
     *      has made to connect this vertex to other vertices.
     * - The vertex deletes the configuration and the disjoint-set element
     *   when it is destructed.
     */  
    class PlannerVertex
    {
    public:
	PlannerVertex(void) : m_cfg(NULL),
			      m_dsetElem(NULL),
			      m_isGoal(false)
	{
	}

	virtual ~PlannerVertex(void)
	{
	    if(m_cfg)
		delete m_cfg;
	    if(m_dsetElem)
		delete m_dsetElem;
	}
	
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get pointer to configuration.
	 */
 	virtual const Cfg* GetCfg(void) const
	{
	    return m_cfg;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get pointer to configuration.
	 */
	virtual Cfg* GetCfg(void)
	{
	    return m_cfg;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set pointer to configuration.
	 */
	virtual void SetCfg(Cfg * const cfg)
	{
	    m_cfg = cfg;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the ids of the vertices to which it is connected.
	 */
	virtual const std::unordered_set<int>* GetConnections(void) const
	{
	    return &m_connections;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the ids of the vertices to which it is connected.
	 */
	virtual std::unordered_set<int>* GetConnections(void) 
	{
	    return &m_connections;
	}
		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get pointer to disjoint-set element.
	 */
	virtual const DisjointSet::Elem* GetDisjointSetElem(void) const
	{
	    return m_dsetElem;
	}

		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get pointer to disjoint-set element.
	 */
	virtual DisjointSet::Elem* GetDisjointSetElem(void)
	{
	    return m_dsetElem;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set pointer to disjoint-set element.
	 */
	virtual void SetDisjointSetElem(DisjointSet::Elem * const dsetElem)
	{
	    m_dsetElem = dsetElem;
	}

		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the attempts (successful or failed)  the planner
	 *       has made to connect this vertex to other vertices.
	 */
	virtual const std::unordered_set<int>* GetConnectionAttempts(void) const
	{
	    return &m_attempts;
	}

		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the attempts (successful or failed)  the planner
	 *       has made to connect this vertex to other vertices.
	 */
	virtual std::unordered_set<int>* GetConnectionAttempts(void)
	{
	    return &m_attempts;
	}

	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Indicate whether the vertex is marked as a goal vertex.
	 */
	virtual bool IsGoal(void) const
	{
	    return m_isGoal;
	}
		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the goal mark for the vertex to true/false.
	 */
	virtual void MarkAsGoal(const bool isGoal)
	{
	    m_isGoal = isGoal;
	}
	
    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Configuration.
	 */
	Cfg *m_cfg;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Ids of the vertices to which it is connected.
	 */
	std::unordered_set<int>  m_connections;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Disjoint-set element which is used to keep track of the
	 *       connected components in the planner grapj.
	 */
	DisjointSet::Elem  *m_dsetElem;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Attempts (successful or failed)  the planner
	 *       has made to connect this vertex to other vertices.
	 */	
	std::unordered_set<int> m_attempts;
		
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Flag to indicate whether or not it is a goal vertex, i.e.,
	 *       the associated configuration satisfies the goal.
	 */
	bool  m_isGoal;
    };

}

#endif
