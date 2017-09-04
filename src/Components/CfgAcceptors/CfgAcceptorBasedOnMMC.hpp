/*
 * CfgAcceptorBasedOnMMC.hpp
 *
 *  Created on: Jul 19, 2017
 *      Author: kmolloy
 */

#ifndef SRC_COMPONENTS_CFGACCEPTORS_CFGACCEPTORBASEDONMMC_HPP_
#define SRC_COMPONENTS_CFGACCEPTORS_CFGACCEPTORBASEDONMMC_HPP_


#include "Components/CfgAcceptors/CfgAcceptor.hpp"
#include "Components/CfgEnergyEvaluators/CfgEnergyEvaluator.hpp"
#include <cmath>
#include <limits>
#include <iostream>

namespace Antipatrea
{
    /**
     *@author Kevin Molloy, Erion Plaku, Amarda Shehu
     *@brief Motion-planning component which accepts a configuration only if the difference in energy
     *       is below accepted based on a temperature and a Metropolis test.
     *
     *@remarks
     *  - This class requires access to CfgEnergyEvaluator component in order to evaluate the
     *    energy of the configuration.
     *  - The user should also set the energy threshold.
     */
    class CfgAcceptorBasedOnMMC : public CfgAcceptor,
				     	 	 	 	 public CfgEnergyEvaluatorContainer
    {
    public:
    	CfgAcceptorBasedOnMMC(void) : CfgAcceptor(),
										 CfgEnergyEvaluatorContainer()
	{
	    m_temperature = Constants::VAL_CfgAcceptorBasedOnEnergy_Temperature;
	    m_temperatureRate = 0.1;

	    m_maxEnergy = -1. * std::numeric_limits<double>::max();
	    m_minEnergy = std::numeric_limits<double>::max();
	    std::cout << "in MMC constructor." << std::endl;
	}

	virtual ~CfgAcceptorBasedOnMMC(void)
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
	    Logger::m_out << prefix << " EnergyThreshold    = " << GetTemperature() << std::endl
	                  << prefix << " CfgEnergyEvaluator = " << Name(GetCfgEnergyEvaluator()) << std::endl;
	}

	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 *
	 *@remarks
	 * - Function first invokes CfgAcceptor::SetupFromParams(params).
	 * - It then sets the energy threshold (keyword Constants::KW_EnergyThreshold).
	 * - Used as general cfg acceptor, it uses the parameter group associated with
	 *   the keyword Constants::KW_CfgAcceptorBasedOnMMC.
	 * - The parameter value can be specified in a text file as, for example,
	 *     <center><tt>CfgAcceptorBasedOnMMC { Temperature 70.00 }</tt></center>
	 */
	virtual void SetupFromParams(Params & params)
	{
	    CfgAcceptor::SetupFromParams(params);

	    auto data = params.GetData(Constants::KW_CfgAcceptorBasedOnMMC);
	    if(data && data->m_params)
	    	SetTemperature(data->m_params->GetValueAsDouble(Constants::KW_MCCTemperature));
	}

	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Name says it all.
	 */
	virtual double GetTemperature(void) const
	{
	    return m_temperature;
	}

	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Name says it all.
	 */
	virtual void SetTemperature(const double temperature)
	{
	    m_temperature = temperature;
	}

	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Get pointer to target/goal cfg
	 *
	 *@remarks
	 * - Function does not perform a copy -- just returns the pointer to target/goal cfg.
	 */
	virtual const Cfg* GetSourceCfg(void) const
	{
	    return m_cfgSource;
	}

	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Set pointer to target/goal cfg
	 *
	 *@remarks
	 * - Function does not perform a copy -- just pointer assignment.
	 */
	virtual void SetSourceCfg(Cfg * const cfg)
	{
	    m_cfgSource = cfg;
	}

	/**
	 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
	 *@brief Returns true iff the energy of <tt>cfg</tt> is lower than the CfgSource
	         configuration OR it passes a probabalistic test based on the current
	         temperature.
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
	    // Metropolis-test
	    if (cfg.GetEnergy() > m_maxEnergy)
	    	m_maxEnergy = cfg.GetEnergy();

	    if (cfg.GetEnergy() < m_minEnergy)
	    	m_minEnergy = cfg.GetEnergy();

	    bool passMMC = true;

	    double deltaE = cfg.GetEnergy() - m_cfgSource->GetEnergy();
	    double priorTemp = m_temperature;
	    if (deltaE > 0.0)
	    {
	    	double acceptProb = exp(-1 * deltaE/(Constants::VAL_Boltzman_K * m_temperature));

	    	if (acceptProb < RandomUniformReal())
	    	{
	    		passMMC = false;
	    		/* adjust temperature up to increase change of success */
	    		m_temperature *= pow(2, m_temperatureRate);
	    	}
	    	else
	    	{
	    		/* decrease the temperature */
	    		// m_temperature /= pow(2, (deltaE) / (0.1 * m_maxEnergy - m_minEnergy));
	    		m_temperature /= 1.05;
	    	}

	    }
	    std::cout << "passMMC:" << passMMC << " old temp:" << priorTemp
	              << " new temp:" << m_temperature  << " deltaE:" << deltaE << std::endl;
	    return (passMMC);
	}

    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief User-defined temperature used in MMC test.
	 */
	double m_temperature;

	/**
		 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
		 *@brief Rate at which temperature increases after a failed MMC test.
	*/
	double m_temperatureRate;

	/**
		 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
		 *@brief max energy encountered so far.
	*/

	double m_maxEnergy;

	/**
		 *@author Kevin Molloy, Erion Plaku, Amarda Shehu
		 *@brief min energy encountered so far.
	*/

	double m_minEnergy;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Pointer to target/goal cfg.
	*/
	Cfg *m_cfgSource;

    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgAcceptorBasedOnMMC.
     */
    ClassContainer(CfgAcceptorBasedOnMMC, m_cfgAcceptorBasedOnMMC);

}

#endif /* SRC_COMPONENTS_CFGACCEPTORS_CFGACCEPTORBASEDONMMC_HPP_ */
