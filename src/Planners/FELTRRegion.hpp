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

// typedef double Id;




namespace Antipatrea
{
    const double CA_CA_IDEAL_D=3.8; // used in USR-cell dimension calcs

    double get_constrained_R0(int nres);

    class FELTRVertex;

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
     * -  Each energy region is discretized into a set of 3d cells
     *    based on the cfg's USR coordinates.
     */

    class FELTRCell: public CfgDistanceContainer
    {
    public:
        FELTRCell(void) : m_nsel(0)
                         , m_weight(0.0)
                         , m_nconfs(0)
                         , m_newPaths(0)
                         , m_threshold(0.25)
                         , m_reductionCount(1)
        {
        }

        virtual ~FELTRCell(void)
        {
        }

        /**
           *@author Kevin Molloy, Erion Plaku, Amarda Shehu
           *@brief  Select a vertex from the cell.  The vid
           *        is returned.
           */
        int SelectVertex(void);

        /**
           *@author Kevin Molloy, Erion Plaku, Amarda Shehu
           *@brief  Store the scalar ID of the cell.
           */

        void SetId(unsigned int pId) {m_id = pId;}

        unsigned int GetId() {return m_id;}


        void IncNew(int inc=1) {m_newPaths+= inc;}

        /**
           *@author Kevin Molloy, Erion Plaku, Amarda Shehu
           *@brief  return the number of times this cell has been selected.
           */

        int GetNrSelections(void) const
        {
            return m_nsel;
        }

        /**
           *@author Kevin Molloy, Erion Plaku, Amarda Shehu
           *@brief  return the number of cfgs within this cell.
           */

        int GetNrConfs(void) const
        {
           return m_nconfs;
        }
        /**
           *@author Kevin Molloy, Erion Plaku, Amarda Shehu
           *@brief  Get the weight of this cell..
           */

        double GetWeight(void) const
        {
            return m_weight;
        }

        /**
           *@author Kevin Molloy, Erion Plaku, Amarda Shehu
           *@brief  Add a vertex to the cell and adjust the weight.
           */

        void AddVertex(int vid,const Cfg *cfg);
        /**
           *@author Kevin Molloy, Erion Plaku, Amarda Shehu
           *@brief  Return true iff a similar cfg already exists
           *        within the cell.  Similarity is defined by the
           *        distance function and the user specified threshold.
           */

        bool SimilarCfgExists(Cfg * const cfg);
        //----------------------------------------------------------------------------


    protected:

        /**
           *@author Kevin Molloy, Erion Plaku, Amarda Shehu
           *@brief  Update the weight of the cell based on it
           *        being selected.
           */

        void UpdateWeightOnSelection(const int nsel)
        {
            m_nsel += nsel;
            UpdateWeight();
        }

        void UpdateWeightOnInsert()
        {
            ++m_nconfs;
            UpdateWeight();
        }

        void UpdateWeight(void)
        {
            m_weight = 1.0 / ((double) (1.0 + m_nsel) * m_nconfs);
        }

        int weightType;
        int           m_nsel;   //number of times cell has been selected
        double        m_weight;
        int           m_nconfs;
        double        m_threshold;
        Selector<int> m_insideCellSelector;
        unsigned int  m_reductionCount;
        std::vector<const Cfg *> m_cfgs;

        int m_newPaths;
        int m_id;  // key that was used to map to this cell
                   // used to verify pointers are not getting crossed
    };  /* FELTRCell */


    /////////////
    ////////////
    ////////////
    class FELTRRegion:public CfgDistanceContainer
    {
    public:
        enum WeightFunc {RAND=1, QUAD=2, METR=3, COV1=4, COV2=5, MTCV=6,NORM=7};
        typedef enum WeightFunc WeightFunc;

        FELTRRegion(void) : m_nsel(0),
                           m_sumE(0.0),
                           m_minE(std::numeric_limits<double>::min()),
                           m_maxE(std::numeric_limits<double>::max()),
                           m_nrConfs(0),
                           m_weight(0.0),
                           m_newPaths(0),
                           m_gridCells(NULL)
        {
        }

        FELTRRegion(int id,int cellCount,unsigned int objDim ): m_nsel(0),
                m_sumE(0.0),
                m_minE(std::numeric_limits<double>::min()),
                m_maxE(std::numeric_limits<double>::max()),
                m_nrConfs(0),
                m_weight(0.0),
                m_id(id),
                m_newPaths(0),
                m_objDim(objDim),
                m_reductionCount(0),
                m_weightFunction(QUAD)
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
        void UpdateWeightQUAD(void)
        {
            //m_weight = m_sumE / ((double) (1.0 + m_nsel) * m_selector.GetNrNodes());
            //m_weight = 1.0 / ((double) (1.0 + m_nsel) * m_selector.GetNrNodes());
            //            m_weight = TreeVertex::ComputeWeight(m_nsel, m_sumE/m_nrConfs);
            double averageEnergy = GetAvgE();

            if (averageEnergy > 0)
                m_weight =  1.0/(1 + averageEnergy * averageEnergy);
            else
                m_weight = fabs(pow(averageEnergy,2));

            /* Logger::m_out << "Update energy weight for average energy:" << averageEnergy
                          << " is:" << m_weight << "\n"; */

        }

        void UpdateWeightNORM(void)
        {
            //m_weight = m_sumE / ((double) (1.0 + m_nsel) * m_selector.GetNrNodes());
            //m_weight = 1.0 / ((double) (1.0 + m_nsel) * m_selector.GetNrNodes());
            //            m_weight = TreeVertex::ComputeWeight(m_nsel, m_sumE/m_nrConfs);
            double averageEnergy = GetAvgE();

            if (averageEnergy > 0)
                m_weight =  1.0/(1 + averageEnergy * averageEnergy);
            else
                m_weight = fabs(pow(averageEnergy,2));

            /* Logger::m_out << "Update energy weight for average energy:" << averageEnergy
                          << " is:" << m_weight << "\n"; */

        }
        void UpdateWeight()
        {
            switch (m_weightFunction)
            {
                case NORM:
                    UpdateWeightNORM();
                    break;
                case QUAD:
                default:
                    UpdateWeightQUAD();
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


