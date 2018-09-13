#ifndef Antipatrea__Planner_HPP_
#define Antipatrea__Planner_HPP_

#include "Planners/PlannerProblem.hpp"
#include "Planners/PlannerSolution.hpp"
#include "Utils/Reader.hpp"
#include "Utils/Writer.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface for a planner which takes a planning problem, tries to solve it, and, 
     *       if successful, provides a solution.
     * 
     *@remarks
     * - This class requires access to PlannerProblem to access the planning problem.
     */
    class Planner : public Component,
                    public PlannerProblemContainer,
                    public Reader,
                    public Writer
    {
    public:
        Planner(void) : Component(),
                        PlannerProblemContainer(),
                        Reader(),
                        Writer()
        {
        }

        virtual ~Planner(void)
        {
        }

        virtual bool CheckSetup(void)
        {
            return
                Component::CheckSetup() &&
                GetPlannerProblem() != NULL &&
                GetPlannerProblem()->CheckSetup();
        }

        virtual void Info(const char prefix[]) const
        {
            Component::Info(prefix);
            Logger::m_out << prefix << " PlannerProblem = " << Name(GetPlannerProblem()) << std::endl;
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Initialize the planner.
         *
         *@remarks
         * - This function should be called only once at the beginning
         *   after the problem and other components have been setup.
         */
        virtual void Start(void) = 0;

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Run the planner for a certain amount of time.
         *
         *@remarks
         * - Solve should be called after Start.
         * - Solve can be called repeatedly until a solution is found.
         * - Solve returns true as soon as it finds a solution.
         */
        virtual bool Solve(const double tmax) = 0;

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Return true iff the planner has found a solution.
         */
        virtual bool IsSolved(void) = 0;

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Construct the solution that the planner has found.
         *
         *@remarks
         * - The function returns true iff the planner has found a solution.
         */
        virtual bool GetSolution(PlannerSolution & sol) = 0;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to Planner.
     */
    ClassContainer(Planner, m_planner);
    
}

#endif
