#ifndef Antipatrea__PlannerProblem_HPP_
#define Antipatrea__PlannerProblem_HPP_

#include "Components/CfgManagers/Cfg.hpp"
#include "Components/CfgAcceptors/CfgAcceptor.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief The planning problem is defined by the initial configuration and a goal acceptor.
     * 
     *@remarks
     * - This class requires access to GoalAcceptor to determine whether or not the goal has been reached.
     */
    class PlannerProblem : public Component,
                           public GoalAcceptorContainer
    {
    public: 
        PlannerProblem(void) : Component(),
                               GoalAcceptorContainer(),
                               m_cfgInit(NULL)
        {
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@remarks
         * - Destructor deletes the initial configuration.
         */
        virtual ~PlannerProblem(void)
        {
            if(m_cfgInit)
                delete m_cfgInit;
        }

        virtual bool CheckSetup(void)
        {
            return 
                Component::CheckSetup() &&
                GetGoalAcceptor() != NULL &&
                GetGoalAcceptor()->CheckSetup();
        }
        
        virtual void Info(const char prefix[]) const
        {
            Component::Info(prefix);
            Logger::m_out << prefix << " GoalAcceptor = " << Name(GetGoalAcceptor()) << std::endl;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get a pointer to the initial configuration.
         */
        virtual const Cfg* GetInitialCfg(void) const
        {
            return m_cfgInit;
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get a pointer to the initial configuration.
         */
        virtual Cfg* GetInitialCfg(void)
        {
            return m_cfgInit;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Set the initial configuration.
         */
        virtual void SetInitialCfg(Cfg * const cfg)
        {
            m_cfgInit = cfg;
        }
        
    protected:
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Pointer to the initial configuration.
         */
        Cfg *m_cfgInit;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to PlannerProblem.
     */
    ClassContainer(PlannerProblem, m_plannerProblem);
    
}

#endif
