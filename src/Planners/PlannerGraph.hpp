#ifndef Antipatrea___PlannerGraph_HPP_
#define Antipatrea___PlannerGraph_HPP_

#include "Planners/PlannerVertex.hpp"
#include "Components/Component.hpp"
#include "Utils/DisjointSet.hpp"
#include "Utils/HashFn.hpp"
#include <vector>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Representation of the undirected graph (roadmap or tree)
     *       constructed by a sampling-based motion planner.
     *
     *@remarks
     * - The graph has the following:
     *    - a set of vertices;
     *    - a set of edges; and
     *    - a set of connected components represented as a disjoint set.
     * - The set of edges is the union of the edges kept by each vertex.
     *   It is kept as a separate set to facilitate enumeration and searching.
     * - It is assumed that a sampling-based motion planner will
     *   construct an undirected graph, but the edge cost from A to B could
     *   be different from the edge cost from B to A. This could happen, 
     *   for example, when the edge cost is based on the energy change.
     * - For this reason, and to facilitate the graph search, the graph
     *   maintains the edges as directed edges. In other words,
     *   when a sampling-based motion planner makes a connection from 
     *   vertex A to B, it needs to add two edges to the graph, namely,
     *   the edge (A, B) with cost from A to B and the edge (B, A)
     *   with cost from B to A.
     */
    class PlannerGraph : public Component
    {
    public:
        PlannerGraph(void) : Component()
        {
        }

        virtual ~PlannerGraph(void);

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get the number of vertices.
         */
        virtual int GetNrVertices(void) const
        {
            return m_vertices.size();
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get a pointer to the <tt>i</tt>-th vertex.
         *
         *@remarks
         * - <tt>i</tt> should be between <tt>0</tt> and <tt>GetNrVertices()-1</tt>
         * - The function does not check whether <tt>i</tt> is within the proper range.
         */
        virtual const PlannerVertex* GetVertex(const int vid) const
        {
            return m_vertices[vid];
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get a pointer to the <tt>i</tt>-th vertex.
         *
         *@remarks
         * - <tt>i</tt> should be between <tt>0</tt> and <tt>GetNrVertices()-1</tt>
         * - The function does not check whether <tt>i</tt> is within the proper range.
         */
        virtual PlannerVertex* GetVertex(const int vid) 
        {
            return m_vertices[vid];
        }

        /**
              *@author Erion Plaku, Amarda Shehu
              *@brief Get a pointer to by CFG address (yes it is ugly).
              *
         */

		 virtual int GetVertexByCfg(const Cfg *cfg)
		 {
			 unsigned int vid=0;
			 for (unsigned int i(0);i< m_vertices.size();++i) {
				 if (m_vertices[i]->GetCfg() == cfg)
				 {
					 vid = i;
				 	 break;
				 }
			 }
			return vid;
		 }
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Add the vertex to the graph.
         *
         *@remarks
         * - The function also creates a new graph component with <tt>v</tt> as its only vertex.
         */
        virtual int AddVertex(PlannerVertex * const v)
        {
            v->SetDisjointSetElem(m_components.Make());
            m_vertices.push_back(v);
            return GetNrVertices() - 1;
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get the number of edges.
         */
        virtual int GetNrEdges(void) const
        {
            return m_edges.size();
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief The key is defined as  <tt>(min(vidFrom, vidTo), max(vidFrom, vidTo))</tt>.
         */
        virtual std::pair<int, int> GetEdgeKey(const int vidFrom, const int vidTo) const
        {
            return std::make_pair(std::min(vidFrom, vidTo), std::max(vidFrom, vidTo));
        }
                
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Return a pointer to the edge <tt>(vidFrom, vidTo)</tt>.
         *
         *@remarks
         * - If the edge is not in the graph, the function returns <tt>NULL</tt>.
         */
        virtual PlannerEdge* FindEdge(const int vidFrom, const int vidTo) const
        {
            auto curr = m_edges.find(GetEdgeKey(vidFrom, vidTo));
            if(curr == m_edges.end())
                return NULL;
            else
                return curr->second;
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Add the edge to the graph.
         *
         *@remarks
         * - The function does not check if the edge is already in the graph.
         *   It is the responsibility of the calling function to perform this check (using FindEdge)
         *   if indeed there may be a possibility that the same edge could be added multiple times.
         * - The function also joins the components associated with the end vertices of the edge.
         */
        virtual void AddEdge(PlannerEdge * const edge);

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Return true iff the two vertices are connected by a path.
         *
         *@remarks
         * - This function uses the disjoint-set data structure, so it works
         *   correctly only if the graph is undirected, i.e.,
         *   if (A, B) is an edge in the graph, then so is (B, A).
         */
        virtual bool AreVerticesPathConnected(const int vidFrom, const int vidTo)
        {
            return m_components.Same(GetVertex(vidFrom)->GetDisjointSetElem(),
                                     GetVertex(vidTo)->GetDisjointSetElem());
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get a pointer to the disjoint-set data structure representing the connected components in the graph.
         */
        virtual const DisjointSet* GetComponents(void) const
        {
            return &m_components;
        }

        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get a pointer to the disjoint-set data structure representing the connected components in the graph.
         */
        virtual DisjointSet* GetComponents(void)
        {
            return &m_components;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Define the data structure (unordered map with the vertex pair as the key) to store
         *       all the edges of the graph.
         */
        typedef std::unordered_map< std::pair<int, int>, PlannerEdge*, HashStruct< std::pair<int, int> > > Edges;

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get a pointer to the data structure storing all the edges of the graph.
         */
        virtual const Edges* GetEdges(void) const
        {
            return &m_edges;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get a pointer to the data structure storing all the edges of the graph.
         */
        virtual Edges* GetEdges(void)
        {
            return &m_edges;
        }

        
    protected:
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Vertices of the graph.
         */
        std::vector<PlannerVertex*> m_vertices;

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Connected components of the graph.
         */
        DisjointSet m_components;
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Edges of the graph.
         */
        Edges m_edges;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to PlannerGraph.
     */
    ClassContainer(PlannerGraph, m_plannerGraph);
    
}

#endif
