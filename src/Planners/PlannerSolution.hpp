#ifndef Antipatrea__PlannerSolution_HPP_
#define Antipatrea__PlannerSolution_HPP_

#include "Components/CfgManagers/CfgManager.hpp"
#include "Utils/Reader.hpp"
#include "Utils/Writer.hpp"
#include "Utils/Misc.hpp"
#include <vector>

namespace Antipatrea
{
     /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Data structure to store the solution obtained by a motion planner.
     * 
     *@remarks
     * - Solution keeps track of the sequence of vertices in the planner graph,
     *   overall cost, and the sequence of configurations.
     * - The sequence of configurations, <tt>m_cfgs</tt> may not necessarily correspond
     *   to the sequence of vertices, <tt>m_vids</tt>. In fact, <tt>m_cfgs</tt> could contain
     *   additional configurations, which would come from discretizing the paths
     *   associated with the edges in the solution. In other words, when
     *   considering the edge <tt>(m_vids[i], m_vids[i+1])</tt>, the path from <tt>m_vids[i]</tt>
     *   to <tt>m_vids[i+1]</tt> is discretized according to a time step and the resulting
     *   configurations are added to <tt>m_cfgs</tt>.
     * - This class requires access to CfgManager to print/read configurations.
     */
    class PlannerSolution : public Component,
			    public CfgManagerContainer,
			    public Writer,
			    public Reader
    {
    public:
	PlannerSolution(void) : Component(),
				CfgManagerContainer(),
				Writer(),
				Reader(),
				m_cost(0.0)
	{
	}

	virtual ~PlannerSolution(void)
	{
	    Clear();
	    
	}

	virtual bool CheckSetup(void) const
	{
	    return
		Component::CheckSetup() &&
		GetCfgManager() != NULL &&
		GetCfgManager()->CheckSetup();
	}
	
	virtual void Info(const char prefix[]) const
	{
	    Component::Info(prefix);
	    Logger::m_out << prefix << " CfgManager = " << Name(GetCfgManager()) << std::endl;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the cost to zero, delete any previously allocated memory,
	 *       and empty the sequence of vertices and configurations.
	 */
	virtual void Clear(void)
	{
	    m_cost = 0.0;
	    m_vids.clear();
	    ClearCfgs();
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Delete any previously allocated memory and empty the sequence of configurations.
	 */
	virtual void ClearCfgs(void)
	{
	    DeleteItems<Cfg*>(m_cfgs);
	    m_cfgs.clear();
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the pointer to the vertex sequence.
	 */
	virtual const std::vector<int>* GetVertexSequence(void) const
	{
	    return &m_vids;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the pointer to the vertex sequence.
	 */
	virtual std::vector<int>* GetVertexSequence(void)
	{
	    return &m_vids;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the overall cost of the solution.
	 */	
	virtual double GetCost(void) const
	{
	    return m_cost;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the overall cost of the solution.
	 */
	virtual void SetCost(const double cost)
	{
	    m_cost = cost;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the number of configurations.
	 */
	virtual int GetNrCfgs(void) const
	{
	    return m_cfgs.size();
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get a pointer to the <tt>i</tt>-th configuration.
	 *
	 *@remarks
	 * - Index <tt>i</tt> should be between <tt>0</tt> and <tt>GetNrCfgs()-1</tt>.
	 * - The function does not check whether <tt>i</tt> is within range.
	 */
	virtual const Cfg* GetCfg(const int i) const
	{
	    return m_cfgs[i];
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get a pointer to the <tt>i</tt>-th configuration.
	 *
	 *@remarks
	 * - Index <tt>i</tt> should be between <tt>0</tt> and <tt>GetNrCfgs()-1</tt>.
	 * - The function does not check whether <tt>i</tt> is within range.
	 */
	virtual Cfg* GetCfg(const int i)
	{
	    return m_cfgs[i];
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Add a configuration to the sequence.
	 */
	virtual void AddCfg(Cfg * const cfg)
	{
	    m_cfgs.push_back(cfg);
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Print the cost, sequence of vertices, and sequence of configurations.
	 */
	virtual std::ostream& Print(std::ostream & out) const;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Read the cost, sequence of vertices, and sequence of configurations.
	 */
	virtual std::istream& Read(std::istream & in);

    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Sequence of vertices making up the solution.
	 */
	std::vector<int>  m_vids;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Solution cost.
	 */
	double m_cost;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Sequence of configurations making up the solution.
	 *
	 *@remarks
	 */
	std::vector<Cfg*> m_cfgs;
	
    };

     /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to PlannerSolution.
     */
   ClassContainer(PlannerSolution, m_plannerSolution);
    
}

#endif
