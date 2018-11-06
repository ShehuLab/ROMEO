/*
 * FELTRCell.hpp
 *
 *  Created on: Oct 5, 2017
 *      Author: Amarda Shehu, Erion Plaku, Kevin Molloy
 */

#ifndef Antipatrea__FELTRCELL_HPP_
#define Antipatrea__FELTRCELL_HPP_



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


}

#endif


