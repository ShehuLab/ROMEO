#ifndef Antipatrea__CfgAcceptor_HPP_
#define Antipatrea__CfgAcceptor_HPP_

#include "Components/CfgManagers/CfgManager.hpp"

#include<iostream>

namespace Antipatrea
{        
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface for determining whether or not a given configuration is acceptable.
     * 
     *@remarks
     * - This class provides a general interface for checking whether or not a given configuration is acceptable.
     * - It should be extended with specific implementations.
     * - The class can be used for two purposes: 
     *    - (1) to determine whether a configuration in PRM, RRT, or any other 
     *         sampling-based approach satisfies the desired constraints, 
     *         e.g., its energy level is below a certain threshold; and
     *    - (2) to determine whether a configuration satisfies the goal,
     *          e.g., its energy level is close to a native state, or is really close to 
     *          the goal configuration.                   
     *
     *    By default it is used for the first purpose. The member function UseAsGoalAcceptor(true) 
     *    should be called when an object from this class is used to test goal acceptance.
     */
    class CfgAcceptor : public Component
    {
    public:
        CfgAcceptor(void) : Component(),
                            m_useAsGoalAcceptor(false)
        {
        	m_verboseFlag = false;
        }
        
        virtual ~CfgAcceptor(void)
        {
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Returns true iff the configuration <tt>cfg</tt> is acceptable.
         */
        virtual bool IsAcceptable(Cfg & cfg) = 0;

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief When possible, return a pointer to a configuration that is acceptable.
         *
         *@remarks
         * - Such function is useful for implementing biased sampling in RRT-like motion planners.
         *   As an illustration, the acceptor could be used for goal testing. The function
         *   could return a pointer to the goal configuration.
         *
         * - The returned pointer should not be deleted since the function may return the 
         *   address of a class variable. Deleting the pointer could cause memory issues.
         */
        virtual Cfg* GetAnAcceptableCfg(void)
        {
            return NULL;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get the class purpose: goal acceptor (true) or configuration acceptor (false).
         */
        virtual bool IsUsedAsGoalAcceptor(void) const
        {
            return m_useAsGoalAcceptor;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Set the class purpose: goal acceptor (true) or configuration acceptor (false).
         */
        virtual void UseAsGoalAcceptor(const bool use)
        {
            m_useAsGoalAcceptor = use;
        }
        
        virtual void SetupFromParams(Params & params)
        {
        	Component::SetupFromParams(params);

            auto data = params.GetData(IsUsedAsGoalAcceptor() ? Constants::KW_GoalAcceptorBasedOnDistance : Constants::KW_CfgAcceptorBasedOnDistance);
            if(data && data->m_params) {
            	m_verboseFlag = data->m_params->GetValueAsBool(Constants::KW_VerboseFlag,false);
            }

        }
    protected:
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Class purpose: goal acceptor (true) or configuration acceptor (false).
         */
        bool m_useAsGoalAcceptor;

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief flag to control verbosity
          */
        bool m_verboseFlag;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgAcceptor.
     */
    ClassContainer(CfgAcceptor, m_cfgAcceptor);

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Redefine CfgAcceptor as GoalAcceptor since the same class can be used for either purpose.
     */
    typedef CfgAcceptor GoalAcceptor;
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to GoalAcceptor.
     */
    ClassContainer(GoalAcceptor, m_goalAcceptor);
    
}

#endif
