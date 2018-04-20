#ifndef Antipatrea__CfgOffspringGeneratorByGaussianPerturbation_HPP_
#define Antipatrea__CfgOffspringGeneratorByGaussianPerturbation_HPP_

#include "Components/CfgOffspringGenerators/CfgOffspringGenerator.hpp"
#include "Setup/Defaults.hpp"

namespace Antipatrea
{	
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Generate an offspring configuration near the parent configuration by perturbing one or more values.
     * 
     *@remarks
     * - The parameter, m_step, is interpreted here as the standard deviation of the Gaussian perturbation.
     * - Other parameters (m_minNrValuesToPerturb, m_maxNrValuesToPerturb, m_consecutive) are used to
     *   decide the minimum/maximum number of values to be perturbed and whether the perturbed values should be consecutive.
     * - The offspring generator first selects the number of values to be perturbed (uniformly at random from min/max values).
     * - If the consecutive flag is set to true, the start position is selected uniformly at random (from the applicable range)
     *   and the selected number of values are perturbed using a Gaussian distribution centered around the parent cfg.
     * - If the consecutive flag is set to false, a subset is selected uniformly at random (from the applicable range)
     *   and the values in the subset are perturbed using a Gaussian distribution centered around the parent cfg.
     */
    class CfgOffspringGeneratorByGaussianPerturbation : public CfgOffspringGenerator
    {
    public:
	CfgOffspringGeneratorByGaussianPerturbation(void) : CfgOffspringGenerator(),
							    m_consecutive(Constants::VAL_CfgOffspringGeneratorByGaussianPerturbation_Consecutive),
							    m_minNrValuesToPerturb(Constants::VAL_CfgOffspringGeneratorByGaussianPerturbation_MinNrValuesToPerturb),
							    m_maxNrValuesToPerturb(Constants::VAL_CfgOffspringGeneratorByGaussianPerturbation_MaxNrValuesToPerturb)
	{
	}
	
	virtual ~CfgOffspringGeneratorByGaussianPerturbation(void)
	{
	}

	
	virtual void Info(const char prefix[]) const
	{
	    CfgOffspringGenerator::Info(prefix);
	    Logger::m_out << prefix << " Consecutive          = " << GetConsecutive() << std::endl
		          << prefix << " MinNrValuesToPerturb = " << GetMinNrValuesToPerturb() << std::endl
	                  << prefix << " MaxNrValuesToPerturb = " << GetMaxNrValuesToPerturb() << std::endl;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the parameter values of the component from the given parameters.
	 * 
	 *@remarks
	 * - Function first invokes CfgOffspringGenerator::SetupFromParams(params).
	 * - It then sets the minimum number of values to perturb, maximum number of values to perturb,
	 *   and whether the values to be perturbed should be in consecutive order 
	 *   (keywords Constants::KW_Consecutive, Constants::KW_MinNrValuesToPerturb, Constants::KW_MaxNrValuesToPerturb).
	 * - It uses the parameter group associated with the keyword Constants::KW_CfgOffspringGeneratorByGaussianPerturbation.
	 * - The parameter values can be specified in a text file as, for example,
	 *     <center><tt>CfgOffspringGeneratorByGaussianPerturbation { Consecutive false MinNrValuesToPerturb 1 MaxNrValuesToPerturb 100000 }</tt></center>
	 */	
	virtual void SetupFromParams(Params & params)
	{
	    CfgOffspringGenerator::SetupFromParams(params);

	    auto data = params.GetData(Constants::KW_CfgOffspringGeneratorByGaussianPerturbation);
	    if(data && data->m_params)
	    {
			SetConsecutive(data->m_params->GetValueAsBool(Constants::KW_Consecutive, GetConsecutive()));
			SetMinNrValuesToPerturb(data->m_params->GetValueAsBool(Constants::KW_MinNrValuesToPerturb, GetMinNrValuesToPerturb()));
			SetMaxNrValuesToPerturb(data->m_params->GetValueAsBool(Constants::KW_MaxNrValuesToPerturb, GetMaxNrValuesToPerturb()));
	    }
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Indicate whether the values that are perturbed during the offspring generation should be consecutive.
	 */
	virtual bool GetConsecutive(void) const
	{
	    return m_consecutive;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the minimum number of values that should be perturbed when generating the offspring.
	 */
	virtual int GetMinNrValuesToPerturb(void) const
	{
	    return m_minNrValuesToPerturb;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Get the maximum number of values that should be perturbed when generating the offspring.
	 *
	 *@remarks
	 * - This value is capped to the number of configuration dimensions when used even
	 *   when the user initially sets it to a larger number.
	 */
	virtual int GetMaxNrValuesToPerturb(void) const
	{
	    return m_maxNrValuesToPerturb;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the falg to indicate whether the values that are perturbed during the offspring generation should be consecutive.
	 */
	virtual void SetConsecutive(const bool consecutive)
	{
	   m_consecutive = consecutive;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the minimum number of values that should be perturbed when generating the offspring.
	 */
	virtual void SetMinNrValuesToPerturb(const int n) 
	{
	    m_minNrValuesToPerturb = n;
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set the maximum number of values that should be perturbed when generating the offspring.
	 *
	 *@remarks
	 * - This value is capped to the number of configuration dimensions when used even
	 *   when the user initially sets it to a larger number.
	 */
	virtual void SetMaxNrValuesToPerturb(const int n)
	{
	    m_maxNrValuesToPerturb = n;
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Generate an offspring configuration near the parent configuration by perturbing one or more values.
	 * 
	 *@remarks
	 * - The parameter, m_step, is interpreted here as the standard deviation of the Gaussian perturbation.
	 * - The offspring generator first selects the number of values to be perturbed (uniformly at random from min/max values).
	 * - If the consecutive flag is set to true, the start position is selected uniformly at random (from the applicable range)
	 *   and the selected number of values are perturbed using a Gaussian distribution centered around the parent cfg.
	 * - If the consecutive flag is set to false, a subset is selected uniformly at random (from the applicable range)
	 *   and the values in the subset are perturbed using a Gaussian distribution centered around the parent cfg.
   	 */ 
	virtual void GenerateOffspringCfg(Cfg & cfg);

    protected:
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Indicate whether the perturbed values should be consecutive.
	 */ 
	bool m_consecutive;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Minimum number of values that should be perturbed.
	 */ 
	int  m_minNrValuesToPerturb;
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Maximum number of values that should be perturbed.
	 *
	 *@remarks
	 * - This value is capped to the number of configuration dimensions when used even
	 *   when the user initially sets it to a larger number.
	 */ 
	int  m_maxNrValuesToPerturb;
    };
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgOffspringGeneratorByGaussianPerturbation.
     */	 
    ClassContainer(CfgOffspringGeneratorByGaussianPerturbation, m_cfgOffspringGeneratorByGaussianPerturbation);
    
}

#endif
