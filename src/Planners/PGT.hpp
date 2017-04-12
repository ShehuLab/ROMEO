#ifndef Antipatrea__PGT_HPP_
#define Antipatrea__PGT_HPP_

#include "Planners/TreeSamplingBasedPlanner.hpp"
#include "Components/CfgProjectors/CfgProjector.hpp"
#include "Setup/Defaults.hpp"
#include "Utils/Grid.hpp"

namespace Antipatrea
{
   /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Projection-Guided Tree (PGT).
     * 
     *@remarks
     * - PGT relies on CfgProjector to obtain a low-dimensional
     *   projection for each configuration/vertex that is added to the tree.
     * - PGT also uses a grid (over the projection space) to partition
     *   the vertices into groups. In particular, vertices <tt>v_i</tt> and 
     *   <tt>v_j</tt> belong to the same group iff their projections
     *   belong to the same grid cell.
     * - Each grid cell is assigned a weight based on the number of vertices
     *   associated with it and the number of times it has been selected
     *   for expansion. The weight is high when it has few vertices in
     *   order to promote expansions from sparsely explored areas. To avoid
     *   always selecting the same grid cell, the weight decreases with the
     *   number of selections. This is essential to ensure that the tree
     *   expansion does not become stuck when expansion from vertices 
     *   associated with a particular grid cell is difficult or impossible
     *   due to configuration constraints, e.g., energetic constraints 
     *   when configurations represent molecular structures or 
     *   collision-avoidance constraints in standard robotic motion planning.
     * - At each iteration, PGT selects a non-empty grid cell with probability
     *   proportional to its weight and then it selects a vertex uniformly 
     *   at random from the selected cell.
     * - The target is selected as in the base class, using the function
     *   SelectTarget.
     * - After the vertex and target have been selected, as in the base class,
     *   PGT uses ExpandTowardTarget to expand the tree from the selected
     *   vertex toward the selected target.
     */
    class PGT : public TreeSamplingBasedPlanner,
		public CfgProjectorContainer
    {
    public:
	PGT(void) : TreeSamplingBasedPlanner(),
		    CfgProjectorContainer(),
		    m_totalWeight(0.0)
	{
	}
	
	virtual ~PGT(void);
	
	
	virtual bool CheckSetup(void)
	{
	    return
		TreeSamplingBasedPlanner::CheckSetup() &&
		GetCfgProjector() != NULL &&
		GetCfgProjector()->CheckSetup();
	}
	
	virtual void Info(const char prefix[]) const
	{
	    TreeSamplingBasedPlanner::Info(prefix);
	    Logger::m_out << prefix << " CfgProjector = " << Component::Name(GetCfgProjector()) << std::endl
		          << prefix << " Grid         = ";
	    m_grid.Info(((std::string) prefix + " ").c_str());
	}

	

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the planner from the given parameters.
	 * 
	 *@remarks
	 * - Function first invokes TreeSamplingBasedPlanner::SetupFromParams(params).
	 * - It then sets the projection grid (keyword Constants::KW_Grid)
	 * - It uses the parameter group associated with the keyword Constants::KW_PGT.
	 * - The parameter values can be specified in a text file as, for example,
	 *     <center><tt>PGT { Grid { Dims [32 37] Min [-10 10] Max [10 10] }  }</tt></center>
	 */
	virtual void SetupFromParams(Params & params);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the pointer to the grid imposed over the projection space.
	 */
	virtual const Grid* GetProjectionGrid(void) const
	{
	    return &m_grid;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the pointer to the grid imposed over the projection space.
	 */
	virtual Grid* GetProjectionGrid(void)
	{
	    return &m_grid;
	}
	
    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Add a new vertex to the graph with <tt>cfg</tt> as its associated configuration.
	 *
	 *@remarks
	 * - The newly added vertex is also added to the corresponding grid cell
	 *   (using CfgProjector to compute the projection of <tt>cfg</tt>).
	 * - The function does not make a copy of <tt>cfg</tt>; it simply does pointer assignment.
	 * - The function does not check if <tt>cfg</tt> is already in the graph. It is the 
	 *   responsibility of the calling function to perform such a check (using FindCfg)
	 *   if indeed there is a possibility that the same configuration is added to the 
	 *   graph multiple times (which even if it happened would not cause any errors).
	 */
	virtual int AddVertex(Cfg * const cfg);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Select a vertex from which to expand the tree and 
	 *       a target toward which to expand the tree.
	 *
	 *@remarks
	 * - Function returns the id of the selected vertex.
	 * - The vertex is selected by first selecting a grid cell (among the non-empty grid cells)
	 *   with probability proportional to its weight and then selecting a vertex
	 *   uniformly at random from the vertices associated with the selected grid cell.
	 * - TreeSamplingBasedPlanner::SelectTarget is used to select the target.
	 */
	virtual int SelectVertex(void);
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Each cell keeps track of the vertices associated with it and the number of times it has been selected for expansion.
	 *
	 *@remarks
	 * - A vertex <tt>v</tt> is associated with a grid cell <tt>c</tt>
	 *   iff the projection of the configuration associated with <tt>v</tt> falls inside the cell <tt>c</tt>.
	 */
	class Cell
	{
	public:
	    Cell(void) : m_nrSel(0)
	    {
	    }

	    virtual ~Cell(void)
	    {
	    }

	    /**
	     *@author Erion Plaku, Amarda Shehu
	     *@brief Get the cell weight.
	     *
	     *@remarks
	     * - The weight is high when the cell has few vertices in
	     *   order to promote expansions from sparsely explored areas. 
	     * - To avoid always selecting the same grid cell, the weight 
	     *   decreases with the number of selections. 
	     */
	    virtual double GetWeight(void) const
	    {
		return 1.0 / (1.0 + m_nrSel + m_vids.size());
	    }

	    /**
	     *@author Erion Plaku, Amarda Shehu
	     *@brief Add a vertex to the cell.
	     */
	    virtual void AddVertex(const int vid)
	    {
		m_vids.push_back(vid);
	    }

	    /**
	     *@author Erion Plaku, Amarda Shehu
	     *@brief Select a vertex uniformly at random.
	     *
	     *@remarks
	     * - The function also incremnts the number of times the cell has been selected by one.
	     * - The function returns the id of the selected vertex.
	     */
	    virtual int SelectVertex(void) 
	    {
		++m_nrSel;
		
		return m_vids[RandomUniformInteger(0, m_vids.size() - 1)];
	    }
	    
	protected:
	    /**
	     *@author Erion Plaku, Amarda Shehu
	     *@brief Ids of vertices associated with the cell.
	     */
	    std::vector<int> m_vids;
	    
	    /**
	     *@author Erion Plaku, Amarda Shehu
	     *@brief Number of times a vertex from the cell has been selected for tree expansions.
	     */
	    int m_nrSel;
	};

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Factory for creating new cells.
	 *
	 *@remarks
	 * - PGT should use <tt>NewCell()</tt>  instead of <tt>new Cell()</tt> to 
	 *   allocate memory for a new cell, since planners extending PGT could
	 *   also extend Cell.  
	 */
	virtual Cell* NewCell(void) const
	{
	    return new Cell();
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Sum of weights of the non-empty grid cells.
	 */
	double m_totalWeight;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Grid over the projection space.
	 */
	Grid m_grid;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Non-empty grid cells.
	 */
	std::unordered_map<int, Cell*> m_cells;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to PGT.
     */
    ClassContainer(PGT, m_pgt);
    
}

#endif
