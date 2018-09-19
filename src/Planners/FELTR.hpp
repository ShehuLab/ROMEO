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
     *   a FELTR object keeps projection information.
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


    /**
     *@author Kevin Molloy, Erion Plaku, Amarda Shehu
     *@brief  Implement the FELTR algorithm.
     *        See IJRR (2010) Shehu, Olson Guiding the Search for
     *        Native-like Protein Conformations with an Ab-Initio
     *        Tree-based exploration and Olson, Molloy, Shehu In Search
     *        of the Protein Native State with a Probabilistic Sampling
     *        Approach (JBCC 2011).
     *
     *@remarks
     * -  This implementation uses Rosetta for the forward kinematics, energy functions,
     *    and fragment libraries.
     *
     */
    class FELTR : public TreeSamplingBasedPlanner,
                  public CfgProjectorContainer
    {
    public:
        FELTR(void) : TreeSamplingBasedPlanner(),
                     m_energyGridGranularity(Constants::VAL_EnergyGrid_Granularity),
                     m_energyGridMin(Constants::VAL_EnergyGrid_Min),
                     m_energyGridMax(Constants::VAL_EnergyGrid_Max),
                     m_cellGridGranularity(Constants::VAL_CellGrid_Granularity)
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
                m_cellGridGranularity = (data->m_params->GetValueAsInt(Constants::KW_FELTR_CELLGRID_GRAN,
                                                                       m_cellGridGranularity));
                m_energyGridGranularity = (data->m_params->GetValueAsInt(Constants::KW_FELTR_ENERGYGRID_GRAN,
                                                                         m_energyGridGranularity));
                m_energyGridMin = (data->m_params->GetValueAsDouble(Constants::KW_FELTR_ENERGYGRID_MIN,
                                                                    m_energyGridMin));
                m_energyGridMax = (data->m_params->GetValueAsDouble(Constants::KW_FELTR_ENERGYGRID_MAX,
                                                                    m_energyGridMax));
            }

            // initialize 1st level projection coordinates, which ignore
            // the first 3 values (USR coordinates) and only projects
            // based on energy scores.
            int granRegions[FELTR_PROJECTION_COORDS];
            double gMin[FELTR_PROJECTION_COORDS];
            double gMax[FELTR_PROJECTION_COORDS];
            for (unsigned int i(0);i < FELTR_USR_COORDS;++i) {
                    granRegions[i] = 1;
                    gMin[i] = 0;
                    gMax[i] = 1;
            }

            // Energy region projection
            granRegions[FELTR_ENERGY_COORD_OFFSET] = m_energyGridGranularity;

            gMin[FELTR_ENERGY_COORD_OFFSET] = m_energyGridMin;
            gMax[FELTR_ENERGY_COORD_OFFSET] = m_energyGridMax;

            Logger::m_out << "Making energy grid with min:" << m_energyGridMin
                          << " and max:" << m_energyGridMax << "\n";

            m_energyGrid.Setup(FELTR_PROJECTION_COORDS,granRegions,gMin,gMax);
        }


    protected:
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief  Create a new planner vertices.
         *
         *@remarks
         * - Create FELTRVertex instead of PlannerVertex.
         */
        virtual PlannerVertex* NewVertex(void) const
        {
                return new FELTRVertex();
        }

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  Given a 4d FELTR projection, return a FELTRRegion from the
          *        SELECTOR structure (not the graph).
          */
        virtual Selector<FELTRRegion *>::Node * GetEnergyRegion(double projection[]);

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  Given a configuration, add it to the FELTR projections and
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
         * - First, an energy region is selected using the desired
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
          *@brief  The granularity (number of cells in each dimension)
          *        for the FELTRRegions.
          */
        int m_energyGridGranularity;


        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  Low end of the energy projection.  All cfgs with lower energies
          *        are simply projected to this region.
          */
        double m_energyGridMin;

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  High end of the energy projection.  All cfgs with higher energies
          *        are projected into this region.
          */

        double m_energyGridMax;

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  Object that accepts the 4 projection coordinates and
          *        returns an integer scalar that can be used to map to a
          *        specific energy region.
          */

        Grid m_energyGrid;

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  Allow for efficient probabilistic selection for an
          *        energy region (stores pointers to energy regions and
          *        their associated weight).
          */

        Selector<FELTRRegion *> m_selector;

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  Maps the scalar returns by m_energyGrid to its
          *        cooresponding node within the selector tree.
          */

        std::map<int,Selector<FELTRRegion*>::Node *> m_FELTRRegionToSelectorMap;
       };
}
#endif /* SRC_PLANNERS_FELTR_HPP_ */
