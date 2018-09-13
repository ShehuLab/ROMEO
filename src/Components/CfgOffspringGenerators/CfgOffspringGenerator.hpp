#ifndef Antipatrea__CfgOffspringGenerator_HPP_
#define Antipatrea__CfgOffspringGenerator_HPP_

#include "Components/CfgManagers/CfgManager.hpp"
#include "Setup/Defaults.hpp"

namespace Antipatrea
{        
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Generate an offspring configuration near the parent configuration.
     * 
     *@remarks
     * - A parameter, m_step, provides a measure of how far/different the offspring should be from its parent.
     */
    class CfgOffspringGenerator : public Component,
                                  public CfgManagerContainer
    {
    public:
        CfgOffspringGenerator(void) : Component(),
                                      CfgManagerContainer(),
                                      m_cfgParent(NULL),
                                      m_step(Constants::VAL_CfgOffspringGenerator_Step)
        {
        	m_verboseFlag = false;
        }
        
        virtual ~CfgOffspringGenerator(void)
        {
        }
        
        virtual bool CheckSetup(void) const
        {
            return
                Component::CheckSetup() &&
                GetCfgManager() != NULL &&
                GetCfgManager()->CheckSetup();
        }

        virtual void Info(const char prefix[]) const
        {
            Component::Info(prefix);
            Logger::m_out << prefix << " CfgManager = " << Name(GetCfgManager()) << std::endl
                          << prefix << " Step       = " << GetStep() << std::endl;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Set the parameter values of the component from the given parameters.
         * 
         *@remarks
         * - Function first invokes Component::SetupFromParams(params). 
         * - It then sets the step size used for guiding the offspring generation (keyword Constants::KW_Step).
         * - It uses the parameter group associated with the keyword Constants::KW_CfgOffspringGenerator.
         * - The parameter value can be specified in a text file as, for example,
         *     <center><tt>CfgOffspringGenerator { Step 0.1 }</tt></center>
         */
        virtual void SetupFromParams(Params & params)
        {
            Component::SetupFromParams(params);

            auto data = params.GetData(Constants::KW_CfgOffspringGenerator);
            if(data && data->m_params) {
				SetStep(data->m_params->GetValueAsDouble(Constants::KW_Step, GetStep()));
				m_verboseFlag = data->m_params->GetValueAsBool(Constants::KW_VerboseFlag,false);
            }
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get the pointer to the parent configuration.
         */
        virtual const Cfg* GetParentCfg(void) const
        {
            return m_cfgParent;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Set the pointer to the parent configuration.
         */
        virtual void SetParentCfg(const Cfg * const cfgParent)
        {
            m_cfgParent = cfgParent;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get the step size used for guiding the offspring generation.
         *
         *@remarks
         *  Step size is a measure of how far/different the offspring should be from its parent.
         */
        virtual double GetStep(void) const
        {
            return m_step;
        }

        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Get the step size used for guiding the offspring generation.
         *
         *@remarks
         *  Step size is a measure of how far/different the offspring should be from its parent.
         */
        virtual void SetStep(const double step)
        {
            m_step = step;
        }
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Generate the offspring.
         */ 
        virtual void GenerateOffspringCfg(Cfg & cfg) = 0;

    protected:
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Pointer to the parent configuration.
         */
        const Cfg *m_cfgParent;
        
        /**
         *@author Erion Plaku, Amarda Shehu
         *@brief Step size to guide the offspring generation.
         *
         *@remarks
         *  Step size is a measure of how far/different the offspring should be from its parent.
         */
        double m_step;

        /**
         *@author Kevin Molloy, Erion Plaku, Amarda Shehu
         *@brief Verbosity Flag for all offspring generators.
         *
         */
        bool m_verboseFlag;

    };
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgOffspringGenerator.
     */         
    ClassContainer(CfgOffspringGenerator, m_cfgOffspringGenerator);
    
}

#endif
