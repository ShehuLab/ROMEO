/*
 * CfgAcceptorBasedOnFixedMMC.hpp
 *
 *  Created on: Jul 19, 2017
 *      Author: kmolloy
 */

#ifndef SRC_COMPONENTS_CFGACCEPTORS_CFGACCEPTORBASEDONFIXEDMMC_HPP_
#define SRC_COMPONENTS_CFGACCEPTORS_CFGACCEPTORBASEDONFIXEDMMC_HPP_


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
     *       is below accepted based on a fixed temperature and a Metropolis test.
     *
     *@remarks
     *  - This class requires access to CfgEnergyEvaluator component in order to evaluate the
     *    energy of the configuration.
     *  - The initial temperature and adjustment factor are user parameters.
     */
    class CfgAcceptorBasedOnFixedMMC : public CfgAcceptor,
                                                       public CfgEnergyEvaluatorContainer
    {
    public:
        CfgAcceptorBasedOnFixedMMC(void) : CfgAcceptor(),
                                           CfgEnergyEvaluatorContainer()
        {
            m_acceptDeltaE = 10;
            m_acceptProb   = 0.10;

            m_maxEnergy = -1. * std::numeric_limits<double>::max();
            m_minEnergy = std::numeric_limits<double>::max();
            m_testSuccessCount = 0;
            m_testFailCount = 0;
        }

        virtual ~CfgAcceptorBasedOnFixedMMC(void)
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
            Logger::m_out << prefix << " MMC Temperature    = " << m_temperature << std::endl
                          << prefix << " CfgEnergyEvaluator = " << Name(GetCfgEnergyEvaluator()) << std::endl;
        }

        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief Set the parameter values of the component from the given parameters.
         *
         *@remarks
         * - Function first invokes CfgAcceptor::SetupFromParams(params).
         * - Used as general cfg acceptor, it uses the parameter group associated with
         *   the keyword Constants::KW_CfgAcceptorBasedOnFixedMMC.
         * - The parameter value can be specified in a text file as, for example,
         *     <center><tt>CfgAcceptorBasedOnFixedMMC { }</tt></center>
         */
        virtual void SetupFromParams(Params & params)
        {
            CfgAcceptor::SetupFromParams(params);

            auto data = params.GetData(Constants::KW_CfgAcceptorBasedOnFixedMMC);
            if(data && data->m_params)
            {
                m_acceptDeltaE = data->m_params->GetValueAsDouble(Constants::KW_FixedAcceptDeltaE,m_acceptDeltaE);
                // verify prob value is valid ( > 0 and < 1)








                

                m_acceptProb =
                    data->m_params->GetValueAsDouble(Constants::KW_FixedAcceptProb,m_acceptProb);
            }

            double lgAcceptProb= log(m_acceptProb);
            m_temperature = (-1 * m_acceptDeltaE)/(Constants::VAL_Boltzman_K*lgAcceptProb);
            Logger::m_out << "Fixed MMC: temp set to:" << m_temperature
                          << ", yielding a:"<< m_acceptProb << " change of accepting a:" << m_acceptDeltaE
                          << " change in energy.\n";

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


            if (cfg.GetEnergy() > m_maxEnergy)
                m_maxEnergy = cfg.GetEnergy();

            if (cfg.GetEnergy() < m_minEnergy)
                m_minEnergy = cfg.GetEnergy();

            // Metropolis-test
            bool passMMC = true;

            double deltaE = cfg.GetEnergy() - m_cfgSource->GetEnergy();
            double randomDraw = 0.0;
            double acceptProb = 1.0;

            if (deltaE > 0.0)
            {
                acceptProb = exp(-1 * deltaE/(Constants::VAL_Boltzman_K * m_temperature));

                randomDraw = RandomUniformReal();
                if (acceptProb < randomDraw)
                {
                    passMMC = false;
                }
            }

            (passMMC) ? ++m_testSuccessCount : ++m_testFailCount;
            if (m_verboseFlag)
            {
				Logger::m_out  << "MMCTest passMMC:" << passMMC
							   << " temp:" << m_temperature
							   << " deltaE:" << deltaE
							   << " new cfg energy:" << cfg.GetEnergy()
							   << " prob:" << acceptProb
							   << " random:" << randomDraw << "\n";
            }
            return (passMMC);
        }

    protected:
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Accept Delta E (for calculation of temperature).
         */
        double m_acceptDeltaE;

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief Desired probability of accepted DeltaE change.
        */
        double m_acceptProb;

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief temperature for MMC test.
        */
        double m_temperature;


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
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  Count of number of passed MMC tests.
        */
        unsigned int m_testSuccessCount;

        /**
          *@author Kevin Molloy, Erion Plaku, Amarda Shehu
          *@brief  Count of number of failed MMC tests.
        */
        unsigned int m_testFailCount;

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Pointer to prior/source cfg.
        */
        Cfg *m_cfgSource;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgAcceptorBasedOnFixedMMC.
     */
    ClassContainer(CfgAcceptorBasedOnFixedMMC, m_cfgAcceptorBasedOnFixedMMC);

}

#endif /* SRC_COMPONENTS_CFGACCEPTORS_CFGACCEPTORBASEDONFIXEDMMC_HPP_ */
