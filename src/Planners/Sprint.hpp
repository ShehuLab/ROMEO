/*
 * Sprint.hpp
 *
 *  Created on: Sept 17, 2018
 *      Author: kmolloy
 */

#ifndef Antipatrea__Sprint_HPP_
#define Antipatrea__Sprint_HPP_

#include "Planners/EST.hpp"
#include "Planners/TreeSamplingBasedPlanner.hpp"
#include "Setup/Defaults.hpp"
#include "Utils/Grid.hpp"
#include "PluginRosetta/CfgProjectorDeltaR.hpp"
#include <Utils/Selector.hpp>

namespace Antipatrea
{


    class SprintVertex : public PlannerVertex
    {
    public:
        SprintVertex(void) : PlannerVertex()
        {
        }

        virtual ~SprintVertex(void)
        {
        }
    };

    class DeltaRRegion
    {
    public:
    	DeltaRRegion()
        {
    		m_weight = 0.0;
    		m_power = 2;
        }

    	virtual ~DeltaRRegion()
        {
        }

    	void AddVertex(int vid)
    	{
    		auto newNode = m_selector.Create();
			newNode->SetKey(vid);
			newNode->SetWeight(1.0);

			m_selector.Insert(newNode);
    	}

    	int SelectVertex()
    	{
    		auto cell = m_selector.Select();
    		return (cell->GetKey());
    	}

    	double GetWeight()
    	{
    		if (m_selector.GetNrNodes() == 1) // *first node added
    		{
    			m_weight = pow(m_weightId,m_power);
    		}

    		// std::cout << "for region:" << m_weightId << " return weight:" << m_weight << std::endl;
    		return m_weight;;
    	}

    	unsigned int GetNrConfs()
    	{
    		return (m_selector.GetNrNodes());
    	}

    	void SetPower(unsigned int p)
    	{
    		m_power = p;
    	}

    	Selector<unsigned int>::Node *nodePtr;
    	int m_weightId;
    	int m_power;
    protected:
        Selector<int> m_selector;
        double m_weight;

    };


    /**
     *@author Kevin Molloy, Erion Plaku, Amarda Shehu
     *@brief  Implement methods to calculate motions
     *        between stable protein states.  Based on work
     *        by Molloy et al (see BMC Structural Biology (2012)
     *        Molloy, Shehu, Elucidating the ensemble
     *        of functionally-relevant transitions in protein
     *        systems with a robotics-inspired method.
     *
     *@remarks
     * -  This implementation does not support all the techniques
     *    used in the publication above.
     *
     */
    class Sprint : public TreeSamplingBasedPlanner,
                   public CfgProjectorContainer
    {
    public:
        Sprint(void) : TreeSamplingBasedPlanner()
        {
        }

        virtual ~Sprint(void)
        {
        }


        virtual void Info(const char prefix[]) const
        {
			TreeSamplingBasedPlanner::Info(prefix);
			Logger::m_out << prefix << " Sprint parms TBD" << std::endl;
        }


        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief Set the parameter values of the planner from the given parameters.
         *
         *@remarks
         * - Function first invokes TreeSamplingBasedPlanner::SetupFromParams(params).
         * - It uses the parameter group associated with the keyword Constants::KW_SPRINT.
         */
        virtual void SetupFromParams(Params & params)
        {
            TreeSamplingBasedPlanner::SetupFromParams(params);

            auto data = params.GetData(Constants::KW_SPRINT);
            if(data && data->m_params)
            {;
                m_regionExp = data->m_params->GetValueAsInt(Constants::KW_SPRINT_REGION_EXP,
                		                                    Constants::VAL_SPRINT_REGION_EXP);
            }


        }

        void SetupDeltaRRegions();
    protected:
        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief  Create a new planner vertices.
         *
         *@remarks
         * - Create SprintVertex instead of PlannerVertex.
         */

        virtual PlannerVertex* NewVertex(void) const
        {
			return new SprintVertex();
        }

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  Given a configuration, add it to the Sprint projections and
          *        the TreeBasedPlanner data structures.  Returns the vid, or
          *        -1 is no vertex was added.  Recall that because of the
          *        granularity reduction, vertices may not added (and
          *        a -1 is returned).
          */
        virtual int AddVertex(Cfg * const cfg);

        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief Select a vertex from which to expand the tree and
         *       a target toward which to expand the tree.
         *
         *@remarks
         * - Function returns the vid of the selected vertex.
         * - First, an RMSD energy region is selected using the desired
         *   weighting function, and then a cell within that energy
         *   region is selected.
         */
        virtual int SelectVertex(void);

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  The granularity (number of cells in each dimension)
          *        within a FELTRCell (projections within an energy level).
          */
        int m_cellGridGranularity;

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  The granularity (number of cells in each dimension) for
          *        mapping the Delta R values.
          */
        int m_deltaRGridGranularity;


        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  Low end of the DeltaR projection.  All cfgs with lower DeltaR
          *        are simply projected to this region.
          */
        double m_deltaRGridMin;

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  High end of the DeltaR projection.  All cfgs with higher DeltaR
          *        are projected into this region.
          */

        double m_deltaRGridMax;


        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  Allow for efficient probabilistic selection for an
          *        energy region (stores pointers to energy regions and
          *        their associated weight).
          */

        std::vector<DeltaRRegion> m_regions;
        Selector<unsigned int> m_selector;
        int m_regionExp;
       };
}
#endif /* SRC_PLANNERS_SPRINT_HPP_ */
