/*
 * FELTRRegion.hpp
 *
 *  Created on: Oct 5, 2017
 *      Author: Amarda Shehu, Erion Plaku, Kevin Molloy
 */

#ifndef Antipatrea__FELTRREGION_HPP_
#define Antipatrea__FELTRREGION_HPP_


#include <limits>
#include <list>
#include <map>

#include <stddef.h>
#include "Planners/TreeSamplingBasedPlanner.hpp"
#include <Utils/Grid.hpp>
#include <Utils/Selector.hpp>
#include <Planners/FELTR.hpp>

typedef double Id;

namespace Antipatrea
{
	class FELTRVertex;

	class FELTRCell
	{
	public:
		FELTRCell(void) : m_nsel(0)
				 , m_sumE(0.0)
				 , m_weight(0.0)
				 , m_nconfs(0)
				 , m_newPaths(0)
		{
		}

		virtual ~FELTRCell(void)
		{
		}

		int SelectVertex(void);

		void IncNew(int inc=1) {m_newPaths+= inc;}

		int GetNrSelections(void) const
		{
			return m_nsel;
		}

		double GetSumE(void) const
		{
			return m_sumE;
		}

		int GetNrConfs(void) const
			{
		   return m_nconfs;
			}

		double GetWeight(void) const
		{
			return m_weight;
		}

		void AddVertex(int vid);

		FELTRVertex* AddCount(FELTRVertex * v);
		FELTRVertex* AddDist(FELTRVertex * v);
		FELTRVertex* SetTreeVertex(FELTRVertex * const v, Selector<FELTRVertex *>::Node * const node);
		Selector<FELTRVertex *>::Node* GetVertexNode(FELTRVertex* vertex);
		//----------------------------------------------------------------------------


		protected:
		void UpdateWeightOnSelection(const int nsel)
		{
			m_nsel += nsel;
			UpdateWeight();
		}

		void UpdateWeightOnEnergy(const double energy)
		{
			++m_nconfs;
			m_sumE += energy;
			UpdateWeight();
		}

			/*
			void UpdateWeight(void)
			{
				m_weight = TreeVertex::ComputeWeight(m_nsel, m_sumE/m_nconfs);
			}
			*/

		void UpdateWeight(void)
		{
			m_weight = 1.0 / ((double) (1.0 + m_nsel) * m_nconfs);
		}

		int weightType;
		int           m_nsel;   //number of times cell has been selected
		double        m_sumE;   //sum over energies of states in cell
		double        m_weight;
		int           m_nconfs;
		Selector<int> m_selector;
		int m_newPaths;
	};  /* FELTRCell */


	class FELTRRegion
	{
	public:
		enum WeightFunc {RAND=1, QUAD=2, METR=3, COV1=4, COV2=5, MTCV=6};
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

	FELTRRegion(Id id,int cellCount ): m_nsel(0),
		m_sumE(0.0),
		m_minE(std::numeric_limits<double>::min()),
		m_maxE(std::numeric_limits<double>::max()),
		m_nrConfs(0),
		m_weight(0.0),
		m_id(id),
		m_newPaths(0)
		{
			int dims[4];
			dims[3] = 1;
			double g_min[4];
			double g_max[4];
			for (auto i=0;i < 2;++i)
			{
				g_min[i] = 0.0;
				g_max[i] = 100.0;
				dims[i] = cellCount;
			}
			g_min[3] = -200;
			g_max[3] = 0;
			dims[3] = 1;

			m_gridCells = new Grid();

		    m_gridCells->Setup(4,dims,g_min,g_max);

		}


		virtual ~FELTRRegion(void)
		{
		}


		Id AddVertex(int vid,
				     const double * const p,
				     FELTRRegion * &region,
				     FELTRCell   * &cell);

		void IncNew(int inc=1) {m_newPaths+= inc;}

		Id GetCellIdFromPoint(const double * const p) const
		{
			return m_gridCells->GetCellIdFromPoint(p);
		}


		Id GetId() {return m_id;}

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

		FELTRVertex* AddTreeVertex(FELTRVertex * v, const Id idCell, bool & newCell);

		int SelectVertex(void);

		protected:
		void UpdateWeightOnSelection(const int nsel)
		{
			m_nsel += nsel;
			UpdateWeight();
		}

		void UpdateWeightOnEnergy(const double energy)
		{
			++m_nrConfs;
			m_sumE += energy;

			if(energy < m_minE) m_minE = energy;
				if(energy > m_maxE) m_maxE = energy;

			UpdateWeight();

	//	    printf("min e max avg = %f %f %f %f\n", m_minE, energy, m_maxE, m_sumE/m_nrConfs);
		}
		//--------------------------------------------------------------------------
		//--------------------------------------------------------------------------
		void UpdateWeight(void)
		{
			//m_weight = m_sumE / ((double) (1.0 + m_nsel) * m_selector.GetNrNodes());
			//m_weight = 1.0 / ((double) (1.0 + m_nsel) * m_selector.GetNrNodes());
				//	    m_weight = TreeVertex::ComputeWeight(m_nsel, m_sumE/m_nrConfs);
			m_weight = pow(GetAvgE(),2);
		}

		int              m_nsel;   //number of times region has been selected

		int              m_nrConfs;
		double           m_sumE;   //sum over energies of states in region
		double           m_minE;   //minimum energy over energies in region
		double           m_maxE;   //maximum energy over energies in region

		double           m_weight;

		Selector<FELTRCell *> m_selector;
		std::map<Id, Selector<FELTRCell *>::Node *> m_CellToSelectorMap;

		Grid *m_gridCells;
		Id m_id;
		int m_newPaths;
	};
}

#endif


