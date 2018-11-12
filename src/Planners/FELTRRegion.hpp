/*
 * FELTRRegion.hpp
 *
 *  Created on: Oct 5, 2017
 *      Author: Amarda Shehu, Erion Plaku, Kevin Molloy
 */

#ifndef Antipatrea__FELTRREGION_HPP_
#define Antipatrea__FELTRREGION_HPP_



#define FELTR_PROJECTION_COORDS 4
#define FELTR_USR_COORDS 3
#define FELTR_ENERGY_COORD_OFFSET 3


#include <limits>
#include <list>
#include <map>

#include <stddef.h>
#include "Planners/TreeSamplingBasedPlanner.hpp"

#include <Utils/Grid.hpp>
#include <Utils/Selector.hpp>
#include "Planners/FELTR.hpp"
#include "Planners/FELTRRegion.hpp"
#include "Planners/FELTRCell.hpp"

// typedef double Id;




namespace Antipatrea
{
    class FELTRRegion:public CfgDistanceContainer
    {
    public:
        enum WeightFunc {LINEAR=1, QUAD=2, NORM=3};
        typedef enum WeightFunc WeightFunc;

        FELTRRegion(void) : m_nsel(0),
                            m_sumE(0.0),
                            m_minE(std::numeric_limits<double>::max()),
                            m_maxE(std::numeric_limits<double>::min()),
                            m_nrConfs(0),
                            m_weight(0.0),
                            m_newPaths(0),
                            m_gridCells(NULL),
							m_weightFunction(QUAD)
        {
        }

        FELTRRegion(int id,int cellCount,unsigned int objDim,unsigned int weightFunction = QUAD ): m_nsel(0),
                m_sumE(0.0),
                m_minE(std::numeric_limits<double>::max()),
                m_maxE(std::numeric_limits<double>::min()),
                m_nrConfs(0),
                m_weight(0.0),
                m_id(id),
                m_newPaths(0),
                m_objDim(objDim),
                m_reductionCount(0),
                m_weightFunction(weightFunction)
        {
             // double minRg = get_constrained_R0(m_objDim - (FRAG_LENGTH - 1)) - 1.0;
            double minRg = get_constrained_R0(m_objDim - (3 - 1)) - 1.0;

            double extendedLength = (m_objDim - 1) * CA_CA_IDEAL_D;
            double medRg = extendedLength/4.0;

            int dims[4];
            double g_min[4];
            double g_max[4];

            for(int i = 0; i < FELTR_PROJECTION_COORDS - 1; ++i)
            {
                g_min[i] = minRg;
                g_max[i] = medRg;
                dims[i] = cellCount;
                /* std::cout << "FELTRCell limits:gmin:" << g_min[i] << ":gmax:" << g_max[i]
                          << ":granularity:" << dims[i] << "\n"; */
            }

            dims[3]  = 1; // energy is not used here.
            g_min[3] = 1;
            g_max[3] = 1;

            m_gridCells = new Grid();

            m_gridCells->Setup(FELTR_PROJECTION_COORDS,dims,g_min,g_max);
        }


        virtual ~FELTRRegion(void)
        {
        }


        void AddVertex(int vid,
                       const Cfg *cfg,
                       const double * const p,
                       FELTRRegion * &region,
                       FELTRCell   * &cell);

        void IncNew(int inc=1) {m_newPaths+= inc;}

        int GetId() {return m_id;}

        int GetNrGridCells()
        {
            return m_CellToSelectorMap.size();
        }

        int GetNrSelections(void) const
        {
            return m_nsel;
        }

        double GetAvgE(void) const
        {
            return m_sumE / m_nrConfs;
        }

        double GetSumE(void) const
        {
            return m_sumE;
        }

        double GetMinE(void) const
        {
            return m_minE;
        }

        double GetMaxE(void) const
        {
            return m_maxE;
        }

        int GetNrConfs(void) const
        {
            return m_nrConfs;
        }

        double GetWeight(void) const
        {
            return m_weight;
        }
        void PrintCellIdList(std::ostream & stream);

        int SelectVertex(void);
        bool CheckVertex(Cfg * const cfg,
                         double []);

    protected:
        void UpdateWeightOnSelection(const int nsel)
        {
            m_nsel += nsel;
            // UpdateWeight(); since weight is not based on nmr of selections
        }

        void UpdateWeightOnEnergy(const double energy)
        {
            ++m_nrConfs;
            m_sumE += energy;

            if(energy < m_minE)
                m_minE = energy;
            if(energy > m_maxE)
                m_maxE = energy;

            UpdateWeight();
        }

        //--------------------------------------------------------------------------
        void UpdateWeightP(unsigned int power)
        {
            //m_weight = m_sumE / ((double) (1.0 + m_nsel) * m_selector.GetNrNodes());
            //m_weight = 1.0 / ((double) (1.0 + m_nsel) * m_selector.GetNrNodes());
            //            m_weight = TreeVertex::ComputeWeight(m_nsel, m_sumE/m_nrConfs);
            double averageEnergy = GetAvgE();

            if (averageEnergy > 0)
                m_weight =  1.0/(1 + pow(averageEnergy,power));
            else
                m_weight = fabs(pow(averageEnergy,power));

            /* Logger::m_out << "Update energy weight for average energy:" << averageEnergy
                          << " is:" << m_weight << "\n"; */

        }

        void UpdateWeight()
        {
            switch (m_weightFunction)
            {
                case LINEAR:
                	UpdateWeightP(1);
                	break;
                case QUAD:
                default:
                    UpdateWeightP(2);
                    break;

            }
        }


        int              m_nsel;   //number of times region has been selected

        int              m_nrConfs;
        double           m_sumE;   //sum over energies of states in region
        double           m_minE;   //minimum energy over energies in region
        double           m_maxE;   //maximum energy over energies in region

        double           m_weight;

        Selector<FELTRCell *> m_selector;
        std::map<int, Selector<FELTRCell *>::Node *> m_CellToSelectorMap;

        Grid *m_gridCells;
        int m_id;
        int m_newPaths;
        unsigned int m_objDim;
        unsigned int  m_reductionCount;
        unsigned int m_weightFunction;
    }; // FELTRRegion
}

#endif


