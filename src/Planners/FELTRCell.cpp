/*
 * FELTRRegion.cpp
 *
 *  Created on: Oct 6, 2017
 *      Author: kmolloy
 */

#include "Planners/FELTRRegion.hpp"
#include "Planners/FELTRCell.hpp"

namespace Antipatrea
{
    double get_constrained_R0(int nres)
    {
        return (2.5 * pow((nres),0.34));
    }

    void FELTRCell::AddVertex(int vid,
                              const Cfg *cfg)
    {
        // add to the cell
        UpdateWeightOnInsert();

        auto newNode = m_insideCellSelector.Create();
        newNode->SetKey(vid);
        newNode->SetWeight(1.0);

        m_insideCellSelector.Insert(newNode);
        m_cfgs.push_back(cfg);
    }

    int FELTRCell::SelectVertex(void)
    {
        auto node = m_insideCellSelector.Select();

        assert(node->GetWeight() == 1.0); // all nodes should have weight 1 in cell

        int  v    = node->GetKey();

        UpdateWeightOnSelection(1);
        // m_selector.Update(node, GetWeight());

        return v;
    }

    bool FELTRCell::SimilarCfgExists(Cfg * const cfg)
    {
        auto distF = GetCfgDistance();
        bool foundSimilar = false;
        double dist = 999.99;
        for (auto c : m_cfgs)
        {
            dist = distF->Distance(*cfg,*c);
            if (dist < m_threshold)
            {
                foundSimilar = true;
                break;
            }
        }

        if (foundSimilar)
        {
            std::cout << "SIMILARCell_rmsd:" << dist << "\n";
            ++m_reductionCount;
        }

        return(foundSimilar);

        // check each configuration in this cell

    }
}  // namespace
