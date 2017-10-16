#ifndef Antipatrea__CfgAcceptorBasedOnEnergy_HPP_
#define Antipatrea__CfgAcceptorBasedOnEnergy_HPP_

#include "Components/CfgAcceptors/CfgAcceptor.hpp"
#include "Components/CfgEnergyEvaluators/CfgEnergyEvaluator.hpp"
#include <cmath>

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Motion-planning component which accepts a configuration only if its energy is below 
     *       a user-defined threshold.
     *
     *@remarks
     *  - This class requires access to CfgEnergyEvaluator component in order to evaluate the 
     *    energy of the configuration.
     *  - The user should also set the energy threshold.
     */
    class CfgAcceptorBasedOnEnergy : public CfgAcceptor,
				     public CfgEnergyEvaluatorContainer
    {
    public:
	CfgAcceptorBasedOnEnergy(void) : CfgAcceptor(),
					 CfgEnergyEvaluatorContainer()
	{
	    m_energyThreshold = IsUsedAsGoalAcceptor() ?
		Constants::VAL_GoalAcceptorBasedOnEnergy_EnergyThreshold :
		Constants::VAL_CfgAcceptorBasedOnEnergy_EnergyThreshold;
	}
	
	virtual ~CfgAcceptorBasedOnEnergy(void)
	{
	}

	virtual bool CheckSetup(void)
	{
	    return
		CfgAcceptor::CheckSetup() &&
		GetCfgEnergyEvaluator() != NULL &&
		GetCfgEnergyEvaluator()->CheckSetup();
	}

	virtual void Info(const char prefix[]) const
	{
	    CfgAcceptor::Info(prefix);
	    Logger::m_out << prefix << " EnergyThreshold    = " << GetEnergyThreshold() << std::endl
	                  << prefix << " CfgEnergyEvaluator = " << Name(GetCfgEnergyEvaluator()) << std::endl;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 * 
	 *@remarks
	 * - Function first invokes CfgAcceptor::SetupFromParams(params).
	 * - It then sets the energy threshold (keyword Constants::KW_EnergyThreshold).
	 * - When used as a goal acceptor, it uses the parameter group associated with the keyword Constants::KW_GoalAcceptorBasedOnEnergy.
	 * - When used as general cfg acceptor, it uses the parameter group associated with the keyword Constants::KW_CfgAcceptorBasedOnEnergy
	 * - The parameter value can be specified in a text file as, for example,
	 *     <center><tt>CfgAcceptorBasedOnEnergy { EnergyThreshold -12.34 }</tt></center>
	 *   or 
	 *     <center><tt>GoalAcceptorBasedOnEnergy { EnergyThreshold -100.12 }</tt></center>
	 */
	virtual void SetupFromParams(Params & params)
	{
	    CfgAcceptor::SetupFromParams(params);

	    auto data = params.GetData(IsUsedAsGoalAcceptor() ? Constants::KW_GoalAcceptorBasedOnEnergy : Constants::KW_CfgAcceptorBasedOnEnergy);
	    if(data && data->m_params)
		SetEnergyThreshold(data->m_params->GetValueAsDouble(Constants::KW_EnergyThreshold, GetEnergyThreshold()));
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Name says it all.
	 */
	virtual double GetEnergyThreshold(void) const
	{
	    return m_energyThreshold;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Name says it all.
	 */
	virtual void SetEnergyThreshold(const double energyThreshold)
	{
	    m_energyThreshold = energyThreshold;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Returns true iff the energy of <tt>cfg</tt> is below the user-defined threshold.
	 *
	 *@remarks
	 * - The function will call CfgEnergyEvaluator component to evaluate the energy of <tt>cfg</tt>
	 * - It will do so, however, only if the energy of <tt>cfg</tt> has not been previously evaluated
	 *   (it checks whether the energy field is undefined -- if so, it computes the energy; otherwise, it accesses the energy value).
	 */
	virtual bool IsAcceptable(Cfg & cfg)
	{
	    if(cfg.IsEnergySet() == false)
		cfg.SetEnergy(GetCfgEnergyEvaluator()->EvaluateEnergy(cfg));

	    return GetEnergyThreshold() >= cfg.GetEnergy();
	}

    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief User-defined energy threshold.
	 */
	double m_energyThreshold;
	
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgAcceptorBasedOnEnergy.
     */
    ClassContainer(CfgAcceptorBasedOnEnergy, m_cfgAcceptorBasedOnEnergy);

    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Redefine CfgAcceptorBasedOnEnergy as GoalAcceptorBasedOnEnergy since the same class can be used for either purpose.
     */
    typedef CfgAcceptorBasedOnEnergy GoalAcceptorBasedOnEnergy;

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to GoalAcceptorBasedOnEnergy.
     */	
    ClassContainer(GoalAcceptorBasedOnEnergy, m_goalAcceptorBasedOnEnergy);
  
}

#endif
