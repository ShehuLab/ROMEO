#ifndef Antipatrea__CfgDistanceLp_HPP_
#define Antipatrea__CfgDistanceLp_HPP_

#include "Components/CfgDistances/CfgDistance.hpp"
#include "Components/CfgManagers/CfgManager.hpp"
#include "Components/CfgDistances/SignedDistanceBetweenTwoValues.hpp"

namespace Antipatrea
{
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Defines the distance between two configurations as the Lp norm 
     *       of the pairwise distances between configuration values.
     *
     *
     *@remarks
     *  - Distance is computed as <tt>LpNorm(a_{0}, ..., a_{dim-1})</tt>
     *          where <tt> a_{i} = GetSignedDistanceBetweenTwoValues()->SignedDistance(cfg1->GetValues()[i], cfg2->GetValues()[i])</tt>
     *  - This class requires access to SignedDistanceBetweenTwoValues component in order to compute the 
     *    distance between any two values.
     *  - It also requires access to CfgManager to access the configuration dimension.
     *  - The user also needs to set the exponent p in the Lp norm.
     */

    class CfgDistanceLp : public CfgDistance,
                          public CfgManagerContainer,
                          public SignedDistanceBetweenTwoValuesContainer
    {
    public:
        CfgDistanceLp(void) : CfgDistance(),
                              CfgManagerContainer(),
                              SignedDistanceBetweenTwoValuesContainer(),
                              m_exponent(Constants::VAL_CfgDistanceLp_Exponent)
                                        
        {
        }
        
        virtual ~CfgDistanceLp(void)
        {
        }
        
        virtual bool CheckSetup(void) const
        {
            return
                CfgDistance::CheckSetup() &&
                GetCfgManager() != NULL &&
                GetSignedDistanceBetweenTwoValues() != NULL &&
                GetCfgManager()->CheckSetup() &&
                GetSignedDistanceBetweenTwoValues()->CheckSetup();
        }

        virtual void Info(const char prefix[]) const
        {
            CfgDistance::Info(prefix);
            Logger::m_out << prefix << " CfgManager          = " << Name(GetCfgManager()) << std::endl
                          << prefix << " SignedDistanceBetweenTwoValues = " << Name(GetSignedDistanceBetweenTwoValues()) << std::endl;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Set the parameter values of the component from the given parameters.
         * 
         *@remarks
         * - Function first invokes CfgDistance::SetupFromParams(params).
         * - It then sets the exponent for the Lp norm (keyword Constants::KW_Exponent).
         * - It uses the parameter group associated with the keyword Constants::KW_CfgDistanceLp.
         * - The parameter value can be specified in a text file as, for example,
         *     <center><tt>CfgDistanceLp { Exponent 2 }</tt></center>
         */
        virtual void SetupFromParams(Params & params)
        {
            CfgDistance::SetupFromParams(params);

            auto data = params.GetData(Constants::KW_CfgDistanceLp);
            if(data && data->m_params)
                SetExponent(data->m_params->GetValueAsInt(Constants::KW_Exponent, GetExponent()));
        }

        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get the exponent used for the Lp norm calculation.
         */
        virtual int GetExponent(void) const
        {
            return m_exponent;
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Set the exponent used for the Lp norm calculation.
         */
        virtual void SetExponent(int exponent)
        {
            m_exponent = exponent;
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Computes the distance between two configurations as the Lp norm of the pairwise distances between configuration values.
         *
         *@remarks
         *  - Distance is computed as <tt>LpNorm(a_{0}, ..., a_{dim-1})</tt>
         *    where <tt> a_{i} = GetSignedDistanceBetweenTwoValues()->SignedDistance(cfg1->GetValues()[i], cfg2->GetValues()[i])</tt>
         */
        virtual double Distance(const Cfg & cfg1,
                                const Cfg & cfg2);


    protected:
        int m_exponent;
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgDistanceLp.
     */         
    ClassContainer(CfgDistanceLp, m_cfgDistanceLp);
    
}

#endif
