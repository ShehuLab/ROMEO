#include "Components/CfgForwardKinematics/CfgForwardKinematics.hpp"
#include "Utils/Misc.hpp"

namespace Antipatrea
{
    void CfgForwardKinematics::CopyJointValues(const double vals[])
    {
	const int n       = GetCfgManager()->GetDim();
	bool      changed = false;

	if(n != m_joints.size())
	{
	    changed = true;
	    m_joints.resize(n);
	}
	else
	{
	    for(int i = 0; i < n; ++i)
		if(m_joints[i] != vals[i])
		{
		    changed = true;
		    break;
		}
	}
	
	if(changed)
	{
	    CopyArray<double>(&m_joints[0], n, vals);
	    OnValuesChanged();
	}
    }
}
