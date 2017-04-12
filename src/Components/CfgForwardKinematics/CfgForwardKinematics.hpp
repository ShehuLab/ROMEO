#ifndef Antipatrea__CfgForwardKinematics_HPP_
#define Antipatrea__CfgForwardKinematics_HPP_

#include "Components/CfgManagers/CfgManager.hpp"
#include "Utils/Flags.hpp"
#include <vector>

namespace Antipatrea
{	
    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Interface for computing the forward kinematics corresponding to the specified configuration values.
     *
     *@remarks
     *  - This class requires access to CfgManager  component in order to access the
     *    configuration dimension.
     */
    class CfgForwardKinematics : public Component,
				 public CfgManagerContainer
    {
    public:
	CfgForwardKinematics(void) : Component(),
				     CfgManagerContainer(),
				     m_flags(FLAG_RECOMPUTE_FK)
	{
	}
	
	virtual ~CfgForwardKinematics(void)
	{
	}

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Copy the joint values.
	 *
	 *@remarks
	 * - The function also sets FLAG_RECOMPUTE_FK if the new values, <tt>vals</tt>, are different
	 *   from the current values. If they are the same, the flag is not set to avoid recomputing the forward
	 *   kinematics.
	 */
	virtual void CopyJointValues(const double vals[]);

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Compute the forward kinematics if FLAG_RECOMPUTE_FK has been set.
	 */
	virtual void FK(void)
	{
	    if(HasFlag(m_flags, FLAG_RECOMPUTE_FK))
	    {		
		DoFK();
		m_flags = RemoveFlag(m_flags, FLAG_RECOMPUTE_FK);
	    }
	}
	
    protected:
        /**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Set FLAG_RECOMPUTE_FK when values change.
	 */
	virtual void OnValuesChanged(void)
	{
	    m_flags = AddFlag(m_flags, FLAG_RECOMPUTE_FK);
	}
	
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Implement the forward kinematics.
	 *
	 *@remarks
	 * - This function should always compute the forward kinematics, ignoring flag values.
	 */
	virtual void DoFK(void) = 0;
	
	enum
	    {
		FLAG_RECOMPUTE_FK = 1
	    };
	
	    
	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Joint values.
	 */
	std::vector<double> m_joints;

	/**
	 *@author Erion Plaku, Amarda Shehu
	 *@brief Flags to keep track of status, e.g., whether or not FK should be recomputed.
	 */
	Flags m_flags;
	
    };

    /**
     *@author Erion Plaku, Amarda Shehu
     *@brief Get/set methods for components that need access to CfgForwardKinematics.
     */
    ClassContainer(CfgForwardKinematics, m_cfgForwardKinematics);
}

#endif
