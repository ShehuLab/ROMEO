#ifndef Antipatrea__CfgAcceptorBasedOnDistance_HPP_
#define Antipatrea__CfgAcceptorBasedOnDistance_HPP_

#include "Components/CfgAcceptors/CfgAcceptor.hpp"
#include "Components/CfgDistances/CfgDistance.hpp"
#include "Setup/Defaults.hpp"
#include <cmath>

#include <iostream>

namespace Antipatrea
{	
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Motion-planning component which accepts a configuration only if it is within 
     *       a user-defined distance from a target/goal configuration.
     *
     *@remarks
     *  - This class requires access to CfgDistance component in order to compute
     *    the distance between two configurations.
     *  - The user should also set the distance threshold.
     */

    class CfgAcceptorBasedOnDistance : public CfgAcceptor,
				       public CfgDistanceContainer
    {
    public:
	CfgAcceptorBasedOnDistance(void) : CfgAcceptor(),
					   CfgDistanceContainer(),
					   m_cfgTarget(NULL)
	{
	    m_distanceThreshold = IsUsedAsGoalAcceptor() ?
	    		Constants::VAL_GoalAcceptorBasedOnDistance_DistanceThreshold :
				Constants::VAL_CfgAcceptorBasedOnDistance_DistanceThreshold;

	    m_minDistanceAboveThreshold = 99999999999999.9;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
         *
	 *@remarks
	 *  - It does not delete m_cfgTarget since it is not the one
	 *    that allocates it. 
	 */
	virtual ~CfgAcceptorBasedOnDistance(void)
	{
	}

	virtual bool CheckSetup(void)
	{
	    return
		CfgAcceptor::CheckSetup() &&
		GetCfgDistance() != NULL &&
		GetCfgDistance()->CheckSetup();
	}
	
	virtual void Info(const char prefix[]) const
	{
	    CfgAcceptor::Info(prefix);
	    Logger::m_out << prefix << " DistanceThreshold = " << GetDistanceThreshold() << std::endl
		          << prefix << " CfgTarget         = " << m_cfgTarget << std::endl
	                  << prefix << " CfgDistance       = " << Name(GetCfgDistance()) << std::endl;
	}


	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 * 
	 *@remarks
	 * - Function first invokes CfgAcceptor::SetupFromParams(params).
	 * - It then sets the distance threshold (keyword Constants::KW_DistanceThreshold).
	 * - When used as a goal acceptor, it uses the parameter group associated with the keyword Constants::KW_GoalAcceptorBasedOnDistance.
	 * - When used as general cfg acceptor, it uses the paramater group associated with the keyword Constants::KW_CfgAcceptorBasedOnDistance
	 * - The parameter value can be specified in a text file as, for example,
	 *     <center><tt>CfgAcceptorBasedOnDistance { DistanceThreshold  0.5 }</tt></center>
	 *   or 
	 *     <center><tt>GoalAcceptorBasedOnDistance { DistanceThreshold 0.01 }</tt></center>
	 */	
	virtual void SetupFromParams(Params & params)
	{
	    CfgAcceptor::SetupFromParams(params);

	    auto data = params.GetData(IsUsedAsGoalAcceptor() ? Constants::KW_GoalAcceptorBasedOnDistance : Constants::KW_CfgAcceptorBasedOnDistance);
	    if(data && data->m_params)
		SetDistanceThreshold(data->m_params->GetValueAsDouble(Constants::KW_DistanceThreshold, GetDistanceThreshold()));
	}

	
	/**
         *@author Erion Plaku, Amarda Shehu
	 *@brief Returns a pointer to the target/goal configuration.
         *
	 *@remarks
         * - The returned pointer should not be deleted since the function returns the 
	 *   address of a class variable. Deleting the pointer could cause memory issues.
	 */
	virtual Cfg* GetAnAcceptableCfg(void)
	{
	    return GetTargetCfg();
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Returns true iff the distance between <tt>cfg</tt> and the target cfg is no more than a user-defined threshold.
	 *       Kevin Molloy -- mod -- for a general exploration of the search space, a goal might not be defined.
	 *                       We avoid this computation when no target has been specified
	 */
	virtual bool IsAcceptable(Cfg & cfg)
	{
		bool accept = false;
		if (!IsUsedAsGoalAcceptor() && m_cfgTarget != NULL) {
			std::cout << "Error, isaccept called for distance with no source." << std::endl;
			exit(99);
		}
		else
		if (m_cfgTarget) // if we have a goal
		{
			double dist = GetCfgDistance()->Distance(cfg, *m_cfgTarget);
			if (dist <= GetDistanceThreshold())
			{
				accept = true;
			}
			else
			{
				if (dist < m_minDistanceAboveThreshold)
					m_minDistanceAboveThreshold = dist;
			}
		}

		return(accept);
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get pointer to target/goal cfg
	 *
	 *@remarks
	 * - Function does not perform a copy -- just returns the pointer to target/goal cfg.
	 */
	virtual Cfg* GetTargetCfg(void)
	{
	    return m_cfgTarget;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get pointer to target/goal cfg
	 *
	 *@remarks
	 * - Function does not perform a copy -- just returns the pointer to target/goal cfg.
	 */
	virtual const Cfg* GetTargetCfg(void) const
	{
	    return m_cfgTarget;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set pointer to target/goal cfg
	 *
	 *@remarks
	 * - Function does not perform a copy -- just pointer assignment.
	 */
	virtual void SetTargetCfg(Cfg * const cfg)
	{
	    m_cfgTarget = cfg;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Name says it all.
	 */
	virtual double GetDistanceThreshold(void) const
	{
	    return m_distanceThreshold;
	}
	
	virtual double GetMinDistanceAboveThresolhold() const
	{
		return m_minDistanceAboveThreshold;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Name says it all.
	 */
	virtual void SetDistanceThreshold(const double distanceThreshold)
	{
	    m_distanceThreshold = distanceThreshold;
	}

    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Pointer to target/goal cfg.
	 */
	Cfg *m_cfgTarget;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief User-defined distance threshold.
	 */
	double m_distanceThreshold;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief min distance that exceeded the threshold.
	 *       Nice to show progress towards goal
	 */
	double m_minDistanceAboveThreshold;

    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgAcceptorBasedOnDistance.
     */
    ClassContainer(CfgAcceptorBasedOnDistance, m_cfgAcceptorBasedOnDistance);

     /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Redefine CfgAcceptorBasedOnDistance as GoalAcceptorBasedOnDistance since the same class can be used for either purpose.
     */   
    typedef CfgAcceptorBasedOnDistance GoalAcceptorBasedOnDistance;

     /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to GoalAcceptorBasedOnDistance.
     */	
    ClassContainer(GoalAcceptorBasedOnDistance, m_goalAcceptorBasedOnDistance);

}

#endif
