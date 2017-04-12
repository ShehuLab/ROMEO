#ifndef Antipatrea__CfgOffspringGeneratorToTargetByLinearInterpolation_HPP_
#define Antipatrea__CfgOffspringGeneratorToTargetByLinearInterpolation_HPP_

#include "Components/CfgOffspringGenerators/CfgOffspringGeneratorToTarget.hpp"
#include "Components/CfgDistances/SignedDistanceBetweenTwoValues.hpp"

namespace Antipatrea
{    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief The offspring generator defines a path that starts at the parent configuration
     *       and reaches the target configuration by 
     *       linearly interpolating the configuration values.
     * 
     *@remarks
     *  - The parameter, m_step, is interpreted here as time, ranging from 0 to 1.
     *  - The linear interpolation uses SignedDistanceBetweenTwoValues
     *    to determine the direction and magnitude of the change between
     *    the values being interpolated.
     *  - If SignedDistanceBetweenTwoValues points to an object from the class
     *    SignedDistanceBetweenTwoAngles, then the linear interpolation between
     *    each pairwise angles in the configuration values is done along
     *    the shortest arc connecting the two angles.
     *  - If SignedDistanceBetweenTwoValues points to an object from the class
     *    SignedDistanceBetweenTwoNumbers, then the linear interpolation 
     *    is the normal one, i.e., <tt>(1 - t) * start + t * end</tt>.
     *  - When dealing with joint values, the angle interpolation should be used.
     *  - When dealing with positions, the number interpolation should be used.
     *  - This class needs access to CfgManager to allocate auxilary configurations 
     *    and to SignedDistanceBetweenTwoValues to compute the pairwise value
     *    interpolations.
     */
    class CfgOffspringGeneratorToTargetByLinearInterpolation : public CfgOffspringGeneratorToTarget,
							       public SignedDistanceBetweenTwoValuesContainer
    {
    public:
	CfgOffspringGeneratorToTargetByLinearInterpolation(void) : CfgOffspringGeneratorToTarget(),
								   SignedDistanceBetweenTwoValuesContainer()
	{
	}
	
	virtual ~CfgOffspringGeneratorToTargetByLinearInterpolation(void)
	{
	}
	
	virtual bool CheckSetup(void) const
	{
	    return
		CfgOffspringGeneratorToTarget::CheckSetup() &&
		GetCfgManager() != NULL &&
		GetSignedDistanceBetweenTwoValues() != NULL &&
		GetCfgManager()->CheckSetup() &&
		GetSignedDistanceBetweenTwoValues()->CheckSetup();
	}
	
	
	virtual void Info(const char prefix[]) const
	{
	    CfgOffspringGeneratorToTarget::Info(prefix);
	    Logger::m_out << prefix << " CfgManager        = " << Name(GetCfgManager()) << std::endl
		          << prefix << " SignedDistanceBetweenTwoValues = " << Name(GetSignedDistanceBetweenTwoValues()) << std::endl;
	}
	
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Generate the offspring by linearly interpolating the parent and target configurations.
	 * 
	 *@remarks
	 *  - The parameter, m_step, is interpreted here as time, ranging from 0 to 1.
	 *  - The linear interpolation uses SignedDistanceBetweenTwoValues
	 *    to determine the direction and magnitude of the change between
	 *    the values being interpolated.
	 *  - If SignedDistanceBetweenTwoValues points to an object from the class
	 *    SignedDistanceBetweenTwoAngles, then the linear interpolation between
	 *    each pairwise angles in the configuration values is done along
	 *    the shortest arc connecting the two angles.
	 *  - If SignedDistanceBetweenTwoValues points to an object from the class
	 *    SignedDistanceBetweenTwoNumbers, then the linear interpolation 
	 *    is the normal one, i.e., <tt>(1 - t) * start + t * end</tt>.
	 *  - When dealing with joint values, the angle interpolation should be used.
	 *  - When dealing with positions, the number interpolation should be used.
	 */
	virtual void GenerateOffspringCfg(Cfg & cfg);
    };
    
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgOffspringGeneratorToTargetByLinearInterpolation.
     */	 
    ClassContainer(CfgOffspringGeneratorToTargetByLinearInterpolation, m_cfgOffspringGeneratorToTargetByLinearInterpolation);
    
}

#endif
