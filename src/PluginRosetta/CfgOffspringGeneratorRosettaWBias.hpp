#ifndef Antipatrea__CfgOffspringGeneratorRosettaWBias_HPP_
#define Antipatrea__CfgOffspringGeneratorRosettaWBias_HPP_

#include "PluginRosetta/CfgOffspringGeneratorRosetta.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

namespace Antipatrea
{

    /**
     *@author Kevin Molloy, Erion Plaku, Amarda Shehu
     *@brief Use Rosetta/library to generate an offspring near the parent configuration.
     *
     *
     */
    class CfgOffspringGeneratorRosettaWBias : public CfgOffspringGeneratorRosetta
    {
    protected:
        int m_offspringToGenerate;

    public:
        CfgOffspringGeneratorRosettaWBias(void) : CfgOffspringGeneratorRosetta()        {
            m_offspringToGenerate=1;
        }

        virtual ~CfgOffspringGeneratorRosettaWBias(void)
        {
        }


         virtual void SetupFromParams(Params & params);

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Use Rosetta to generate the offspring configuration near the parent configuration.
         *
         *@remarks
         * - Use GetStep() as a measure of how far/different the offspring should be from its parent.
         */
        virtual void GenerateOffspringCfg(Cfg & cfg);

    protected:
        virtual double GenerateAnOffspringCfg(Cfg & cfg);
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgOffspringGeneratorRosetta.
     */
    ClassContainer(CfgOffspringGeneratorRosettaWBias, m_cfgOffspringGeneratorRosettaWBias);
}

#endif
