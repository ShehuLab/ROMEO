#ifndef Antipatrea__EdgeCostEvaluator_HPP_
#define Antipatrea__EdgeCostEvaluator_HPP_

#include "Components/Component.hpp"
#include "Components/CfgManagers/Cfg.hpp"
#include <vector>

namespace Antipatrea
{	
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface for evaluating the direct and reverse cost of a path.
     * 
     *@remarks
     * - The reverse cost of a path from A to B corresponds to the cost
     *   of the reverse path, namely from B to A.
     * - Often the direct and reverse costs are the same but not always.
     * - This interface allows the user to implement nonsymmetric
     *   path-cost evaluators.
     */ 
    class EdgeCostEvaluator : public Component
    {
    public:
	EdgeCostEvaluator(void) : Component()
	{
	}
	
	virtual ~EdgeCostEvaluator(void)
	{
	}
	
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Represent the direct and reverse costs of a path.
	 */	 
	struct Costs
	{
	    Costs(void) : m_costFromTo(0.0),
			  m_costToFrom(0.0)
	    {
	    }
	    
	    virtual ~Costs(void)
	    {
	    }
	    
	  
	    /**
	     *@author Erion Plaku, Amarda Shehu
	     *@brief Cost of the direct path.
	     */
	    double m_costFromTo;
	    
	  
	    /**
	     *@author Erion Plaku, Amarda Shehu
	     *@brief Cost of the reverse path.
	     */
	    double m_costToFrom;
	};
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Evaluate the direct and reverse cost of a path when given only its start and end configurations.
	 */ 
	virtual Costs EvaluateGivenFromToCfgs(Cfg & cfgFrom, Cfg & cfgTo) = 0;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Evaluate the direct and reverse cost of a path when given the start, end, and the sequence of intermediate configurations.
	 *
	 *@remarks
	 * - This function accumulates the costs along the sequence of configurations.
	 */ 
	virtual Costs EvaluateGivenFromToAndIntermediateCfgs(Cfg & cfgFrom, Cfg & cfgTo, std::vector<Cfg*> & intermediates);
	
    };
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to EdgeCostEvaluator.
     */	 
    ClassContainer(EdgeCostEvaluator, m_edgeCostEvaluator);
    
}

#endif
