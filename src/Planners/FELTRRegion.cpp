/*
 * FELTRRegion.cpp
 *
 *  Created on: Oct 6, 2017
 *      Author: kmolloy
 */

#include "Planners/FELTRRegion.hpp"
namespace Antipatrea
{
	void FELTRCell::AddVertex(int vid)
	{
		// add to the cell
		auto newNode = m_selector.Create();
		newNode->SetKey(vid);
		newNode->SetWeight(1.0);
		m_selector.Insert(newNode);
	}

	int FELTRCell::SelectVertex(void)
	{
		auto node = m_selector.Select();
		int  v    = node->GetKey();

		//v->UpdateWeightOnSelection(1);
		//m_selector.Update(node, v->GetWeight());

		// UpdateWeightOnSelection(1);

		return v;
	}


	Id FELTRRegion::AddVertex(int vid,
					          const double * const p,
							  FELTRRegion * &region,
							  FELTRCell   * &cell)
	{
		region = this;

		auto cellId = m_gridCells->GetCellIdFromPoint(p);
		// see if CELL needs to be created
		auto iter =  m_CellToSelectorMap.find(cellId);
		bool newCell = (iter == m_CellToSelectorMap.end());

		cell = NULL;
		Selector<FELTRCell *>::Node *cellNode;
		if (newCell)
		{
			cell = new FELTRCell();
			cellNode = m_selector.Create();
			cellNode->SetKey(cell);
			cellNode->SetWeight(cell->GetWeight());

			m_selector.Insert(cellNode);

			m_CellToSelectorMap.insert(std::make_pair(cellId,cellNode));
		}
		else
		{
			cellNode = iter->second;
			cell = cellNode->GetKey();
		}

		UpdateWeightOnEnergy(p[3]);
		cell->AddVertex(vid);


	}

    int FELTRRegion::SelectVertex(void)
	{
		Selector<FELTRCell *>::Node *cellNode = m_selector.Select();
		FELTRCell                   *cell     = cellNode->GetKey();
		Id                          v        = cell->SelectVertex();

		m_selector.Update(cellNode, cell->GetWeight());

		UpdateWeightOnSelection(1);

		return v;
	}
}
