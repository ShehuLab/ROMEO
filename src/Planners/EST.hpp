#ifndef Antipatrea__EST_HPP_
#define Antipatrea__EST_HPP_

#include "Planners/TreeSamplingBasedPlanner.hpp"
#include "Setup/Defaults.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Vertex representation for EST.
     *
     *@remarks
     * - In addition to the information stored by PlannerVertex,
     *   an ESTVertex also keeps track of the number of neighbors
     *   inside a user-specified neighborhood and the
     *   number of times the vertex has been selected for expansion.
     */  
    class ESTVertex : public PlannerVertex
    {
    public:
        ESTVertex(void) : PlannerVertex(),
                          m_nrNeighbors(0),
                          m_nrSel(0)
        {
        }

        virtual ~ESTVertex(void)
        {
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get the number of neighbors in the user-defined neighborhood.
         */
        virtual int GetNrNeighbors(void) const
        {
            return m_nrNeighbors;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Set the number of neighbors in the user-defined neighborhood.
         */
        virtual void SetNrNeighbors(const int nrNeighbors)
        {
            m_nrNeighbors = nrNeighbors;
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get the number of times the vertex has been selected for tree expansions.
         */
        virtual int GetNrSelections(void) const
        {
            return m_nrSel;
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get the number of times the vertex has been selected for tree expansions.
         */
        virtual void SetNrSelections(const int nrSel)
        {
            m_nrSel = nrSel;
        }
        
    protected:
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Number of neighbors in the user-defined neighborhood.
         */
        int m_nrNeighbors;
        

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Number of times a vertex has been selected for tree expansions.
         */
        int m_nrSel;
        
    };

        
    class EST : public TreeSamplingBasedPlanner
    {
    public:
        EST(void) : TreeSamplingBasedPlanner(),
                    m_neighborhoodRadius(Constants::VAL_EST_NeighborhoodRadius),
                    m_totalWeight(0.0)
        {
        }

        virtual ~EST(void)
        {
        }

       
        virtual void Info(const char prefix[]) const
        {
            TreeSamplingBasedPlanner::Info(prefix);
            Logger::m_out << prefix << " NeighborhoodRadius = " << GetNeighborhoodRadius() << std::endl;
        }


        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Set the parameter values of the planner from the given parameters.
         * 
         *@remarks
         * - Function first invokes TreeSamplingBasedPlanner::SetupFromParams(params).
         * - It then sets the neighborhood radius (keyword Constants::KW_NeighborhoodRadius)
         * - It uses the parameter group associated with the keyword Constants::KW_EST.
         * - The parameter values can be specified in a text file as, for example,
         *     <center><tt>EST { NeighborhoodRadius 1.0 }  }</tt></center>
         */        
        virtual void SetupFromParams(Params & params)
        {
            TreeSamplingBasedPlanner::SetupFromParams(params);

            auto data = params.GetData(Constants::KW_EST);
            if(data && data->m_params)
                SetNeighborhoodRadius(data->m_params->GetValueAsDouble(Constants::KW_NeighborhoodRadius,
                                                                      GetNeighborhoodRadius()));
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get the radius of the neighboorhod used to measure
         *       the tree-coverage density.
         */
        virtual double GetNeighborhoodRadius(void) const
        {
            return m_neighborhoodRadius;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Set the radius of the neighboorhod used to measure
         *       the tree-coverage density.
         */
        virtual void SetNeighborhoodRadius(const double r)
        {
            m_neighborhoodRadius = r;
        }


    protected:
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Factory for creating new planner vertices.
         *
         *@remarks
         * - Create ESTVertex instead of PlannerVertex. 
         */
        virtual PlannerVertex* NewVertex(void) const
        {
            return new ESTVertex();
        }

        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Add a new vertex to the graph with <tt>cfg</tt> as its associated configuration.
         *
         *@remarks
         * - The function also computes the number of vertices that fall inside the 
         *   neighborhood centered at <tt>cfg</tt> (configurations whose distance from
         *   <tt>cfg</tt> is no more than <tt>GetNeighboorhodRadius().</tt> For each
         *   of these configurations, it also increments the number of neighbors counter
         *   by one (since <tt>cfg</tt> will be inside their neighborhoods).
         * 
         * - The function does not make a copy of <tt>cfg</tt>; it simply does pointer assignment.
         * - The function does not check if <tt>cfg</tt> is already in the graph. It is the 
         *   responsibility of the calling function to perform such a check (using FindCfg)
         *   if indeed there is a possibility that the same configuration is added to the 
         *   graph multiple times (which even if it happened would not cause any errors).
         */
        virtual int AddVertex(Cfg * const cfg);
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get the vertex weight.
         *
         *@remarks
         * - The weight is high when the vertex has few neighbors inside
         *   its neighborhood in  order to promote expansions from sparsely explored areas. 
         * - To avoid always selecting the same vertex, the weight 
         *   decreases with the number of selections. 
         */
        virtual double GetVertexWeight(const ESTVertex & v) const;

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Select a vertex from which to expand the tree and 
         *       a target toward which to expand the tree.
         *
         *@remarks
         * - Function returns the id of the selected vertex.
         * - The vertex is selected with probability proportional to its weight.
         * - TreeSamplingBasedPlanner::SelectTarget is used to select the target.
         */
        virtual int SelectVertex(void);
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Radius of the neighboorhod used to measure
         *       the tree-coverage density.
         */
        double m_neighborhoodRadius;

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Sum of weights of associates with the tree vertices.
         */
        double m_totalWeight;
        
    };
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to EST.
     */
    ClassContainer(EST, m_est);
    
}

#endif
