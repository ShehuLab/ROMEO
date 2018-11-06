/*
 * FELTRRegion.cpp
 *
 *  Created on: Oct 6, 2017
 *      Author: kmolloy
 */

#include "Planners/FELTRRegion.hpp"
namespace Antipatrea
{
    bool FELTRRegion::CheckVertex(Cfg * const cfg,
                                  double p[])
    {
        bool vertexOK = true;
        auto cellId = m_gridCells->GetCellIdFromPoint(p);

        // see if CELL needs to be created
        auto iter =  m_CellToSelectorMap.find(cellId);
        bool newCell = (iter == m_CellToSelectorMap.end());
        Selector<FELTRCell *>::Node *cellNode;
        if (!newCell)
        {
            cellNode = iter->second;
            FELTRCell *cell = cellNode->GetKey();
            if (cell->SimilarCfgExists(cfg) == true)
                vertexOK = false;
        }

        if (!vertexOK) ++m_reductionCount;

        return(vertexOK);
    }

    void FELTRRegion::AddVertex(int vid,
                                const Cfg *cfg,
                                const double * const p,
                                FELTRRegion * &region,
                                FELTRCell   * &cell)
    {
        region = this;

        /**
        //KMDEBUG
			std::cout << "in Addvertex with projection:";
			for (unsigned int i(0);i < 4;++i)
			{
				std::cout << p[i] << " ";
			}
			std::cout << "\n";
			//KMDEBUG
		*/

        auto cellId = m_gridCells->GetCellIdFromPoint(p);
        // see if CELL needs to be created
        auto iter =  m_CellToSelectorMap.find(cellId);
        bool newCell = (iter == m_CellToSelectorMap.end());

        cell = NULL;
        Selector<FELTRCell *>::Node *cellNode;
        if (newCell)
        {
            cell = new FELTRCell();
            cell->SetCfgDistance(GetCfgDistance());
            cell->SetId(cellId);
            cellNode = m_selector.Create();
            cellNode->SetKey(cell);
            cellNode->SetWeight(cell->GetWeight());

            // std::cout << "Creating new cell with id:" << cellId << "\n"; //KMDEBUG


            m_selector.Insert(cellNode);

            m_CellToSelectorMap.insert(std::make_pair(cellId,cellNode));
        }
        else
        {
            std::cout << "Found cell with id:" << cellId << "\n";
            cellNode = iter->second;
            cell = cellNode->GetKey();
        }

        UpdateWeightOnEnergy(p[3]);
        cell->AddVertex(vid,cfg);

        m_selector.Update(cellNode,cell->GetWeight());
    }


    int FELTRRegion::SelectVertex(void)
    {
        Selector<FELTRCell *>::Node *cellNode = m_selector.Select();
        FELTRCell                   *cell     = cellNode->GetKey();

        //DEBUG
        double oldWeight = cell->GetWeight();
        //END DEBUG
        int v        = cell->SelectVertex();

       	//std::cout << "SELECTvid:" << v << ":region:" << m_id << ":cellid:" << cell->GetId() << ":CellsInThisRegion:"
       	//		  << m_selector.GetNrNodes() << ":cfgsInThisCell:"
        //          << cell->GetNrConfs() << ":ocw:" << oldWeight << ":ncw:" << cell->GetWeight() << "\n";

        m_selector.Update(cellNode, cell->GetWeight());

        UpdateWeightOnSelection(1);

        return v;
    }
}
