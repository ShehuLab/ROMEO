#ifndef Antipatrea__PlannerEdge_HPP_
#define Antipatrea__PlannerEdge_HPP_

#include "Components/CfgManagers/Cfg.hpp"
#include "Utils/Constants.hpp"
#include "Utils/Misc.hpp"
#include <vector>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Edge representation in the graph (roadmap or tree)
     *       constructed by a sampling-based motion planner.
     *
     *@remarks
     *  - An edge <tt>(vidFrom, vidTo)</tt> indicates that
     *    the vertices <tt>vidFrom</tt> and <tt>vidTo</tt> are connected.
     *  - Edge allows for nonsymmetric costs, so it keeps track
     *    of both the cost to go from <tt>vidFrom</tt> to <tt>vidTo</tt> and the
     *    cost to go from <tt>vidTo</tt> to <tt>vidFrom</tt>.
     */
    class PlannerEdge
    {
    public:
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Edge indices.
	 */
   	enum Index
	    {
		INDEX_FROM    = 0,
		INDEX_TO      = 1,
		INDEX_FROM_TO = 0,
		INDEX_TO_FROM = 1
	    };
	
	PlannerEdge(void) 
	{
	    m_vids[INDEX_FROM] = m_vids[INDEX_TO] = Constants::ID_UNDEFINED;
	    m_costs[INDEX_FROM] = m_costs[INDEX_TO] = 0.0;
	}

	    
	virtual ~PlannerEdge(void)
	{
	    DeleteItems<Cfg*>(m_cfgs);
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Name says it all.
	 */
	virtual int GetVertexId(const Index i) const
	{
	    return m_vids[i];
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Name says it all.
	 */
	virtual double GetCost(const Index i) const
	{
	    return m_costs[i];
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Name says it all.
	 */
	virtual void SetVertexId(const Index i, const int vid)
	{
	    m_vids[i] = vid;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Name says it all.
	 */
	virtual void SetFromToVertexIds(const int vidFrom, const int vidTo)
	{
	    SetVertexId(INDEX_FROM, vidFrom);
	    SetVertexId(INDEX_TO, vidTo);
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Name says it all.
	 */
	virtual void SetCost(const Index i, const double cost)
	{
	    m_costs[i] = cost;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Name says it all.
	 */
	virtual void SetCosts(const double costFromTo, const double costToFrom)
	{
	    SetCost(INDEX_FROM_TO, costFromTo);
	    SetCost(INDEX_TO_FROM, costToFrom);
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get a pointer to the sequence of configurations connecting the end configurations of the edge.
	 */
	virtual const std::vector<Cfg*>* GetIntermediateCfgs(void) const
	{
	    return &m_cfgs;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get a pointer to the sequence of configurations connecting the end configurations of the edge.
	 */
	virtual std::vector<Cfg*>* GetIntermediateCfgs(void) 
	{
	    return &m_cfgs;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the number of intermediate configurations in the sequence connecting the end configurations of the edge.
	 */
	virtual int GetNrIntermediateCfgs(void) const
	{
	    return m_cfgs.size();
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get a pointer to the <tt>i</tt>th intermediate configuration in the sequence connecting the end configurations of the edge.
	 */
	virtual const Cfg* GetIntermediateCfg(const Index index, const int i) const
	{
	    return
		index == INDEX_FROM ? m_cfgs[i] : m_cfgs[m_cfgs.size() - 1 - i];
	}
	
	    
    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Id of the from and to vertices.
	 */
	int m_vids[2];
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief The costs for <tt>(vidFrom, vidTo)</tt> and <tt>(vidTo, vidFrom)</tt>.
	 */
	double m_costs[2];
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Sequence of configurations connecting <tt>m_vidFrom</tt> to <tt>m_vidTo</tt>.
	 */
	std::vector<Cfg*> m_cfgs;
    };
    
}

#endif
