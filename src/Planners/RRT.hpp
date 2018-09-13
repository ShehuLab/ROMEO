#ifndef Antipatrea__RRT_HPP_
#define Antipatrea__RRT_HPP_

#include "Planners/TreeSamplingBasedPlanner.hpp"

namespace  Antipatrea
{
      /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Rapidly-exploring Random Tree (RRT).
     * 
     *@remarks
     * - The target is selected as in the base class, using the function
     *   SelectTarget.
     * - The closest vertex in the tree to the sampled target (according to CfgDistance)
     *   is then selected for expansion.
     * - After the vertex and target have been selected, as in the base class,
     *   PGT uses ExpandTowardTarget to expand the tree from the selected
     *   vertex toward the selected target.
     */
    class RRT : public TreeSamplingBasedPlanner
    {
    public:
        RRT(void) : TreeSamplingBasedPlanner()
        {
        }

        virtual ~RRT(void)
        {
        }
        
    protected:
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Select a vertex from which to expand the tree and 
         *       a target toward which to expand the tree.
         *
         *@remarks
         * - Function returns the id of the selected vertex.
         * - The target is selected as in the base class, using the function
         *   SelectTarget.
         * - The closest vertex in the tree to the sampled target (according to CfgDistance)
         *   is then selected for expansion.
           */        
        virtual int SelectVertex(void);
    };
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to RRT.
     */
    ClassContainer(RRT, m_rrt);
    
}

#endif
