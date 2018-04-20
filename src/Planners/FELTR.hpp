/*
 * FELTR.hpp
 *
 *  Created on: Oct 3, 2017
 *      Author: kmolloy
 */

#ifndef Antipatrea__FELTR_HPP_
#define Antipatrea__FELTR_HPP_

#include "Planners/EST.hpp"
#include "Planners/TreeSamplingBasedPlanner.hpp"
#include "Setup/Defaults.hpp"
#include "PluginRosetta/CfgProjectorUSREnergy.hpp"
#include "Planners/FELTRRegion.hpp"
#include "Utils/Grid.hpp"

namespace Antipatrea
{

	class FELTRRegion;
	class FELTRCell;
    /**
     *@author Kevin Molloy, Erion Plaku, Amarda Shehu
     *@brief Vertex representation for FELTR.
     *
     *@remarks
     * - In addition to the information stored by PlannerVertex,
     *   an ESTVertex also keeps track of the number of neighbors
     *   inside a user-specified neighborhood and the
     *   number of times the vertex has been selected for expansion.
     */
    class FELTRVertex : public PlannerVertex
    {
    public:
		FELTRVertex(void) : PlannerVertex(),
		  region(NULL),cell(NULL)
		{
		}

		virtual ~FELTRVertex(void)
		{
		}
			FELTRRegion *region;
			FELTRCell    *cell;
    };


    class FELTR : public TreeSamplingBasedPlanner,public CfgProjectorContainer
    {
    public:
   	    FELTR(void) : TreeSamplingBasedPlanner()
		{
 		}

		virtual ~FELTR(void)
		{
		}


		virtual void Info(const char prefix[]) const
		{
			TreeSamplingBasedPlanner::Info(prefix);
			Logger::m_out << prefix << " FELTR parms TBD" << std::endl;
		}


		/**
		 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
		 *@brief Set the parameter values of the planner from the given parameters.
		 *
		 *@remarks
		 * - Function first invokes TreeSamplingBasedPlanner::SetupFromParams(params).
		 * - It then sets the neighborhood radius (keyword Constants::KW_NeighborhoodRadius)
		 * - It uses the parameter group associated with the keyword Constants::KW_EST.
		 * - The parameter values can be specified in a text file as, for example,
		 *     <center><tt>EST { NeighborhoodRadius 1.0 }  }</tt></center>
		 */
		virtual void SetupFromParams(Params & params)
		{
			TreeSamplingBasedPlanner::SetupFromParams(params);

			auto data = params.GetData(Constants::KW_FELTR);
			if(data && data->m_params)
			{

			}
			int granRegions[4];
			double min[4],max[4];
			for (unsigned int i(0);i < 3;++i) {
				granRegions[i] = 1;
				min[i] = 0;
				max[i] = 100;
			}
			granRegions[3] = 100;
			// For Rosetta, these numbers are bad.
			// Need to make params
			min[3] = -25.0;
			max[3] = 100.0;

			m_energyGrid.Setup(4,granRegions,min,max);

		}


		   protected:
		/**
		 *@author Erion Plaku, Amarda Shehu
		 *@brief Factory for creating new planner vertices.
		 *
		 *@remarks
		 * - Create ESTVertex instead of PlannerVertex.
		 */
		virtual PlannerVertex* NewVertex(void) const
		{
			return new FELTRVertex();
		}

	    virtual Selector<FELTRRegion *>::Node * GetEnergyRegion(double projection[]);


		/**
		 *@author Erion Plaku, Amarda Shehu
		 *@brief Add a new vertex to the graph with <tt>cfg</tt> as its associated configuration.
		 *
		 *@remarks
		 * - The function also computes the number of vertices that fall inside the
		 *   neighborhood centered at <tt>cfg</tt> (configurations whose distance from
		 *   <tt>cfg</tt> is no more than <tt>GetNeighboorhodRadius().</tt> For each
		 *   of these configurations, it also increments the number of neighbors counter
		 *   by one (since <tt>cfg</tt> will be inside their neighborhoods).
		 *
		 * - The function does not make a copy of <tt>cfg</tt>; it simply does pointer assignment.
		 * - The function does not check if <tt>cfg</tt> is already in the graph. It is the
		 *   responsibility of the calling function to perform such a check (using FindCfg)
		 *   if indeed there is a possibility that the same configuration is added to the
		 *   graph multiple times (which even if it happened would not cause any errors).
		 */
		virtual int AddVertex(Cfg * const cfg);

		/**
		 *@author Erion Plaku, Amarda Shehu
		 *@brief Get the vertex weight.
		 *
		 *@remarks
		 * - The weight is high when the vertex has few neighbors inside
		 *   its neighborhood in  order to promote expansions from sparsely explored areas.
		 * - To avoid always selecting the same vertex, the weight
		 *   decreases with the number of selections.
		 */
		//KMDEBUG virtual double GetVertexWeight(const FELTRVertex & v) const;

		/**
		 *@author Erion Plaku, Amarda Shehu
		 *@brief Select a vertex from which to expand the tree and
		 *       a target toward which to expand the tree.
		 *
		 *@remarks
		 * - Function returns the id of the selected vertex.
		 * - The vertex is selected with probability proportional to its weight.
		 * - TreeSamplingBasedPlanner::SelectTarget is used to select the target.
		 */
		virtual int SelectVertex(void);

		/**
		 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
		 *@brief FELTR projection
		 *
		 *
		 *@remarks
		 * - Variable holds the grid from which all vertices are projected.
		 */
		Grid m_grid;
		Grid m_energyGrid;
	    Selector<FELTRRegion *> m_selector;
	    std::map<int,Selector<FELTRRegion*>::Node *> m_FELTRRegionToSelectorMap;

       };
}
#endif /* SRC_PLANNERS_FELTR_HPP_ */
